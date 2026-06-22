#!/usr/bin/env python3
"""Check spec governance links for the Fishnest repository.

The first version is intentionally report-first. Use --strict when a slice
explicitly wants governance gaps to fail the command.
"""

from __future__ import annotations

import argparse
import re
import sys
from collections import defaultdict
from pathlib import Path


ROOT = Path(__file__).resolve().parents[1]
SPEC_DIR = ROOT / "规范"
SPEC_INDEX = SPEC_DIR / "规范目录.md"
RULE_INDEX = ROOT / "docs" / "工程图谱" / "05_规则原子索引.md"
PLAN_INDEX = ROOT / "计划" / "计划索引.md"


def read_text(path: Path) -> str:
    return path.read_text(encoding="utf-8", errors="ignore")


def parse_spec_index() -> tuple[dict[str, str], dict[str, list[str]]]:
    text = read_text(SPEC_INDEX)
    by_file: dict[str, str] = {}
    by_number: dict[str, list[str]] = defaultdict(list)

    for line in text.splitlines():
        match = re.match(
            r"\|\s*([0-9]{3,9})\s*\|.*?\[[^\]]+\]\(([^)]+\.md)\)",
            line,
        )
        if not match:
            continue
        number, link = match.groups()
        name = Path(link).name
        by_file[name] = number
        by_number[number].append(name)

    return by_file, by_number


def parse_header_number(text: str) -> str | None:
    header = "\n".join(text.splitlines()[:80])
    match = re.search(r"(?m)^\s*(?:规范编号|编号)\s*[:：]\s*([0-9]{3,9})\b", header)
    return match.group(1) if match else None


def parse_rule_atoms() -> dict[str, str]:
    if not RULE_INDEX.exists():
        return {}

    text = read_text(RULE_INDEX)
    rules: dict[str, str] = {}
    for match in re.finditer(
        r"(?ms)^###\s+(RULE-[A-Z0-9-]+)[：:][^\n]*\n(.*?)(?=^###\s+RULE-|\Z)",
        text,
    ):
        rules[match.group(1)] = match.group(2)
    return rules


def source_specs_from_rules(rules: dict[str, str]) -> set[str]:
    sources: set[str] = set()
    for block in rules.values():
        for spec in re.findall(r"`?(规范/[^`\n]+?\.md)`?", block):
            sources.add(Path(spec).name)
    return sources


def section_after(label: str, block: str) -> str:
    match = re.search(rf"{re.escape(label)}\s*\n(.*?)(?:\n\n|\Z)", block, re.S)
    return match.group(1).strip() if match else ""


def main() -> int:
    parser = argparse.ArgumentParser(description="Check spec governance links.")
    parser.add_argument("--strict", action="store_true", help="return non-zero on hard gaps")
    args = parser.parse_args()

    if not SPEC_INDEX.exists():
        print(f"ERROR: missing {SPEC_INDEX.relative_to(ROOT)}")
        return 2

    directory_by_file, directory_by_number = parse_spec_index()
    spec_files = sorted(SPEC_DIR.glob("*.md"), key=lambda path: path.name)
    rules = parse_rule_atoms()
    rule_sources = source_specs_from_rules(rules)
    plan_text = read_text(PLAN_INDEX) if PLAN_INDEX.exists() else ""

    issues: list[tuple[str, str]] = []
    warnings: list[tuple[str, str]] = []

    header_numbers: dict[str, list[str]] = defaultdict(list)
    for path in spec_files:
        text = read_text(path)
        header_number = parse_header_number(text)
        directory_number = directory_by_file.get(path.name)

        if header_number:
            header_numbers[header_number].append(path.name)
        else:
            warnings.append(("missing-header-number", f"{path.relative_to(ROOT)}"))

        if not directory_number:
            issues.append(("not-in-spec-index", f"{path.relative_to(ROOT)}"))

        if header_number and directory_number and header_number != directory_number:
            issues.append(
                (
                    "number-mismatch",
                    f"{path.relative_to(ROOT)} header={header_number} index={directory_number}",
                )
            )

        if any(word in path.name for word in ("详细设计", "总结", "说明")) and "权威等级" not in text[:1000]:
            warnings.append(("reference-like-spec-needs-authority", f"{path.relative_to(ROOT)}"))

        if (
            path.name != "规范目录.md"
            and re.search(r"已废止|历史入口|历史参考", text[:2000])
            and path.name in plan_text
        ):
            issues.append(("historical-spec-used-by-plan-index", f"{path.relative_to(ROOT)}"))

        if ("必须" in text or "不得" in text) and path.name not in rule_sources:
            warnings.append(("mandatory-text-without-rule-source", f"{path.relative_to(ROOT)}"))

    for number, names in directory_by_number.items():
        if len(names) > 1:
            issues.append(("duplicate-index-number", f"{number}: {', '.join(names)}"))

    for number, names in header_numbers.items():
        if len(names) > 1:
            issues.append(("duplicate-header-number", f"{number}: {', '.join(names)}"))

    if RULE_INDEX.exists():
        for rule_id, block in rules.items():
            if not re.search(r"规范/[^`\n]+?\.md", block):
                issues.append(("rule-without-source-spec", rule_id))

            tests = section_after("相关测试：", block)
            manual = "人工审查" in block
            has_command = "--" in tests or "python" in tests or "msbuild" in tests
            if not has_command and not manual:
                issues.append(("rule-without-test-or-manual-review", rule_id))
    else:
        issues.append(("missing-rule-index", str(RULE_INDEX.relative_to(ROOT))))

    print("spec files:", len(spec_files))
    print("indexed specs:", len(directory_by_file))
    print("rule atoms:", len(rules))
    print("hard issues:", len(issues))
    print("warnings:", len(warnings))

    if issues:
        print("\nHard issues:")
        for kind, detail in issues:
            print(f"- {kind}: {detail}")

    if warnings:
        print("\nWarnings:")
        for kind, detail in warnings:
            print(f"- {kind}: {detail}")

    if args.strict and issues:
        return 1
    return 0


if __name__ == "__main__":
    sys.exit(main())
