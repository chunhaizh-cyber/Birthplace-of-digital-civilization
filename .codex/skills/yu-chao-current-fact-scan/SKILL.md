---
name: yu-chao-current-fact-scan
description: Use in D:\鱼巢 when the user says "按当前代码检查", "按当前 worktree 检查", "先扫描本地事实", "先确认当前入口和调用点", "按仓库当前状态检查", or asks Codex to confirm current local code facts before planning or editing. This skill is for current local worktree facts, not cloud `birthplace/main` review.
---

# 鱼巢当前代码事实扫描

## Core Meaning

Treat this skill as a read-only local fact scan:

```text
当前用户目标
-> 当前本地 worktree 状态
-> 当前权威规范 / 计划索引
-> 当前真实代码入口 / 调用点 / 承载结构
-> 当前 dirty state 会影响哪些判断
-> 给出只基于本地现状的结论
```

This skill answers: `现在这份本地代码到底是什么状态`。

It does not answer: `云端 birthplace/main 是什么状态`。

If the user explicitly asks for `云端代码检查`、`birthplace/main`、`远端现状`，不要把本技能当成权威基线，转去云端审查口径。

## Hard Preflight

1. Confirm the workspace is `D:\鱼巢`.
2. Read `AGENTS.md` with its limited-read rules.
3. Read at least:

```text
docs/00_文档总索引.md
规范/规范目录.md
规范/0050_项目通用结构与禁止性规则总纲_20260701.md
计划/计划索引.md
```

4. If the scan touches code structure or entrypoints, also read:

```text
docs/工程图谱/00_图谱总索引.md
docs/工程图谱/05_规则原子索引.md
说明书/函数功能说明文档.md（仅命中段）
```

5. Read current local Git evidence before concluding:

```text
git status --short --branch --untracked-files=no
git diff --name-only
git diff --stat -- <relevant-path>
```

Use path-limited Git commands when the target is scoped.

## Authority Order

Use this order:

```text
用户本轮明确要求
-> AGENTS.md
-> 当前正式规范
-> 当前计划索引
-> 当前本地代码事实
-> 当前本地 git status / diff / 未提交改动
-> 当前断点清单 / 实施记录
-> 说明书 / README / 日志 / 总结文字
-> 旧对话记忆
```

Do not silently replace `当前本地代码事实` with `birthplace/main`。

## Workflow

1. Fix the scan object in one sentence:

```text
对象 = 入口 / 函数 / 调用链 / 结构承载 / 当前切片状态 / 某计划的 S0 事实
```

2. State whether the answer is about:

```text
当前本地事实
当前本地 + dirty worktree
当前本地但受未提交改动影响
待核
```

3. Gather only the smallest needed evidence:
   - `rg` for symbols, keywords, callsites
   - short file reads
   - function-document hits
   - current local Git state

4. Split findings into:

```text
已读证据
代码事实
受未提交改动影响
合理推断
待核
```

5. If a dirty worktree may change the answer, say so directly. Do not present a mixed local/cloud judgment as one fact.

6. If the scan target is a planned implementation slice, prefer producing an `S0`-style result:

```text
真实入口
真实调用点
可复用函数
现有承载结构
缺函数 / 缺入口 / 缺证据
禁止直接宣称
```

## Required Output

Return compactly with these fields when relevant:

```text
当前扫描对象：
当前分支：
当前 worktree 是否 dirty：
本地真实入口：
本地真实调用点：
本地承载结构：
受未提交改动影响的判断：
禁止直接宣称：
下一步建议：
```

If no exact entry is found, say `未找到当前本地事实入口` and give the next smallest read-only search.

## Boundaries

- Do not edit code, plans, specs, or docs.
- Do not build or run unless the user explicitly expands scope.
- Do not treat logs, UI, README, or 说明文字 as code facts.
- Do not infer cloud state from local state.
- Do not say `已实现` unless the local code path, structure carrying path, and validation evidence are all present in the current turn.
- Do not suppress dirty-worktree ambiguity. Call it out.
