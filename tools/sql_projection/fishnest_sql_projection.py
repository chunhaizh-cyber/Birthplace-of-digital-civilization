#!/usr/bin/env python
# -*- coding: utf-8 -*-
"""Build a SQL Server query projection for Fishnest facts and runtime evidence."""

from __future__ import annotations

import argparse
import datetime as _dt
import html
import json
import os
import re
import subprocess
import sys
import uuid
from collections import Counter
from pathlib import Path
from typing import Any


FEATURE_FUNC_RE = re.compile(r"\b((?:私有_)?特征_[\w\u3400-\u9fff]+)\s*\(")
LOG_RE = re.compile(
    r"^(?P<time>\d{4}-\d{2}-\d{2} \d{2}:\d{2}:\d{2}\.\d{3}) "
    r"\[(?P<level>[A-Z]+)\] \[T(?P<thread>\d+)\] (?P<body>.*)$"
)
INTERESTING_LOG_TOKENS = (
    "动作动态",
    "来源动作动态",
    "因果",
    "派生需求",
    "提交_生成缺口子需求",
    "事实提交",
    "正式结果",
    "扫描",
    "发现事实",
    "变化事实",
)
EXCLUDED_DIRS = {".git", "x64", "Debug", "Release", "logs", "日志"}
SQL_INTEGER_COLUMNS = {
    "source_line",
    "line_no",
    "event_seq",
    "row_index",
    "message_id",
    "event_type_code",
    "occurred_time_us",
    "system_thread_id",
    "first_message_id",
    "latest_message_id",
    "created_time_us",
    "updated_time_us",
    "exit_time_us",
    "is_blocked",
    "is_paused",
    "is_healthy",
    "is_normal_exit",
    "is_exited",
    "is_fault",
    "creation_message_seen",
    "task_id",
    "work_item_id",
    "version",
    "late_message_count",
    "value_number",
}
SQL_DATETIME2_COLUMNS = {"log_time"}
SQL_COLUMN_NAMES = {
    "id": "记录标识",
    "run_id": "批次标识",
    "created_at": "创建时间",
    "workspace": "工作目录",
    "source_note": "来源备注",
    "feature_name": "特征名称",
    "source_kind": "来源类型",
    "source_path": "来源路径",
    "source_line": "来源行号",
    "symbol_name": "符号名称",
    "raw_text": "原始文本",
    "parent_feature": "父特征",
    "child_feature": "子特征",
    "relation_type": "关系类型",
    "event_seq": "事件序号",
    "log_file": "日志文件",
    "line_no": "行号",
    "log_time": "日志时间",
    "level": "级别",
    "thread_id": "线程标识",
    "event_name": "事件名称",
    "event_class": "事件类别",
    "method_name": "方法名称",
    "task_key": "任务主键",
    "demand_key": "需求主键",
    "host_key": "宿主主键",
    "feature_key": "特征主键",
    "current_state": "当前状态",
    "target_state": "目标状态",
    "result_state": "结果状态",
    "action_dynamic": "动作动态",
    "source_action_dynamic": "来源动作动态",
    "causal_key": "因果主键",
    "source_causal_key": "来源因果主键",
    "source_report_id": "来源报告标识",
    "fields_json": "字段数据",
    "metric_key": "指标键",
    "cxx_type": "代码类型",
    "panel_struct": "控制面板结构",
    "data_group": "数据分组",
    "metric_group": "指标分组",
    "value_text": "值文本",
    "value_number": "数值",
    "row_index": "行号",
    "logical_id": "逻辑标识",
    "thread_name": "线程名称",
    "thread_purpose": "线程用途",
    "thread_category": "线程类别",
    "module_name": "模块名称",
    "creator_logical_id": "创建者逻辑标识",
    "creator_name": "创建者名称",
    "thread_pool_id": "线程池标识",
    "thread_pool_name": "线程池名称",
    "lifecycle_state": "生命周期状态",
    "runtime_state": "运行状态",
    "system_thread_id": "系统线程标识",
    "first_message_id": "首次消息标识",
    "latest_message_id": "最近消息标识",
    "created_time_us": "创建时间微秒",
    "updated_time_us": "更新时间微秒",
    "exit_time_us": "退出时间微秒",
    "is_blocked": "是否阻塞",
    "is_paused": "是否暂停",
    "is_healthy": "是否健康",
    "is_exited": "是否已退出",
    "is_fault": "是否故障",
    "creation_message_seen": "创建消息已到",
    "task_id": "任务标识",
    "work_item_id": "工作项标识",
    "latest_event_type": "最近事件类型",
    "latest_reason_key": "最近原因键",
    "latest_event_summary": "最近事件摘要",
    "version": "版本",
    "late_message_count": "迟到消息数",
    "message_id": "消息标识",
    "event_type_code": "事件类型代码",
    "event_type": "事件类型",
    "occurred_time_us": "发生时间微秒",
    "old_lifecycle_state": "旧生命周期状态",
    "new_lifecycle_state": "新生命周期状态",
    "old_runtime_state": "旧运行状态",
    "new_runtime_state": "新运行状态",
    "is_normal_exit": "是否正常退出",
    "reason_key": "原因键",
    "display_summary": "显示摘要",
}


def read_text(path: Path) -> str:
    for encoding in ("utf-8-sig", "utf-8", "gb18030"):
        try:
            return path.read_text(encoding=encoding)
        except UnicodeDecodeError:
            continue
    return path.read_text(encoding="utf-8", errors="replace")


def iter_project_files(root: Path, suffixes: tuple[str, ...]) -> list[Path]:
    files: list[Path] = []
    for path in root.rglob("*"):
        if not path.is_file() or path.suffix not in suffixes:
            continue
        parts = set(path.relative_to(root).parts)
        if parts & EXCLUDED_DIRS:
            continue
        files.append(path)
    return files


def rel(path: Path, root: Path) -> str:
    try:
        return str(path.relative_to(root)).replace("\\", "/")
    except ValueError:
        return str(path).replace("\\", "/")


def unescape_kv_value(text: str) -> str:
    out: list[str] = []
    escaped = False
    for char in text:
        if not escaped:
            if char == "\\":
                escaped = True
            else:
                out.append(char)
            continue
        if char == "n":
            out.append("\n")
        elif char == "r":
            out.append("\r")
        elif char == "t":
            out.append("\t")
        elif char == "\\":
            out.append("\\")
        else:
            out.append(char)
        escaped = False
    if escaped:
        out.append("\\")
    return "".join(out)


def read_key_value_file(path: Path) -> dict[str, str]:
    fields: dict[str, str] = {}
    if not path.exists():
        return fields
    for line in read_text(path).splitlines():
        if "=" not in line:
            continue
        key, value = line.split("=", 1)
        fields[key.strip()] = unescape_kv_value(value.strip())
    return fields


def int_or_zero(value: Any) -> int:
    try:
        return int(str(value))
    except (TypeError, ValueError):
        return 0


def bool_number(value: Any) -> int:
    text = str(value or "").strip().lower()
    return 1 if text in {"1", "true", "是", "yes"} else 0


def clean_feature_name(value: str) -> str:
    name = value.strip().strip("`").strip()
    name = re.sub(r"^(私有_)?特征_", "", name)
    name = re.sub(r"_筹办(?:查询)?$", "", name)
    return name


def add_feature(features: list[dict[str, Any]], seen: set[tuple[str, str, str, int, str]], row: dict[str, Any]) -> None:
    key = (
        row.get("source_kind") or "",
        row.get("feature_name") or "",
        row.get("source_path") or "",
        int(row.get("source_line") or 0),
        row.get("symbol_name") or "",
    )
    if key in seen:
        return
    seen.add(key)
    features.append(row)


def extract_feature_accessors(root: Path, features: list[dict[str, Any]], seen: set[tuple[str, str, str, int, str]]) -> None:
    for path in iter_project_files(root, (".h", ".cpp", ".ixx")):
        text = read_text(path)
        for line_no, line in enumerate(text.splitlines(), 1):
            if "特征_" not in line:
                continue
            if "语素入口节点类" not in line and "noexcept" not in line:
                continue
            for match in FEATURE_FUNC_RE.finditer(line):
                symbol = match.group(1)
                literal = ""
                literal_match = re.search(r'["“]([^"”]+)["”]', line)
                if literal_match:
                    literal = literal_match.group(1).strip()
                feature_name = literal or clean_feature_name(symbol)
                add_feature(
                    features,
                    seen,
                    {
                        "feature_name": feature_name,
                        "source_kind": "source_accessor",
                        "source_path": rel(path, root),
                        "source_line": line_no,
                        "symbol_name": symbol,
                        "raw_text": line.strip(),
                    },
                )


def split_backtick_features(text: str) -> list[str]:
    names = [clean_feature_name(item) for item in re.findall(r"`([^`]+)`", text)]
    return [name for name in names if name and name not in {"无", "空"}]


def extract_feature_dictionary(root: Path, features: list[dict[str, Any]], seen: set[tuple[str, str, str, int, str]]) -> list[dict[str, Any]]:
    relations: list[dict[str, Any]] = []
    path = root / "详细设计" / "特征类型串联字典.md"
    if not path.exists():
        return relations
    for line_no, line in enumerate(read_text(path).splitlines(), 1):
        if not line.startswith("|") or "---" in line or "特征类型" in line:
            continue
        cells = [cell.strip() for cell in line.strip("|").split("|")]
        if len(cells) < 3:
            continue
        name = clean_feature_name(cells[0])
        if not name:
            continue
        add_feature(
            features,
            seen,
            {
                "feature_name": name,
                "source_kind": "design_dictionary",
                "source_path": rel(path, root),
                "source_line": line_no,
                "symbol_name": "",
                "raw_text": line.strip(),
            },
        )
        for upstream in split_backtick_features(cells[1]):
            relations.append(
                {
                    "parent_feature": upstream,
                    "child_feature": name,
                    "relation_type": "直接上游",
                    "source_path": rel(path, root),
                    "source_line": line_no,
                    "raw_text": line.strip(),
                }
            )
        for downstream in split_backtick_features(cells[2]):
            relations.append(
                {
                    "parent_feature": name,
                    "child_feature": downstream,
                    "relation_type": "直接下游",
                    "source_path": rel(path, root),
                    "source_line": line_no,
                    "raw_text": line.strip(),
                }
            )
    return relations


def tree_node_name(text: str) -> str:
    text = text.strip()
    text = re.sub(r"\s+[|，,;；].*$", "", text)
    return clean_feature_name(text)


def extract_feature_tree(root: Path, features: list[dict[str, Any]], seen: set[tuple[str, str, str, int, str]]) -> list[dict[str, Any]]:
    relations: list[dict[str, Any]] = []
    path = root / "详细设计" / "特征双根树.md"
    if not path.exists():
        return relations
    in_tree = False
    stack: dict[int, str] = {}
    for line_no, line in enumerate(read_text(path).splitlines(), 1):
        if line.strip() == "```text":
            in_tree = True
            continue
        if in_tree and line.strip() == "```":
            in_tree = False
            continue
        if not in_tree:
            if line.startswith("- ") and "->" in line:
                left, right = line[2:].split("->", 1)
                left_name = tree_node_name(left)
                right_name = tree_node_name(right)
                if left_name and right_name:
                    relations.append(
                        {
                            "parent_feature": left_name,
                            "child_feature": right_name,
                            "relation_type": "非树反馈边",
                            "source_path": rel(path, root),
                            "source_line": line_no,
                            "raw_text": line.strip(),
                        }
                    )
            continue
        if line.strip() == "双根树":
            stack = {0: "双根树"}
            continue
        match = re.match(r"^(?P<prefix>[ │]*)(?:├─|└─)\s*(?P<name>.+)$", line)
        if not match:
            continue
        prefix = match.group("prefix")
        level = (len(prefix.replace("│", " ")) // 3) + 1
        name = tree_node_name(match.group("name"))
        if not name:
            continue
        add_feature(
            features,
            seen,
            {
                "feature_name": name,
                "source_kind": "dual_root_tree",
                "source_path": rel(path, root),
                "source_line": line_no,
                "symbol_name": "",
                "raw_text": line.strip(),
            },
        )
        parent = stack.get(level - 1)
        if parent:
            relations.append(
                {
                    "parent_feature": parent,
                    "child_feature": name,
                    "relation_type": "双根树主父子",
                    "source_path": rel(path, root),
                    "source_line": line_no,
                    "raw_text": line.strip(),
                }
            )
        stack[level] = name
        for old_level in list(stack.keys()):
            if old_level > level:
                stack.pop(old_level, None)
    return relations


CXX_FIELD_RE = re.compile(
    r"^\s*(?P<cxx_type>[\w:\u3400-\u9fff]+(?:<[^>]+>)?)\s+"
    r"(?P<field_name>[\w\u3400-\u9fff_]+)\s*(?:=|\{|\;)"
)


def panel_field_group(field_name: str) -> str:
    ordered_prefixes = (
        ("任务管理界面线程", "任务管理界面线程"),
        ("任务管理工作线程池", "任务管理工作线程池"),
        ("任务管理工作线程", "任务管理工作线程"),
        ("控制面板摘要", "控制面板摘要"),
        ("自我场景", "自我场景"),
        ("自我线程", "自我线程"),
        ("需求树", "需求树"),
        ("缺口", "缺口治理"),
        ("自检", "自检"),
        ("任务", "任务"),
        ("方法", "方法"),
        ("因果", "因果"),
        ("线程", "线程"),
        ("世界", "世界"),
        ("基础信息", "世界基础"),
    )
    for prefix, group in ordered_prefixes:
        if field_name.startswith(prefix):
            return group
    if "线程" in field_name:
        return "线程"
    if "需求" in field_name:
        return "需求"
    if "任务" in field_name:
        return "任务"
    if "方法" in field_name:
        return "方法"
    if "动态" in field_name:
        return "动态"
    if "因果" in field_name:
        return "因果"
    return "运行概览"


def extract_struct_fields(root: Path, source_path: str, struct_name: str, source_kind: str) -> list[dict[str, Any]]:
    path = root / source_path
    if not path.exists():
        return []
    rows: list[dict[str, Any]] = []
    in_struct = False
    for line_no, line in enumerate(read_text(path).splitlines(), 1):
        stripped = line.strip()
        if not in_struct:
            if re.search(rf"\bstruct\s+{re.escape(struct_name)}\b", stripped):
                in_struct = True
            continue
        if stripped.startswith("};"):
            break
        match = CXX_FIELD_RE.match(line)
        if not match:
            continue
        field_name = match.group("field_name")
        cxx_type = match.group("cxx_type")
        rows.append(
            {
                "metric_key": field_name,
                "cxx_type": cxx_type,
                "panel_struct": struct_name,
                "data_group": panel_field_group(field_name),
                "source_kind": source_kind,
                "source_path": source_path,
                "source_line": line_no,
                "raw_text": stripped,
            }
        )
    return rows


def extract_panel_metric_catalog(root: Path) -> list[dict[str, Any]]:
    rows = extract_struct_fields(
        root,
        "控制面板类.ixx",
        "结构_控制面板快照",
        "panel_snapshot_contract",
    )
    rows.extend(
        extract_struct_fields(
            root,
            "控制面板摘要线程模块.ixx",
            "结构_控制面板摘要快照",
            "panel_summary_contract",
        )
    )
    return rows


def parse_log_fields(body: str) -> tuple[str, dict[str, str]]:
    parts = [part.strip() for part in body.split(" | ")]
    event_name = parts[0] if parts else body.strip()
    fields: dict[str, str] = {}
    for part in parts[1:]:
        if "=" not in part:
            continue
        key, value = part.split("=", 1)
        fields[key.strip()] = value.strip()
    return event_name, fields


def classify_event(event_name: str, fields: dict[str, str]) -> str:
    if fields.get("动作动态"):
        return "动作动态"
    if "提交_生成缺口子需求" in event_name:
        return "派生需求入树"
    if "任务筹办跟踪" in event_name:
        return "方法候选"
    if "因果" in event_name or any("因果" in key for key in fields):
        return "因果"
    if "动态" in event_name or any("动态" in key for key in fields):
        return "动态"
    if "扫描" in event_name:
        return "扫描"
    if "提交" in event_name:
        return "提交"
    return "运行事件"


def is_interesting_log(body: str) -> bool:
    return any(token in body for token in INTERESTING_LOG_TOKENS)


def latest_run_logs(root: Path, count: int) -> list[Path]:
    log_dir = root / "日志"
    if not log_dir.exists():
        return []
    logs = sorted(log_dir.glob("鱼巢_run_*.低值g"), key=lambda item: item.stat().st_mtime, reverse=True)
    return logs[:count]


def extract_runtime_events(root: Path, logs: list[Path], max_events: int) -> list[dict[str, Any]]:
    events: list[dict[str, Any]] = []
    for log_path in logs:
        with log_path.open("r", encoding="utf-8", errors="replace") as handle:
            for line_no, line in enumerate(handle, 1):
                if max_events and len(events) >= max_events:
                    return events
                line = line.rstrip("\n")
                if not is_interesting_log(line):
                    continue
                match = LOG_RE.match(line)
                if not match:
                    continue
                event_name, fields = parse_log_fields(match.group("body"))
                event_id = len(events) + 1
                action_dynamic = fields.get("动作动态") or ""
                source_action_dynamic = fields.get("来源动作动态") or ""
                source_causal_key = fields.get("来源因果键") or fields.get("入树来源因果键") or fields.get("来源因果") or ""
                causal_key = fields.get("因果") or fields.get("根因果") or fields.get("来源因果信息ID") or ""
                row = {
                    "event_seq": event_id,
                    "log_file": rel(log_path, root),
                    "line_no": line_no,
                    "log_time": match.group("time"),
                    "level": match.group("level"),
                    "thread_id": match.group("thread"),
                    "event_name": event_name,
                    "event_class": classify_event(event_name, fields),
                    "method_name": fields.get("方法") or fields.get("本能动作") or fields.get("当前方法") or "",
                    "task_key": fields.get("任务") or fields.get("关联任务") or "",
                    "demand_key": fields.get("需求") or fields.get("来源需求") or fields.get("父需求") or "",
                    "host_key": fields.get("目标主体") or fields.get("当前存在") or fields.get("目标宿主") or "",
                    "feature_key": fields.get("目标特征") or fields.get("需求特征") or fields.get("特征类型") or fields.get("主果") or "",
                    "current_state": fields.get("当前状态") or "",
                    "target_state": fields.get("目标状态") or "",
                    "result_state": fields.get("实际结果状态") or fields.get("方法运行结果") or fields.get("处理结果") or "",
                    "action_dynamic": action_dynamic,
                    "source_action_dynamic": source_action_dynamic,
                    "causal_key": causal_key,
                    "source_causal_key": source_causal_key,
                    "source_report_id": fields.get("来源报告标识") or fields.get("扫描报告ID") or "",
                    "fields_json": json.dumps(fields, ensure_ascii=False, separators=(",", ":")),
                    "raw_text": line,
                }
                events.append(row)
    return events


def extract_control_panel_thread_info(root: Path) -> list[dict[str, Any]]:
    path = root / "消息中间件" / "control_panel_thread_info_table.cache"
    fields = read_key_value_file(path)
    count = int_or_zero(fields.get("项数"))
    rows: list[dict[str, Any]] = []
    for index in range(count):
        prefix = f"项.{index}."
        item = {key[len(prefix):]: value for key, value in fields.items() if key.startswith(prefix)}
        if not item:
            continue
        rows.append(
            {
                "row_index": index,
                "logical_id": item.get("线程逻辑ID", "") or f"row-{index}",
                "thread_name": item.get("线程名称", ""),
                "thread_purpose": item.get("线程用途", ""),
                "thread_category": item.get("线程类别", ""),
                "module_name": item.get("所属模块", ""),
                "creator_logical_id": item.get("创建者逻辑ID", ""),
                "creator_name": item.get("创建者名称", ""),
                "thread_pool_id": item.get("所属线程池ID", ""),
                "thread_pool_name": item.get("所属线程池名称", ""),
                "lifecycle_state": item.get("生命周期状态", ""),
                "runtime_state": item.get("运行状态", ""),
                "system_thread_id": int_or_zero(item.get("系统线程ID")),
                "first_message_id": int_or_zero(item.get("首次消息ID")),
                "latest_message_id": int_or_zero(item.get("最近消息ID")),
                "created_time_us": int_or_zero(item.get("创建时间")),
                "updated_time_us": int_or_zero(item.get("最近更新时间")),
                "exit_time_us": int_or_zero(item.get("退出时间")),
                "is_blocked": bool_number(item.get("是否堵塞")),
                "is_paused": bool_number(item.get("是否暂停")),
                "is_healthy": bool_number(item.get("是否健康")),
                "is_exited": bool_number(item.get("是否已退出")),
                "is_fault": bool_number(item.get("是否故障")),
                "creation_message_seen": bool_number(item.get("创建消息已到")),
                "task_id": int_or_zero(item.get("关联任务ID")),
                "work_item_id": int_or_zero(item.get("关联工作项ID")),
                "latest_event_type": item.get("最近事件类型", ""),
                "latest_reason_key": item.get("最近原因键", ""),
                "latest_event_summary": item.get("最近事件摘要", ""),
                "version": int_or_zero(item.get("版本")),
                "late_message_count": int_or_zero(item.get("迟到消息数")),
                "source_path": rel(path, root),
                "fields_json": json.dumps(item, ensure_ascii=False, separators=(",", ":")),
            }
        )
    return rows


def thread_lifecycle_message_paths(root: Path, max_events: int) -> list[Path]:
    message_dir = root / "消息中间件"
    if not message_dir.exists():
        return []
    paths: list[Path] = []
    for folder in (message_dir, message_dir / "已消费"):
        if folder.exists():
            paths.extend(path for path in folder.glob("thread_lifecycle_*.msg") if path.is_file())
    paths = sorted(paths, key=lambda item: item.stat().st_mtime, reverse=True)
    if max_events > 0:
        paths = paths[:max_events]
    return paths


def extract_thread_lifecycle_events(root: Path, max_events: int) -> list[dict[str, Any]]:
    rows: list[dict[str, Any]] = []
    seen_message_ids: set[int] = set()
    for path in thread_lifecycle_message_paths(root, max_events):
        fields = read_key_value_file(path)
        message_id = int_or_zero(fields.get("消息ID"))
        if message_id and message_id in seen_message_ids:
            continue
        if message_id:
            seen_message_ids.add(message_id)
        rows.append(
            {
                "message_id": message_id,
                "event_type_code": int_or_zero(fields.get("事件类型代码")),
                "event_type": fields.get("事件类型", ""),
                "occurred_time_us": int_or_zero(fields.get("发生时间")),
                "logical_id": fields.get("线程逻辑ID", ""),
                "system_thread_id": int_or_zero(fields.get("系统线程ID")),
                "thread_name": fields.get("线程名称", ""),
                "thread_purpose": fields.get("线程用途", ""),
                "thread_category": fields.get("线程类别", ""),
                "module_name": fields.get("所属模块", ""),
                "creator_logical_id": fields.get("创建者逻辑ID", ""),
                "creator_name": fields.get("创建者名称", ""),
                "thread_pool_id": fields.get("所属线程池ID", ""),
                "thread_pool_name": fields.get("所属线程池名称", ""),
                "old_lifecycle_state": fields.get("旧生命周期状态", ""),
                "new_lifecycle_state": fields.get("新生命周期状态", ""),
                "old_runtime_state": fields.get("旧运行状态", ""),
                "new_runtime_state": fields.get("新运行状态", ""),
                "is_blocked": bool_number(fields.get("是否堵塞")),
                "is_paused": bool_number(fields.get("是否暂停")),
                "is_healthy": bool_number(fields.get("是否健康")),
                "is_normal_exit": bool_number(fields.get("是否正常退出")),
                "task_id": int_or_zero(fields.get("关联任务ID")),
                "work_item_id": int_or_zero(fields.get("关联工作项ID")),
                "reason_key": fields.get("原因键", ""),
                "display_summary": fields.get("显示摘要", ""),
                "source_path": rel(path, root),
                "fields_json": json.dumps(fields, ensure_ascii=False, separators=(",", ":")),
            }
        )
    rows.sort(key=lambda row: (row.get("occurred_time_us") or 0, row.get("message_id") or 0))
    return rows


def add_panel_metric(
    rows: list[dict[str, Any]],
    metric_key: str,
    metric_group: str,
    value: Any,
    source_kind: str,
    source_path: str = "",
    source_line: int = 0,
    raw_text: str = "",
) -> None:
    value_number = value if isinstance(value, int) and not isinstance(value, bool) else None
    if isinstance(value, bool):
        value_number = 1 if value else 0
    rows.append(
        {
            "metric_key": metric_key,
            "metric_group": metric_group,
            "value_text": str(value),
            "value_number": value_number,
            "source_kind": source_kind,
            "source_path": source_path,
            "source_line": source_line,
            "raw_text": raw_text,
        }
    )


def build_panel_runtime_metrics(
    summary: dict[str, Any],
    events: list[dict[str, Any]],
    thread_info: list[dict[str, Any]],
    thread_events: list[dict[str, Any]],
) -> list[dict[str, Any]]:
    rows: list[dict[str, Any]] = []
    add_panel_metric(rows, "特征类型记录数", "基础信息", summary["feature_count"], "sql_projection_summary")
    add_panel_metric(rows, "特征关系记录数", "基础信息", summary["feature_relation_count"], "sql_projection_summary")
    add_panel_metric(rows, "运行事件记录数", "运行事实", summary["event_count"], "runtime_log_projection")
    add_panel_metric(rows, "动作动态事件数", "运行事实", summary["action_dynamic_count"], "runtime_log_projection")
    add_panel_metric(rows, "线程信息项数", "线程", len(thread_info), "message_middleware_thread_cache", "消息中间件/control_panel_thread_info_table.cache")
    add_panel_metric(rows, "线程生命周期事件数", "线程", len(thread_events), "message_middleware_lifecycle_messages", "消息中间件")

    add_panel_metric(rows, "当前运行线程数", "线程", sum(1 for row in thread_info if row.get("lifecycle_state") == "运行中"), "message_middleware_thread_cache")
    add_panel_metric(rows, "健康线程数", "线程", sum(1 for row in thread_info if row.get("is_healthy") == 1), "message_middleware_thread_cache")
    add_panel_metric(rows, "故障线程数", "线程", sum(1 for row in thread_info if row.get("is_fault") == 1 or row.get("lifecycle_state") == "故障"), "message_middleware_thread_cache")
    add_panel_metric(rows, "已退出线程数", "线程", sum(1 for row in thread_info if row.get("is_exited") == 1 or row.get("lifecycle_state") in {"已退出", "已停止"}), "message_middleware_thread_cache")

    event_classes = Counter(row.get("event_class") or "未分类" for row in events)
    for event_class, count in sorted(event_classes.items()):
        add_panel_metric(rows, f"运行事件_{event_class}", "运行事实", count, "runtime_log_projection")

    lifecycle_states = Counter(row.get("lifecycle_state") or "未记录" for row in thread_info)
    for state, count in sorted(lifecycle_states.items()):
        add_panel_metric(rows, f"线程状态_{state}", "线程", count, "message_middleware_thread_cache")
    return rows


def sql_string(value: Any, null_empty: bool = True) -> str:
    if value is None:
        return "NULL"
    text = str(value)
    if null_empty and text == "":
        return "NULL"
    text = text.replace("'", "''")
    return "N'" + text + "'"


def sql_int(value: Any) -> str:
    try:
        return str(int(value))
    except (TypeError, ValueError):
        return "NULL"


def validate_database_name(name: str) -> str:
    if not name or any(char in name for char in "\0\r\n"):
        raise ValueError("Database name must not be empty or contain control line breaks.")
    return name


def sql_identifier(name: str) -> str:
    return "[" + name.replace("]", "]]") + "]"


def sql_column_identifier(source_key: str) -> str:
    return sql_identifier(SQL_COLUMN_NAMES.get(source_key, source_key))


def write_insert_lines(out: list[str], table: str, columns: list[str], rows: list[dict[str, Any]], run_id: str) -> None:
    sql_columns = [sql_column_identifier(column) for column in columns]
    for index, row in enumerate(rows, 1):
        values = []
        for column in columns:
            if column == "run_id":
                values.append("'" + run_id + "'")
            elif column in SQL_INTEGER_COLUMNS:
                values.append(sql_int(row.get(column)))
            elif column in SQL_DATETIME2_COLUMNS:
                value = row.get(column)
                values.append("CONVERT(datetime2(3), " + sql_string(value) + ", 121)" if value else "NULL")
            else:
                values.append(sql_string(row.get(column)))
        out.append(f"INSERT INTO {table} ({', '.join(sql_columns)}) VALUES ({', '.join(values)});")
        if index % 400 == 0:
            out.append("GO")


def build_sql(database: str, run_id: str, root: Path, payload: dict[str, Any]) -> str:
    database = validate_database_name(database)
    database_literal = sql_string(database, null_empty=False)
    database_identifier = sql_identifier(database)
    now = _dt.datetime.now().isoformat(timespec="seconds")
    lines: list[str] = [
        "SET NOCOUNT ON;",
        "SET ANSI_NULLS ON;",
        "SET QUOTED_IDENTIFIER ON;",
        f"IF DB_ID({database_literal}) IS NULL CREATE DATABASE {database_identifier};",
        "GO",
        f"USE {database_identifier};",
        "GO",
        "SET ANSI_NULLS ON;",
        "SET QUOTED_IDENTIFIER ON;",
        "GO",
        "IF SCHEMA_ID(N'鱼巢') IS NULL EXEC(N'CREATE SCHEMA [鱼巢]');",
        "GO",
        """
DROP VIEW IF EXISTS [鱼巢].[最新控制面板线程生命周期事件];
DROP VIEW IF EXISTS [鱼巢].[最新控制面板线程信息];
DROP VIEW IF EXISTS [鱼巢].[最新控制面板运行指标];
DROP VIEW IF EXISTS [鱼巢].[最新控制面板字段目录];
DROP VIEW IF EXISTS [鱼巢].[最新特征关系];
DROP VIEW IF EXISTS [鱼巢].[最新特征类型];
DROP VIEW IF EXISTS [鱼巢].[最新动作动态];
DROP VIEW IF EXISTS [鱼巢].[最新批次];
DROP TABLE IF EXISTS [鱼巢].[控制面板线程生命周期事件];
DROP TABLE IF EXISTS [鱼巢].[控制面板线程信息];
DROP TABLE IF EXISTS [鱼巢].[控制面板运行指标];
DROP TABLE IF EXISTS [鱼巢].[控制面板字段目录];
DROP TABLE IF EXISTS [鱼巢].[运行事件];
DROP TABLE IF EXISTS [鱼巢].[特征关系];
DROP TABLE IF EXISTS [鱼巢].[特征类型];
DROP TABLE IF EXISTS [鱼巢].[投影批次];
CREATE TABLE [鱼巢].[投影批次] (
    [批次标识] uniqueidentifier NOT NULL PRIMARY KEY,
    [创建时间] datetime2(0) NOT NULL,
    [工作目录] nvarchar(500) NOT NULL,
    [来源备注] nvarchar(max) NULL
);
CREATE TABLE [鱼巢].[特征类型] (
    [记录标识] bigint IDENTITY(1,1) NOT NULL PRIMARY KEY,
    [批次标识] uniqueidentifier NOT NULL,
    [特征名称] nvarchar(300) NOT NULL,
    [来源类型] nvarchar(80) NOT NULL,
    [来源路径] nvarchar(500) NULL,
    [来源行号] int NULL,
    [符号名称] nvarchar(300) NULL,
    [原始文本] nvarchar(max) NULL
);
CREATE TABLE [鱼巢].[特征关系] (
    [记录标识] bigint IDENTITY(1,1) NOT NULL PRIMARY KEY,
    [批次标识] uniqueidentifier NOT NULL,
    [父特征] nvarchar(300) NOT NULL,
    [子特征] nvarchar(300) NOT NULL,
    [关系类型] nvarchar(80) NOT NULL,
    [来源路径] nvarchar(500) NULL,
    [来源行号] int NULL,
    [原始文本] nvarchar(max) NULL
);
CREATE TABLE [鱼巢].[运行事件] (
    [记录标识] bigint IDENTITY(1,1) NOT NULL PRIMARY KEY,
    [批次标识] uniqueidentifier NOT NULL,
    [事件序号] int NOT NULL,
    [日志文件] nvarchar(500) NOT NULL,
    [行号] int NOT NULL,
    [日志时间] datetime2(3) NULL,
    [级别] nvarchar(20) NULL,
    [线程标识] nvarchar(40) NULL,
    [事件名称] nvarchar(300) NOT NULL,
    [事件类别] nvarchar(80) NOT NULL,
    [方法名称] nvarchar(300) NULL,
    [任务主键] nvarchar(80) NULL,
    [需求主键] nvarchar(80) NULL,
    [宿主主键] nvarchar(80) NULL,
    [特征主键] nvarchar(300) NULL,
    [当前状态] nvarchar(120) NULL,
    [目标状态] nvarchar(120) NULL,
    [结果状态] nvarchar(120) NULL,
    [动作动态] nvarchar(120) NULL,
    [来源动作动态] nvarchar(120) NULL,
    [因果主键] nvarchar(120) NULL,
    [来源因果主键] nvarchar(120) NULL,
    [来源报告标识] nvarchar(120) NULL,
    [字段数据] nvarchar(max) NULL,
    [原始文本] nvarchar(max) NULL
);
CREATE TABLE [鱼巢].[控制面板字段目录] (
    [记录标识] bigint IDENTITY(1,1) NOT NULL PRIMARY KEY,
    [批次标识] uniqueidentifier NOT NULL,
    [指标键] nvarchar(300) NOT NULL,
    [代码类型] nvarchar(160) NOT NULL,
    [控制面板结构] nvarchar(160) NOT NULL,
    [数据分组] nvarchar(80) NOT NULL,
    [来源类型] nvarchar(80) NOT NULL,
    [来源路径] nvarchar(500) NULL,
    [来源行号] int NULL,
    [原始文本] nvarchar(max) NULL
);
CREATE TABLE [鱼巢].[控制面板运行指标] (
    [记录标识] bigint IDENTITY(1,1) NOT NULL PRIMARY KEY,
    [批次标识] uniqueidentifier NOT NULL,
    [指标键] nvarchar(300) NOT NULL,
    [指标分组] nvarchar(80) NOT NULL,
    [值文本] nvarchar(max) NULL,
    [数值] bigint NULL,
    [来源类型] nvarchar(80) NOT NULL,
    [来源路径] nvarchar(500) NULL,
    [来源行号] int NULL,
    [原始文本] nvarchar(max) NULL
);
CREATE TABLE [鱼巢].[控制面板线程信息] (
    [记录标识] bigint IDENTITY(1,1) NOT NULL PRIMARY KEY,
    [批次标识] uniqueidentifier NOT NULL,
    [行号] int NOT NULL,
    [逻辑标识] nvarchar(160) NOT NULL,
    [线程名称] nvarchar(300) NULL,
    [线程用途] nvarchar(max) NULL,
    [线程类别] nvarchar(120) NULL,
    [模块名称] nvarchar(300) NULL,
    [创建者逻辑标识] nvarchar(160) NULL,
    [创建者名称] nvarchar(300) NULL,
    [线程池标识] nvarchar(160) NULL,
    [线程池名称] nvarchar(300) NULL,
    [生命周期状态] nvarchar(80) NULL,
    [运行状态] nvarchar(160) NULL,
    [系统线程标识] bigint NULL,
    [首次消息标识] bigint NULL,
    [最近消息标识] bigint NULL,
    [创建时间微秒] bigint NULL,
    [更新时间微秒] bigint NULL,
    [退出时间微秒] bigint NULL,
    [是否阻塞] bit NULL,
    [是否暂停] bit NULL,
    [是否健康] bit NULL,
    [是否已退出] bit NULL,
    [是否故障] bit NULL,
    [创建消息已到] bit NULL,
    [任务标识] bigint NULL,
    [工作项标识] bigint NULL,
    [最近事件类型] nvarchar(120) NULL,
    [最近原因键] nvarchar(300) NULL,
    [最近事件摘要] nvarchar(max) NULL,
    [版本] bigint NULL,
    [迟到消息数] bigint NULL,
    [来源路径] nvarchar(500) NULL,
    [字段数据] nvarchar(max) NULL
);
CREATE TABLE [鱼巢].[控制面板线程生命周期事件] (
    [记录标识] bigint IDENTITY(1,1) NOT NULL PRIMARY KEY,
    [批次标识] uniqueidentifier NOT NULL,
    [消息标识] bigint NOT NULL,
    [事件类型代码] int NULL,
    [事件类型] nvarchar(120) NULL,
    [发生时间微秒] bigint NULL,
    [逻辑标识] nvarchar(160) NULL,
    [系统线程标识] bigint NULL,
    [线程名称] nvarchar(300) NULL,
    [线程用途] nvarchar(max) NULL,
    [线程类别] nvarchar(120) NULL,
    [模块名称] nvarchar(300) NULL,
    [创建者逻辑标识] nvarchar(160) NULL,
    [创建者名称] nvarchar(300) NULL,
    [线程池标识] nvarchar(160) NULL,
    [线程池名称] nvarchar(300) NULL,
    [旧生命周期状态] nvarchar(80) NULL,
    [新生命周期状态] nvarchar(80) NULL,
    [旧运行状态] nvarchar(160) NULL,
    [新运行状态] nvarchar(160) NULL,
    [是否阻塞] bit NULL,
    [是否暂停] bit NULL,
    [是否健康] bit NULL,
    [是否正常退出] bit NULL,
    [任务标识] bigint NULL,
    [工作项标识] bigint NULL,
    [原因键] nvarchar(300) NULL,
    [显示摘要] nvarchar(max) NULL,
    [来源路径] nvarchar(500) NULL,
    [字段数据] nvarchar(max) NULL
);
""",
        "GO",
        """
IF NOT EXISTS (SELECT 1 FROM sys.indexes WHERE name = N'IX_feature_type_run_name' AND object_id = OBJECT_ID(N'[鱼巢].[特征类型]'))
    CREATE INDEX IX_feature_type_run_name ON [鱼巢].[特征类型]([批次标识], [特征名称]);
IF NOT EXISTS (SELECT 1 FROM sys.indexes WHERE name = N'IX_feature_relation_run_parent' AND object_id = OBJECT_ID(N'[鱼巢].[特征关系]'))
    CREATE INDEX IX_feature_relation_run_parent ON [鱼巢].[特征关系]([批次标识], [父特征], [子特征]);
IF NOT EXISTS (SELECT 1 FROM sys.indexes WHERE name = N'IX_runtime_event_run_class' AND object_id = OBJECT_ID(N'[鱼巢].[运行事件]'))
    CREATE INDEX IX_runtime_event_run_class ON [鱼巢].[运行事件]([批次标识], [事件类别], [日志时间]);
IF NOT EXISTS (SELECT 1 FROM sys.indexes WHERE name = N'IX_runtime_event_action_dynamic' AND object_id = OBJECT_ID(N'[鱼巢].[运行事件]'))
    CREATE INDEX IX_runtime_event_action_dynamic ON [鱼巢].[运行事件]([批次标识], [动作动态]) WHERE [动作动态] IS NOT NULL;
IF NOT EXISTS (SELECT 1 FROM sys.indexes WHERE name = N'IX_panel_metric_catalog_run_key' AND object_id = OBJECT_ID(N'[鱼巢].[控制面板字段目录]'))
    CREATE INDEX IX_panel_metric_catalog_run_key ON [鱼巢].[控制面板字段目录]([批次标识], [指标键]);
IF NOT EXISTS (SELECT 1 FROM sys.indexes WHERE name = N'IX_panel_runtime_metric_run_key' AND object_id = OBJECT_ID(N'[鱼巢].[控制面板运行指标]'))
    CREATE INDEX IX_panel_runtime_metric_run_key ON [鱼巢].[控制面板运行指标]([批次标识], [指标键]);
IF NOT EXISTS (SELECT 1 FROM sys.indexes WHERE name = N'IX_panel_thread_info_run_logical' AND object_id = OBJECT_ID(N'[鱼巢].[控制面板线程信息]'))
    CREATE INDEX IX_panel_thread_info_run_logical ON [鱼巢].[控制面板线程信息]([批次标识], [逻辑标识]);
IF NOT EXISTS (SELECT 1 FROM sys.indexes WHERE name = N'IX_panel_thread_lifecycle_run_logical' AND object_id = OBJECT_ID(N'[鱼巢].[控制面板线程生命周期事件]'))
    CREATE INDEX IX_panel_thread_lifecycle_run_logical ON [鱼巢].[控制面板线程生命周期事件]([批次标识], [逻辑标识], [发生时间微秒]);
""",
        "GO",
        f"INSERT INTO [鱼巢].[投影批次] ([批次标识], [创建时间], [工作目录], [来源备注]) VALUES ('{run_id}', CONVERT(datetime2(0), N'{now}', 126), {sql_string(str(root))}, {sql_string(payload['source_note'])});",
    ]

    write_insert_lines(
        lines,
        "[鱼巢].[特征类型]",
        ["run_id", "feature_name", "source_kind", "source_path", "source_line", "symbol_name", "raw_text"],
        payload["features"],
        run_id,
    )
    write_insert_lines(
        lines,
        "[鱼巢].[特征关系]",
        ["run_id", "parent_feature", "child_feature", "relation_type", "source_path", "source_line", "raw_text"],
        payload["feature_relations"],
        run_id,
    )
    write_insert_lines(
        lines,
        "[鱼巢].[运行事件]",
        [
            "run_id",
            "event_seq",
            "log_file",
            "line_no",
            "log_time",
            "level",
            "thread_id",
            "event_name",
            "event_class",
            "method_name",
            "task_key",
            "demand_key",
            "host_key",
            "feature_key",
            "current_state",
            "target_state",
            "result_state",
            "action_dynamic",
            "source_action_dynamic",
            "causal_key",
            "source_causal_key",
            "source_report_id",
            "fields_json",
            "raw_text",
        ],
        payload["events"],
        run_id,
    )
    write_insert_lines(
        lines,
        "[鱼巢].[控制面板字段目录]",
        ["run_id", "metric_key", "cxx_type", "panel_struct", "data_group", "source_kind", "source_path", "source_line", "raw_text"],
        payload["panel_metric_catalog"],
        run_id,
    )
    write_insert_lines(
        lines,
        "[鱼巢].[控制面板运行指标]",
        ["run_id", "metric_key", "metric_group", "value_text", "value_number", "source_kind", "source_path", "source_line", "raw_text"],
        payload["panel_runtime_metrics"],
        run_id,
    )
    write_insert_lines(
        lines,
        "[鱼巢].[控制面板线程信息]",
        [
            "run_id",
            "row_index",
            "logical_id",
            "thread_name",
            "thread_purpose",
            "thread_category",
            "module_name",
            "creator_logical_id",
            "creator_name",
            "thread_pool_id",
            "thread_pool_name",
            "lifecycle_state",
            "runtime_state",
            "system_thread_id",
            "first_message_id",
            "latest_message_id",
            "created_time_us",
            "updated_time_us",
            "exit_time_us",
            "is_blocked",
            "is_paused",
            "is_healthy",
            "is_exited",
            "is_fault",
            "creation_message_seen",
            "task_id",
            "work_item_id",
            "latest_event_type",
            "latest_reason_key",
            "latest_event_summary",
            "version",
            "late_message_count",
            "source_path",
            "fields_json",
        ],
        payload["panel_thread_info"],
        run_id,
    )
    write_insert_lines(
        lines,
        "[鱼巢].[控制面板线程生命周期事件]",
        [
            "run_id",
            "message_id",
            "event_type_code",
            "event_type",
            "occurred_time_us",
            "logical_id",
            "system_thread_id",
            "thread_name",
            "thread_purpose",
            "thread_category",
            "module_name",
            "creator_logical_id",
            "creator_name",
            "thread_pool_id",
            "thread_pool_name",
            "old_lifecycle_state",
            "new_lifecycle_state",
            "old_runtime_state",
            "new_runtime_state",
            "is_blocked",
            "is_paused",
            "is_healthy",
            "is_normal_exit",
            "task_id",
            "work_item_id",
            "reason_key",
            "display_summary",
            "source_path",
            "fields_json",
        ],
        payload["panel_thread_lifecycle_events"],
        run_id,
    )
    lines.extend(
        [
            "GO",
            """
CREATE OR ALTER VIEW [鱼巢].[最新批次] AS
SELECT TOP (1) *
FROM [鱼巢].[投影批次]
ORDER BY [创建时间] DESC;
""",
            "GO",
            """
CREATE OR ALTER VIEW [鱼巢].[最新动作动态] AS
SELECT e.*
FROM [鱼巢].[运行事件] e
WHERE e.[批次标识] = (SELECT [批次标识] FROM [鱼巢].[最新批次])
  AND e.[动作动态] IS NOT NULL;
""",
            "GO",
            """
CREATE OR ALTER VIEW [鱼巢].[最新特征类型] AS
SELECT f.*
FROM [鱼巢].[特征类型] f
WHERE f.[批次标识] = (SELECT [批次标识] FROM [鱼巢].[最新批次]);
""",
            "GO",
            """
CREATE OR ALTER VIEW [鱼巢].[最新特征关系] AS
SELECT r.*
FROM [鱼巢].[特征关系] r
WHERE r.[批次标识] = (SELECT [批次标识] FROM [鱼巢].[最新批次]);
""",
            "GO",
            """
CREATE OR ALTER VIEW [鱼巢].[最新控制面板字段目录] AS
SELECT c.*
FROM [鱼巢].[控制面板字段目录] c
WHERE c.[批次标识] = (SELECT [批次标识] FROM [鱼巢].[最新批次]);
""",
            "GO",
            """
CREATE OR ALTER VIEW [鱼巢].[最新控制面板运行指标] AS
SELECT m.*
FROM [鱼巢].[控制面板运行指标] m
WHERE m.[批次标识] = (SELECT [批次标识] FROM [鱼巢].[最新批次]);
""",
            "GO",
            """
CREATE OR ALTER VIEW [鱼巢].[最新控制面板线程信息] AS
SELECT t.*
FROM [鱼巢].[控制面板线程信息] t
WHERE t.[批次标识] = (SELECT [批次标识] FROM [鱼巢].[最新批次]);
""",
            "GO",
            """
CREATE OR ALTER VIEW [鱼巢].[最新控制面板线程生命周期事件] AS
SELECT e.*
FROM [鱼巢].[控制面板线程生命周期事件] e
WHERE e.[批次标识] = (SELECT [批次标识] FROM [鱼巢].[最新批次]);
""",
            "GO",
        ]
    )
    return "\n".join(lines)


def html_table_rows(rows: list[dict[str, Any]], columns: list[str]) -> str:
    body = []
    for row in rows:
        cells = "".join(f"<td>{html.escape(str(row.get(col, '') or ''))}</td>" for col in columns)
        body.append(f"<tr>{cells}</tr>")
    return "\n".join(body)


def write_html(path: Path, payload: dict[str, Any], run_id: str, database: str, server: str) -> None:
    features = payload["features"][:1500]
    relations = payload["feature_relations"][:1500]
    events = payload["events"][-2500:]
    panel_metrics = payload["panel_runtime_metrics"][:1500]
    panel_catalog = payload["panel_metric_catalog"][:2000]
    thread_info = payload["panel_thread_info"][:1000]
    thread_events = payload["panel_thread_lifecycle_events"][-2500:]
    summary = payload["summary"]
    html_text = f"""<!doctype html>
<html lang="zh-CN">
<head>
  <meta charset="utf-8">
  <meta name="viewport" content="width=device-width, initial-scale=1">
  <title>鱼巢 SQL 投影视图</title>
  <style>
    :root {{ color-scheme: light; font-family: "Microsoft YaHei", "Segoe UI", sans-serif; }}
    body {{ margin: 0; background: #f4f6f8; color: #1d2733; }}
    header {{ padding: 22px 28px; background: #1f2937; color: #fff; }}
    h1 {{ margin: 0 0 8px; font-size: 24px; font-weight: 650; letter-spacing: 0; }}
    header p {{ margin: 4px 0; color: #d6dde8; }}
    main {{ padding: 22px 28px 40px; }}
    .metrics {{ display: grid; grid-template-columns: repeat(auto-fit, minmax(180px, 1fr)); gap: 12px; margin-bottom: 18px; }}
    .metric {{ background: #fff; border: 1px solid #d8dee8; border-radius: 8px; padding: 14px; }}
    .metric b {{ display: block; font-size: 24px; margin-bottom: 4px; }}
    .tabs {{ display: flex; gap: 8px; flex-wrap: wrap; margin: 14px 0; }}
    button {{ border: 1px solid #cbd5e1; background: #fff; padding: 8px 12px; border-radius: 6px; cursor: pointer; }}
    button.active {{ background: #2563eb; color: #fff; border-color: #2563eb; }}
    input {{ width: min(720px, 100%); padding: 10px 12px; border: 1px solid #cbd5e1; border-radius: 6px; margin-bottom: 12px; }}
    section {{ display: none; background: #fff; border: 1px solid #d8dee8; border-radius: 8px; padding: 14px; }}
    section.active {{ display: block; }}
    .table-wrap {{ overflow: auto; max-height: 70vh; border: 1px solid #e5e7eb; }}
    table {{ border-collapse: collapse; width: 100%; font-size: 13px; }}
    th, td {{ border-bottom: 1px solid #e5e7eb; padding: 8px 10px; text-align: left; vertical-align: top; white-space: nowrap; }}
    th {{ position: sticky; top: 0; background: #f8fafc; z-index: 1; }}
    .note {{ color: #526173; font-size: 13px; line-height: 1.6; }}
    code {{ background: #eef2f7; padding: 1px 4px; border-radius: 4px; }}
  </style>
</head>
<body>
  <header>
    <h1>鱼巢 SQL 投影视图</h1>
    <p>数据库：<code>{html.escape(database)}</code>；实例：<code>{html.escape(server)}</code>；批次：<code>{html.escape(run_id)}</code></p>
    <p>此页面是 SQL 投影批次的静态快照，不是权威世界树写入入口。</p>
  </header>
  <main>
    <div class="metrics">
      <div class="metric"><b>{summary['feature_count']}</b><span>特征类型记录</span></div>
      <div class="metric"><b>{summary['feature_relation_count']}</b><span>特征关系记录</span></div>
      <div class="metric"><b>{summary['event_count']}</b><span>运行事件记录</span></div>
      <div class="metric"><b>{summary['action_dynamic_count']}</b><span>动作动态事件</span></div>
      <div class="metric"><b>{summary['panel_metric_catalog_count']}</b><span>控制面板字段目录</span></div>
      <div class="metric"><b>{summary['panel_runtime_metric_count']}</b><span>控制面板运行指标</span></div>
      <div class="metric"><b>{summary['panel_thread_info_count']}</b><span>线程信息项</span></div>
      <div class="metric"><b>{summary['panel_thread_event_count']}</b><span>线程生命周期事件</span></div>
    </div>
    <p class="note">常用 SQL 视图：<code>[鱼巢].[最新特征类型]</code>、<code>[鱼巢].[最新特征关系]</code>、<code>[鱼巢].[最新动作动态]</code>、<code>[鱼巢].[最新控制面板运行指标]</code>、<code>[鱼巢].[最新控制面板线程信息]</code>。</p>
    <input id="filter" type="search" placeholder="过滤当前页表格文本">
    <div class="tabs">
      <button class="active" data-target="panelMetrics">面板指标</button>
      <button data-target="threads">线程信息</button>
      <button data-target="threadEvents">线程事件</button>
      <button data-target="panelCatalog">字段目录</button>
      <button data-target="events">运行事件</button>
      <button data-target="features">特征类型</button>
      <button data-target="relations">特征关系</button>
    </div>
    <section id="panelMetrics" class="active">
      <div class="table-wrap"><table data-filterable><thead><tr><th>指标</th><th>分组</th><th>值</th><th>来源</th><th>文件</th></tr></thead><tbody>
      {html_table_rows(panel_metrics, ['metric_key','metric_group','value_text','source_kind','source_path'])}
      </tbody></table></div>
    </section>
    <section id="threads">
      <div class="table-wrap"><table data-filterable><thead><tr><th>逻辑ID</th><th>线程名</th><th>生命周期</th><th>运行状态</th><th>健康</th><th>模块</th><th>最近原因</th><th>最近摘要</th></tr></thead><tbody>
      {html_table_rows(thread_info, ['logical_id','thread_name','lifecycle_state','runtime_state','is_healthy','module_name','latest_reason_key','latest_event_summary'])}
      </tbody></table></div>
    </section>
    <section id="threadEvents">
      <div class="table-wrap"><table data-filterable><thead><tr><th>消息ID</th><th>事件</th><th>线程</th><th>旧生命周期</th><th>新生命周期</th><th>旧运行</th><th>新运行</th><th>原因</th><th>摘要</th><th>文件</th></tr></thead><tbody>
      {html_table_rows(thread_events, ['message_id','event_type','logical_id','old_lifecycle_state','new_lifecycle_state','old_runtime_state','new_runtime_state','reason_key','display_summary','source_path'])}
      </tbody></table></div>
    </section>
    <section id="panelCatalog">
      <div class="table-wrap"><table data-filterable><thead><tr><th>字段</th><th>类型</th><th>结构</th><th>分组</th><th>来源</th><th>文件</th><th>行</th></tr></thead><tbody>
      {html_table_rows(panel_catalog, ['metric_key','cxx_type','panel_struct','data_group','source_kind','source_path','source_line'])}
      </tbody></table></div>
    </section>
    <section id="events">
      <div class="table-wrap"><table data-filterable><thead><tr><th>时间</th><th>类</th><th>事件</th><th>方法</th><th>任务</th><th>需求</th><th>特征</th><th>动作动态</th><th>来源动态</th><th>日志</th><th>行</th></tr></thead><tbody>
      {html_table_rows(events, ['log_time','event_class','event_name','method_name','task_key','demand_key','feature_key','action_dynamic','source_action_dynamic','log_file','line_no'])}
      </tbody></table></div>
    </section>
    <section id="features">
      <div class="table-wrap"><table data-filterable><thead><tr><th>特征</th><th>来源</th><th>符号</th><th>文件</th><th>行</th></tr></thead><tbody>
      {html_table_rows(features, ['feature_name','source_kind','symbol_name','source_path','source_line'])}
      </tbody></table></div>
    </section>
    <section id="relations">
      <div class="table-wrap"><table data-filterable><thead><tr><th>父/上游</th><th>子/下游</th><th>关系</th><th>文件</th><th>行</th></tr></thead><tbody>
      {html_table_rows(relations, ['parent_feature','child_feature','relation_type','source_path','source_line'])}
      </tbody></table></div>
    </section>
  </main>
  <script>
    const buttons = Array.from(document.querySelectorAll('button[data-target]'));
    const sections = Array.from(document.querySelectorAll('section'));
    const filter = document.getElementById('filter');
    function applyFilter() {{
      const query = filter.value.trim().toLowerCase();
      const active = document.querySelector('section.active table[data-filterable]');
      if (!active) return;
      for (const row of active.tBodies[0].rows) {{
        row.style.display = !query || row.textContent.toLowerCase().includes(query) ? '' : 'none';
      }}
    }}
    buttons.forEach(button => button.addEventListener('click', () => {{
      buttons.forEach(item => item.classList.toggle('active', item === button));
      sections.forEach(section => section.classList.toggle('active', section.id === button.dataset.target));
      applyFilter();
    }}));
    filter.addEventListener('input', applyFilter);
  </script>
</body>
</html>
"""
    path.write_text(html_text, encoding="utf-8")


def build_payload(root: Path, logs: list[Path], max_events: int, max_thread_events: int) -> dict[str, Any]:
    features: list[dict[str, Any]] = []
    feature_seen: set[tuple[str, str, str, int, str]] = set()
    extract_feature_accessors(root, features, feature_seen)
    feature_relations = extract_feature_dictionary(root, features, feature_seen)
    feature_relations.extend(extract_feature_tree(root, features, feature_seen))
    events = extract_runtime_events(root, logs, max_events)
    panel_metric_catalog = extract_panel_metric_catalog(root)
    panel_thread_info = extract_control_panel_thread_info(root)
    panel_thread_lifecycle_events = extract_thread_lifecycle_events(root, max_thread_events)
    summary = {
        "feature_count": len(features),
        "feature_relation_count": len(feature_relations),
        "event_count": len(events),
        "action_dynamic_count": sum(1 for row in events if row.get("action_dynamic")),
        "panel_metric_catalog_count": len(panel_metric_catalog),
        "panel_thread_info_count": len(panel_thread_info),
        "panel_thread_event_count": len(panel_thread_lifecycle_events),
        "logs": [rel(log, root) for log in logs],
    }
    panel_runtime_metrics = build_panel_runtime_metrics(
        summary,
        events,
        panel_thread_info,
        panel_thread_lifecycle_events,
    )
    summary["panel_runtime_metric_count"] = len(panel_runtime_metrics)
    source_note = (
        "SQL projection only; authoritative state remains in Fishnest world tree, "
        "message middleware and action dynamic chain. Logs: "
        + ", ".join(rel(log, root) for log in logs)
    )
    return {
        "source_note": source_note,
        "summary": summary,
        "features": features,
        "feature_relations": feature_relations,
        "events": events,
        "panel_metric_catalog": panel_metric_catalog,
        "panel_runtime_metrics": panel_runtime_metrics,
        "panel_thread_info": panel_thread_info,
        "panel_thread_lifecycle_events": panel_thread_lifecycle_events,
    }


def apply_sql(server: str, sql_path: Path, root: Path) -> dict[str, Any]:
    command = ["sqlcmd", "-S", server, "-E", "-b", "-f", "65001", "-i", str(sql_path)]
    result = subprocess.run(command, cwd=str(root), text=True, capture_output=True)
    return {
        "command": " ".join(command),
        "returncode": result.returncode,
        "stdout": result.stdout[-4000:],
        "stderr": result.stderr[-4000:],
    }


def main() -> int:
    parser = argparse.ArgumentParser(description="Create SQL Server projection and HTML view for Fishnest data.")
    parser.add_argument("--workspace", default=".", help="Project root, default current directory.")
    parser.add_argument("--server", default=r".\SQLEXPRESS", help="SQL Server instance.")
    parser.add_argument("--database", default="鱼巢投影库", help="Target database name.")
    parser.add_argument("--latest-runs", type=int, default=2, help="Number of latest 鱼巢_run logs to read.")
    parser.add_argument("--log", action="append", default=[], help="Additional or explicit log path. Can be repeated.")
    parser.add_argument("--max-events", type=int, default=60000, help="Maximum runtime events to import; 0 means no cap.")
    parser.add_argument("--max-thread-events", type=int, default=2000, help="Maximum latest thread lifecycle messages to import; 0 means no cap.")
    parser.add_argument("--out-dir", default="运行输出/sql_projection", help="Output directory.")
    parser.add_argument("--no-apply", action="store_true", help="Only generate SQL/JSON/HTML, do not call sqlcmd.")
    args = parser.parse_args()

    root = Path(args.workspace).resolve()
    out_dir = (root / args.out_dir).resolve()
    out_dir.mkdir(parents=True, exist_ok=True)
    explicit_logs = [(root / item).resolve() for item in args.log]
    logs = explicit_logs or latest_run_logs(root, args.latest_runs)
    logs = [log for log in logs if log.exists()]
    run_id = str(uuid.uuid4())
    payload = build_payload(root, logs, args.max_events, args.max_thread_events)
    sql_text = build_sql(args.database, run_id, root, payload)

    payload_path = out_dir / "fishnest_projection_data.json"
    sql_path = out_dir / "fishnest_projection_import.sql"
    html_path = out_dir / "fishnest_projection_view.html"
    report_path = out_dir / "fishnest_projection_report.json"
    payload_path.write_text(json.dumps({"run_id": run_id, **payload}, ensure_ascii=False, indent=2), encoding="utf-8")
    sql_path.write_text(sql_text, encoding="utf-8-sig")
    write_html(html_path, payload, run_id, args.database, args.server)

    sql_result: dict[str, Any] | None = None
    if not args.no_apply:
        sql_result = apply_sql(args.server, sql_path, root)
    report = {
        "run_id": run_id,
        "server": args.server,
        "database": args.database,
        "workspace": str(root),
        "outputs": {
            "json": str(payload_path),
            "sql": str(sql_path),
            "html": str(html_path),
        },
        "summary": payload["summary"],
        "sqlcmd": sql_result,
    }
    report_path.write_text(json.dumps(report, ensure_ascii=False, indent=2), encoding="utf-8")
    print(json.dumps(report, ensure_ascii=False, indent=2))
    if sql_result and sql_result["returncode"] != 0:
        return sql_result["returncode"]
    return 0


if __name__ == "__main__":
    raise SystemExit(main())
