from __future__ import annotations

import bisect
import datetime as _dt
import html
import re
import xml.etree.ElementTree as ET
from dataclasses import dataclass
from pathlib import Path


ROOT = Path(__file__).resolve().parents[1]
OUTPUT = ROOT / "说明书" / "函数功能说明文档.md"
PROJECT = ROOT / "鱼巢.vcxproj"

SOURCE_SUFFIXES = {".cpp", ".cc", ".cxx", ".h", ".hpp", ".hxx", ".ixx", ".inl"}
DECLARATION_SUFFIXES = {".h", ".hpp", ".hxx", ".inl"}
EXCLUDED_DIRS = {
    ".git",
    ".vs",
    ".uploads",
    "x64",
    "Debug",
    "Release",
    "logs",
    "日志",
    "运行输出",
    "third_party",
}

CONTROL_NAMES = {
    "if",
    "for",
    "while",
    "switch",
    "catch",
    "sizeof",
    "alignof",
    "decltype",
    "static_assert",
    "return",
    "new",
    "delete",
}

STATEMENT_PREFIXES = (
    "if ",
    "for ",
    "while ",
    "switch ",
    "return ",
    "co_return ",
    "throw ",
    "static_assert",
    "using ",
    "typedef ",
)

VERB_NOTES = [
    ("初始化", "初始化相关对象、状态或运行上下文。"),
    ("创建", "创建并返回或登记对应对象。"),
    ("生成", "根据当前输入生成目标数据、场景、动态或回执。"),
    ("构建", "构建运行所需的数据结构或中间结果。"),
    ("取得", "取得指定对象、状态或运行材料。"),
    ("获取", "读取并返回指定对象、状态或运行材料。"),
    ("读取", "从指定来源读取数据或状态。"),
    ("写入", "把处理结果写入指定对象、场景或日志。"),
    ("设置", "设置对象字段、状态或运行参数。"),
    ("更新", "更新已有对象、状态、索引或缓存。"),
    ("同步", "同步线程、任务、场景或对象之间的状态。"),
    ("提交", "提交事实、动态、任务状态或运行回执。"),
    ("记录", "记录日志、动态、证据或运行痕迹。"),
    ("登记", "登记方法、模板、对象或运行入口。"),
    ("注册", "注册方法、模板、对象或运行入口。"),
    ("查找", "按条件查找目标对象、方法或事实。"),
    ("搜索", "在候选范围内搜索目标对象或路径。"),
    ("选择", "从候选集中选择当前使用对象或方法。"),
    ("绑定", "建立对象、任务、方法或因果之间的绑定关系。"),
    ("确保", "确保目标结构、状态或前置条件存在并可用。"),
    ("检查", "检查状态、条件、缺口或运行约束。"),
    ("判断", "判断条件、状态或业务结果是否成立。"),
    ("评估", "评估状态、风险、质量、权重或完成度。"),
    ("比较", "比较当前值、目标值或历史基准。"),
    ("计算", "计算权重、状态、差值或派生结果。"),
    ("转换", "把输入数据转换为目标类型、语义或结构。"),
    ("解析", "解析输入文本、消息、场景或运行包。"),
    ("序列化", "把对象转换为可保存或可传输形式。"),
    ("反序列化", "从保存或传输形式恢复对象。"),
    ("清理", "清理缓存、临时状态或运行资源。"),
    ("删除", "删除指定对象、状态或登记项。"),
    ("重置", "重置对象、线程、任务或运行状态。"),
    ("启动", "启动线程、模块或运行流程。"),
    ("停止", "停止线程、模块或运行流程。"),
    ("等待", "等待线程、任务、外设或条件变化。"),
    ("运行", "执行对应模块、线程或方法的运行逻辑。"),
    ("执行", "执行任务、方法或动作的主体逻辑。"),
    ("筹办", "推进任务筹办，形成候选、缺口或就绪回执。"),
    ("派发", "派发任务、消息或运行请求。"),
    ("唤醒", "唤醒等待中的任务、线程或治理链。"),
    ("结算", "结算任务、需求、价值或结果贡献。"),
    ("扫描", "执行扫描相关材料获取、比较或提交逻辑。"),
    ("识别", "执行识别相关匹配、确认或结果输出逻辑。"),
    ("跟踪", "执行跟踪相关定位、匹配或结果输出逻辑。"),
    ("确认", "确认候选事实、存在、状态或运行结果。"),
    ("补齐", "补齐条件、参数、证据或方法能力缺口。"),
    ("导出", "导出数据、文档、状态或可视化内容。"),
    ("导入", "导入数据、配置或外部材料。"),
    ("打印", "输出调试、诊断或展示文本。"),
]


@dataclass(frozen=True)
class FunctionRecord:
    file: Path
    line: int
    kind: str
    name: str
    signature: str
    note: str
    note_source: str
    project_item: str


def read_text(path: Path) -> str:
    return path.read_text(encoding="utf-8-sig", errors="replace")


def project_items() -> dict[str, str]:
    if not PROJECT.exists():
        return {}
    ns = {"msb": "http://schemas.microsoft.com/developer/msbuild/2003"}
    tree = ET.parse(PROJECT)
    items: dict[str, str] = {}
    for tag in ("ClCompile", "ClInclude", "None"):
        for node in tree.findall(f".//msb:{tag}", ns):
            include = node.get("Include")
            if include:
                items[include.replace("\\", "/")] = tag
    return items


def is_excluded(path: Path) -> bool:
    rel = path.relative_to(ROOT)
    return any(part in EXCLUDED_DIRS for part in rel.parts)


def source_files() -> list[Path]:
    files: list[Path] = []
    for path in ROOT.rglob("*"):
        if not path.is_file() or path.suffix not in SOURCE_SUFFIXES:
            continue
        if is_excluded(path):
            continue
        files.append(path)
    return sorted(files, key=lambda p: p.relative_to(ROOT).as_posix())


def mask_code(text: str) -> str:
    out: list[str] = []
    i = 0
    n = len(text)
    while i < n:
        ch = text[i]
        nxt = text[i + 1] if i + 1 < n else ""
        if ch == "/" and nxt == "/":
            out.extend("  ")
            i += 2
            while i < n and text[i] != "\n":
                out.append(" ")
                i += 1
            continue
        if ch == "/" and nxt == "*":
            out.extend("  ")
            i += 2
            while i < n:
                if text[i] == "*" and i + 1 < n and text[i + 1] == "/":
                    out.extend("  ")
                    i += 2
                    break
                out.append("\n" if text[i] == "\n" else " ")
                i += 1
            continue
        if ch == "R" and nxt == '"':
            m = re.match(r'R"([^\s()\\]{0,16})\(', text[i:])
            if m:
                delim = m.group(1)
                end = ")" + delim + '"'
                end_pos = text.find(end, i + len(m.group(0)))
                stop = n if end_pos == -1 else end_pos + len(end)
                while i < stop:
                    out.append("\n" if text[i] == "\n" else " ")
                    i += 1
                continue
        if ch in {'"', "'"}:
            quote = ch
            out.append(" ")
            i += 1
            while i < n:
                cur = text[i]
                out.append("\n" if cur == "\n" else " ")
                if cur == "\\" and i + 1 < n:
                    i += 2
                    if i <= n:
                        out.append(" " if i <= n else "")
                    continue
                i += 1
                if cur == quote:
                    break
            continue
        out.append(ch)
        i += 1
    return "".join(out)


def line_starts(text: str) -> list[int]:
    starts = [0]
    for m in re.finditer("\n", text):
        starts.append(m.end())
    return starts


def line_for(starts: list[int], offset: int) -> int:
    return bisect.bisect_right(starts, offset)


def matching_open_paren(masked: str, close: int) -> int | None:
    depth = 1
    i = close - 1
    while i >= 0:
        ch = masked[i]
        if ch == ")":
            depth += 1
        elif ch == "(":
            depth -= 1
            if depth == 0:
                return i
        i -= 1
    return None


def find_signature_start(masked: str, open_paren: int) -> int:
    i = open_paren - 1
    while i >= 0:
        if masked[i] in ";{}":
            return i + 1
        if masked[i] == "\n":
            prev = i - 1
            while prev >= 0 and masked[prev] in " \t\r":
                prev -= 1
            if prev >= 0 and masked[prev] == "\n":
                return i + 1
        i -= 1
    return 0


def clean_signature(sig: str) -> str:
    sig = re.sub(r"^\s*(public|private|protected)\s*:\s*", "", sig)
    sig = re.sub(r"\s+", " ", sig).strip()
    sig = sig.replace("|", "\\|")
    return sig


def extract_name(sig_before_open: str) -> str | None:
    prefix = re.sub(r"\s+", " ", sig_before_open).strip()
    if not prefix:
        return None
    if prefix.endswith("]"):
        return None
    op = re.search(r"(operator\s*(?:\(\)|\[\]|[^\s(]+(?:\s+[^\s(]+)?))\s*$", prefix)
    if op:
        return op.group(1).strip()
    m = re.search(r"((?:[\w~]+::)*~?[\w]+)\s*$", prefix, re.UNICODE)
    if not m:
        return None
    return m.group(1)


def candidate_is_function(signature: str, name: str, declaration: bool) -> bool:
    simple = name.split("::")[-1].replace("~", "")
    if simple in CONTROL_NAMES:
        return False
    stripped = signature.strip()
    if not stripped or stripped[0] in {")", ","}:
        return False
    if "#" in stripped:
        return False
    if re.search(r"\b(class|struct|enum|union)\b", stripped):
        return False
    lowered = signature.strip().lower()
    if lowered.startswith(STATEMENT_PREFIXES):
        return False
    before = signature.rsplit("(", 1)[0]
    if "." in before or "->" in before:
        return False
    name_pos = signature.rfind(name)
    if name_pos > 0 and re.search(r"\)\s*:", signature[:name_pos]):
        return False
    if not declaration and "=" in signature and not name.startswith("operator"):
        return False
    if not declaration and "?" in signature:
        return False
    if declaration and "=" in signature and not re.search(r"=\s*(0|default|delete)\s*$", signature):
        return False
    if declaration and signature.count("(") != signature.count(")"):
        return False
    return True


def previous_comment(lines: list[str], line_no: int) -> tuple[str, str]:
    idx = line_no - 2
    skipped_blank = 0
    while idx >= 0 and not lines[idx].strip() and skipped_blank < 1:
        skipped_blank += 1
        idx -= 1
    if idx < 0:
        return "", ""
    stripped = lines[idx].strip()
    if stripped.startswith("//"):
        chunks: list[str] = []
        while idx >= 0 and lines[idx].strip().startswith("//"):
            chunks.append(re.sub(r"^\s*//\s?", "", lines[idx]).strip())
            idx -= 1
        text = " ".join(reversed([c for c in chunks if c]))
        return summarize_comment(text), "源码注释"
    if stripped.endswith("*/"):
        chunks = []
        while idx >= 0:
            chunks.append(lines[idx].strip())
            if "/*" in lines[idx]:
                break
            idx -= 1
        raw = "\n".join(reversed(chunks))
        raw = re.sub(r"^/\*", "", raw.strip())
        raw = re.sub(r"\*/$", "", raw.strip())
        raw = re.sub(r"^\s*\*\s?", "", raw, flags=re.MULTILINE)
        return summarize_comment(raw), "源码注释"
    return "", ""


def summarize_comment(text: str) -> str:
    text = re.sub(r"\s+", " ", text).strip()
    text = re.sub(r"^(说明|功能|用途)[:：]\s*", "", text)
    if not text:
        return ""
    if len(text) > 120:
        return text[:117] + "..."
    return text


def note_from_name(name: str) -> str:
    simple = name.split("::")[-1]
    if simple == "main" or simple == "wmain":
        return "程序入口，负责启动项目主流程。"
    if simple.startswith("~"):
        return "析构函数，负责释放或收束该对象持有的资源。"
    owner = name.split("::")[-2] if "::" in name else ""
    if owner and simple == owner:
        return "构造函数，负责初始化该对象实例。"
    for verb, note in VERB_NOTES:
        if verb in simple:
            return note
    if simple.startswith("私有_"):
        return "内部辅助函数，服务所在模块的私有流程。"
    return "源码未提供直接注释；当前备注由函数名自动登记，需在复核时补充更精确功能。"


def extract_definitions(path: Path, text: str, masked: str, items: dict[str, str]) -> list[FunctionRecord]:
    starts = line_starts(masked)
    lines = text.splitlines()
    records: list[FunctionRecord] = []
    rel = path.relative_to(ROOT).as_posix()
    project_item = items.get(rel, "未在vcxproj登记")
    for m in re.finditer(r"\{", masked):
        brace = m.start()
        prefix_start = max(0, brace - 1200)
        prefix = masked[prefix_start:brace]
        close_positions = [prefix_start + p.start() for p in re.finditer(r"\)", prefix)]
        for close in reversed(close_positions):
            between = masked[close + 1 : brace]
            if re.search(r"[;{}]", between):
                continue
            open_paren = matching_open_paren(masked, close)
            if open_paren is None:
                continue
            start = find_signature_start(masked, open_paren)
            signature_original = text[start:brace].strip()
            name = extract_name(masked[start:open_paren])
            if not name:
                continue
            signature = clean_signature(signature_original)
            if not signature or not candidate_is_function(signature, name, declaration=False):
                continue
            line = line_for(starts, start)
            comment, source = previous_comment(lines, line)
            note = comment or note_from_name(name)
            records.append(
                FunctionRecord(
                    file=path,
                    line=line,
                    kind="定义",
                    name=name,
                    signature=signature,
                    note=note,
                    note_source=source or "名称推断",
                    project_item=project_item,
                )
            )
            break
    return records


def extract_declarations(path: Path, text: str, masked: str, items: dict[str, str]) -> list[FunctionRecord]:
    if path.suffix not in DECLARATION_SUFFIXES:
        return []
    starts = line_starts(masked)
    lines = text.splitlines()
    records: list[FunctionRecord] = []
    rel = path.relative_to(ROOT).as_posix()
    project_item = items.get(rel, "未在vcxproj登记")
    for m in re.finditer(r";", masked):
        semi = m.start()
        prefix = masked[max(0, semi - 500) : semi]
        close_rel = prefix.rfind(")")
        if close_rel < 0:
            continue
        close = max(0, semi - 500) + close_rel
        if masked[close + 1 : semi].strip() not in {"", "const", "noexcept", "override", "final", "= 0", "= default", "= delete"}:
            continue
        open_paren = matching_open_paren(masked, close)
        if open_paren is None:
            continue
        start = find_signature_start(masked, open_paren)
        signature_original = text[start:semi].strip()
        if "{" in masked[start:semi] or "}" in masked[start:semi]:
            continue
        name = extract_name(masked[start:open_paren])
        if not name:
            continue
        signature = clean_signature(signature_original)
        if not signature or not candidate_is_function(signature, name, declaration=True):
            continue
        line = line_for(starts, start)
        comment, source = previous_comment(lines, line)
        note = comment or note_from_name(name)
        records.append(
            FunctionRecord(
                file=path,
                line=line,
                kind="声明",
                name=name,
                signature=signature,
                note=note,
                note_source=source or "名称推断",
                project_item=project_item,
            )
        )
    return records


def deduplicate(records: list[FunctionRecord]) -> list[FunctionRecord]:
    seen: set[tuple[str, int, str, str]] = set()
    result: list[FunctionRecord] = []
    for record in records:
        key = (record.file.relative_to(ROOT).as_posix(), record.line, record.kind, record.signature)
        if key in seen:
            continue
        seen.add(key)
        result.append(record)
    return result


def markdown_escape(text: str) -> str:
    return html.escape(text, quote=False).replace("|", "\\|")


def render(records: list[FunctionRecord], files: list[Path]) -> str:
    now = _dt.datetime.now().strftime("%Y-%m-%d %H:%M:%S")
    project_count = sum(1 for r in records if r.project_item != "未在vcxproj登记")
    inferred_count = sum(1 for r in records if r.note_source == "名称推断")
    by_file: dict[Path, list[FunctionRecord]] = {}
    for record in records:
        by_file.setdefault(record.file, []).append(record)

    out: list[str] = []
    out.append("# 函数功能说明文档")
    out.append("")
    out.append(f"更新时间：{now}")
    out.append("")
    out.append("生成命令：")
    out.append("")
    out.append("```powershell")
    out.append("python .\\tools\\生成函数功能说明文档.py")
    out.append("```")
    out.append("")
    out.append("## 使用规则")
    out.append("")
    out.append("1. 新建函数前，先在本文档中搜索拟建函数名、关键动词和目标对象，确认没有同名或同功能函数。")
    out.append("2. 若已有函数能覆盖需求，优先复用或在原函数职责内修改；确需新增时，在新增后重新运行生成命令更新本文档。")
    out.append("3. 没有自定义类归属、且需要跨模块复用的共享函数，统一放入 `全局共享函数类.ixx`；只服务单个源文件的局部辅助函数仍保留在所在文件的匿名命名空间或局部作用域。")
    out.append("4. 本文档只登记项目自有源码函数；默认排除 `third_party`、构建目录、日志目录和运行输出目录。")
    out.append("5. `名称推断` 说明表示源码附近没有直接功能注释，后续修改该函数时应顺手补充更准确说明。")
    out.append("")
    out.append("## 统计")
    out.append("")
    out.append(f"- 扫描源码文件：{len(files)}")
    out.append(f"- 登记函数条目：{len(records)}")
    out.append(f"- 已在 `鱼巢.vcxproj` 登记的函数条目：{project_count}")
    out.append(f"- 需要后续人工复核的名称推断条目：{inferred_count}")
    out.append("")
    out.append("## 函数清单")
    out.append("")
    for path in sorted(by_file, key=lambda p: p.relative_to(ROOT).as_posix()):
        rel = path.relative_to(ROOT).as_posix()
        out.append(f"### {rel}")
        out.append("")
        out.append("| 行 | 类型 | 项目登记 | 函数 | 功能备注 | 说明来源 |")
        out.append("| --- | --- | --- | --- | --- | --- |")
        for record in sorted(by_file[path], key=lambda r: (r.line, r.kind, r.name)):
            out.append(
                "| {line} | {kind} | {project} | `{name}`<br>`{sig}` | {note} | {source} |".format(
                    line=record.line,
                    kind=record.kind,
                    project=markdown_escape(record.project_item),
                    name=markdown_escape(record.name),
                    sig=markdown_escape(record.signature),
                    note=markdown_escape(record.note),
                    source=markdown_escape(record.note_source),
                )
            )
        out.append("")
    return "\n".join(out) + "\n"


def main() -> None:
    items = project_items()
    files = source_files()
    records: list[FunctionRecord] = []
    for path in files:
        text = read_text(path)
        masked = mask_code(text)
        records.extend(extract_definitions(path, text, masked, items))
        records.extend(extract_declarations(path, text, masked, items))
    records = deduplicate(records)
    OUTPUT.parent.mkdir(parents=True, exist_ok=True)
    OUTPUT.write_text(render(records, files), encoding="utf-8")
    print(f"已生成：{OUTPUT}")
    print(f"源码文件：{len(files)}")
    print(f"函数条目：{len(records)}")


if __name__ == "__main__":
    main()
