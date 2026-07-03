---
name: yu-chao-pre-implementation-review
description: Use in D:\鱼巢 when the user says "实施前审查", "先审查再改", "改之前先检查", "先确认能不能改", "实现前 review", or asks Codex to review whether a planned local code change is grounded in current facts, inside scope, and safe to implement before editing.
---

# 鱼巢实施前审查

## Core Meaning

Treat this skill as the brake pedal before editing:

```text
当前用户目标
-> 当前本地代码事实
-> 当前规范 / 详细设计 / 计划索引
-> 是否允许实现
-> 先改什么 / 不该改什么 / 还缺什么
```

This skill exists to prevent:

```text
把设计当实现
把旧对话当本地事实
跳过 S0 只读扫描
扩大改动范围
把 dirty worktree 混进当前切片
```

## Hard Preflight

1. Confirm the workspace is `D:\鱼巢`.
2. Read `AGENTS.md` with limited-load discipline.
3. Read at least:

```text
docs/00_文档总索引.md
规范/规范目录.md
规范/0050_项目通用结构与禁止性规则总纲_20260701.md
计划/计划索引.md
```

4. Read the relevant current detailed design / implementation plan if they exist:

```text
规范/详细设计/
规范/详细设计/待完成/
计划/*.md
```

5. If there is no reliable current local fact scan yet, first run a local fact scan or produce the minimum equivalent S0 evidence before concluding.

## Review Questions

Answer these before any edit recommendation:

1. Is the target grounded in current local code facts?
2. Is there a current spec / detailed design / plan authority for the change?
3. Is the requested action still inside the current slice boundary?
4. Will the change collide with current dirty worktree state?
5. Is there an existing function / structure / plan that should be reused first?
6. Does the change require a prior `S0` fact scan?

If any answer is unclear, do not permit implementation by guesswork.

## Output States

Conclude in one of these states:

```text
可执行：当前本地事实、规范、计划和边界都清楚，可以进入实现。
缺事实：本地真实入口、调用点、承载结构或 dirty state 影响未确认。
缺规范：没有足够权威规则支持当前改法。
缺详细设计：已有规范，但没有对应详细设计或实现映射计划。
越界风险：会扩大到当前切片以外的目录、结构或业务口径。
脏树风险：当前未提交改动会影响判断或容易混入本次切片。
```

## Required Output

Return compactly with:

```text
审查对象：
当前本地事实是否已够：
权威依据：
允许修改范围：
禁止修改范围：
现有可复用入口：
主要风险：
审查结论：
进入实现前的下一步：
```

When the conclusion is not `可执行`, the last line must be the smallest blocking step, for example:

```text
先做 S0 只读扫描
先回读 D3 详细设计
先确认 dirty worktree 中某文件是否属于本切片
先补计划索引登记
```

## Boundaries

- Do not implement code during this skill.
- Do not silently fix missing design by inventing fields, structures, or text logic.
- Do not use `说明书/` as implementation authority.
- Do not let control panel / SQL display / logs become machine logic.
- Do not permit edits that mix unrelated dirty files into the slice.
- Do not say `可以改` unless you can name the authority source, local fact source, and allowed file boundary.
