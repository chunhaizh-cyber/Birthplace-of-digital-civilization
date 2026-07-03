---
name: yu-chao-plan
description: Use in D:\鱼巢 when the user says "制定计划", "生成计划", "拆分计划", "修订计划", "更新计划索引", "把需求落入计划", "生成实现计划", "生成映射计划", "代码落地契约", or asks Codex to turn an existing detailed design into a plan-layer implementation mapping. Before planning, verify that a relevant detailed design exists; if not, refuse to create the plan and state that no detailed design exists yet.
---

# 鱼巢制定计划

## Core Meaning

Treat "制定计划" as a plan-layer bridge from an existing detailed design to executable code work.

Do not use this skill to brainstorm rules, write code, or bypass missing design. Its main product is:

```text
详细设计
-> 代码落地契约 / 实现映射计划
-> 计划索引登记
-> 后续执行入口
```

## Hard Preflight

1. Confirm the workspace is `D:\鱼巢`.
2. Read `AGENTS.md` if present, using its limited-read rules.
3. Read at least:

```text
docs/00_文档总索引.md
规范/规范目录.md
规范/0050_项目通用结构与禁止性规则总纲_20260701.md
计划/计划索引.md
```

4. Find the relevant detailed design under:

```text
规范/详细设计/
规范/详细设计/待完成/
```

5. If no relevant detailed design exists, stop and say:

```text
还没有对应详细设计，无法制定计划。
需要先根据规范生成详细设计，再制定代码落地契约 / 实现映射计划。
```

Do not create a plan from discussion notes, README, 说明书, logs, implementation records, flowcharts, or memory alone.

## Authority Rules

- 规范 decides whether a design is allowed.
- 详细设计 explains how to implement under the spec.
- 计划 decides which implementation slice to do.
- 代码落地契约 / 实现映射计划 locks detailed design to code entries.
- 说明书 is reverse-read from code and must not be used as implementation authority.
- If spec, detailed design, plan, and code conflict, stop at the higher authority boundary instead of inventing a compromise.

## Discussion Gate

During planning, if any conflict, unclear concept, missing boundary, uncertain ownership, unverifiable mapping, or implementation ambiguity appears, stop the planning flow and enter discussion mode.

Do not continue by filling the plan with assumptions. First clarify the issue with the user as a design discussion, then revise and complete the detailed design. Only after the detailed design is consistent and complete may the plan continue.

This applies especially when:

```text
规范与详细设计冲突
详细设计内部前后不一致
业务概念无法映射到项目结构
读写方不明确
代码入口不确定且无法通过限载扫描确认
验证证据无法定义
需要新增字段 / 新结构但没有规范依据
```

## Output Artifact

For `D:\鱼巢`, write the plan-layer artifact under `计划/` unless the user gives a stricter valid path.

Recommended filename:

```text
计划/YYYYMMDD_主题实现映射计划_v0.1.md
```

The artifact should state that it is not code implementation and does not prove the design is complete.

## Required Sections

A valid plan must include these sections.

### 1. 设计条款清单

Separate:

```text
必须实现
禁止实现
后续待补
```

State which detailed-design clauses are binding. Do not let implementation decide these freely later.

### 2. 项目结构映射表

Use a table:

```text
业务概念 -> 现有项目结构 -> 值类型 / 身份结构 -> 写入方 -> 读取方 -> 不新增字段的表达方式 -> 验证方式
```

If a concept cannot be represented by existing project structures, mark it as `缺结构 / 缺函数 / 缺入口`; do not create a temporary text field as a workaround.

### 3. 代码入口映射

Map:

```text
设计步骤 -> 现有类 / 函数 / 待新增函数 -> 调用方 -> 返回结构 -> 失败分流
```

Before finalizing code names, require a later implementation slice to scan:

```text
说明书/函数功能说明文档.md
相关 .h / .cpp / .ixx
```

If no entry exists, write `缺函数` or `缺入口`. Do not write a temporary bypass.

### 4. 流程状态表

Include:

```text
输入条件
判断条件
输出状态
失败分流
谁继续承接
```

State whether the continuer is self thread, task management, method layer, demand submission entry, world layer, or display-only layer.

### 5. 反走样清单

Explicitly forbid common drift:

```text
新增字段
日志判断
说明文本判断
控制面板 / SQL 显示反向参与机器判断
临时 JSON
硬编码领域词
父子关系永久类型字段
任务筹办 / 任务执行变成本能方法
外部变化伪装成自我方法动作动态
```

Any hit means the later implementation slice cannot pass review.

### 6. 验证剧本

For every design conclusion, specify evidence:

```text
输入入口
结构写入
状态变化
调用路径
回归命令
失败时应出现的分流
```

Do not accept logs or display text as the only machine evidence.

### 7. 最小切片计划

Prefer this order unless the detailed design proves a different sequence:

```text
S0 代码事实扫描
S1 只读视图 / 只读入口
S2 接调度或父级结算
S3 接截止 / 取消活动承接 / 转挂
S4 显示只读投影
S5 验证和实施记录
```

Each slice must list allowed files, forbidden files, validation, and parent-plan writeback.

## Demand Tree Parent-Child Role View Pattern

When the topic is `需求树父子关系角色视图`, the plan must preserve these fixed points:

```text
设计条款：
    不新增子需求类型字段。
    不新增父子关系永久类型字段。
    角色是运行期只读视图，不是需求本体属性。

代码入口：
    读取父子关系角色视图(...)
    读取父需求未满足必须子需求集合(...)
    读取父需求非必须活动子需求集合(...)
    读取父需求可选路径满足状态(...)

结构映射：
    角色 = 运行期只读视图
    依据 = 需求目标 + 特征定义 + 方法条件 + 来源动态 + 活动承接状态

验证：
    必须子需求阻塞父需求。
    可选路径一条满足后其他路径截止或取消活动承接。
    非必须子需求有更高优先级任务时放弃当前活动承接。
```

## Plan Index

If a plan artifact is created or materially revised, update `计划/计划索引.md` only when project rules allow it.

Do not silently retarget the current active item. If the plan is a bridge for future implementation, register it as `阻塞承接` or another status justified by the current index rules.

## Verification

After creating or revising plan/spec/design documents, run:

```powershell
python .\tools\check_specs.py
```

For document-only planning, do not run C++ build unless the user asks or the plan change is coupled to code.

## Stop Conditions

Stop and report instead of creating a plan when:

- no relevant detailed design exists;
- the detailed design conflicts with a current formal spec;
- planning exposes a conflict, unclear concept, uncertain mapping, or unresolved implementation boundary; enter discussion mode, revise the detailed design, then resume planning;
- the target would require inventing new machine fields or structures without spec authority;
- required implementation ownership is unknown and cannot be discovered with limited reads;
- the user is actually asking for discussion, not plan creation.
