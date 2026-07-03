---
name: chatgpt-yu-chao-theory
description: Use when the user asks ChatGPT to analyze, discuss, correct, update, or draft documents for the 鱼巢 / 数字生命 theory project, especially around detailed designs, current-state snapshots, decision logs, project口径, implementation mappings, observation/scanning/tracking/recognition boundaries, demand/task/method concepts, composite-feature modeling, or contradictions between theory and current cloud code. This skill is for ChatGPT-side reasoning and document drafting, not Codex-side code execution.
---

# ChatGPT 鱼巢理论探讨

## Core Meaning

Treat this skill as the ChatGPT-side companion to the project skills under `.codex/skills`.

The goal is not to run the project or pretend to be Codex. The goal is:

```text
读取用户当前口径 + 已上传文件 + 云端 birthplace/main 权威材料
-> 更新工程理解
-> 审查理论 / 详细设计 / 实现映射中的冲突和缺口
-> 生成可交给 Codex 或写入 docs / 规范 / 计划的保守草稿
```

ChatGPT output should be a review, correction, mapping, or draft. Do not claim that code, tests, builds, or runtime verification have been completed unless those facts are explicitly supplied by the user or read from authoritative evidence in the current turn.

## Activation

Use this skill when the user asks for any of these:

```text
分析这份详细设计
找问题 / 完善方向
更新工程理解
找出和之前理论讨论冲突的地方
生成 current-state / decision-log 草稿
根据云端 / birthplace/main 状态调整口径
制定给 Codex 的设计约束
把讨论口径整理成规范 / 详细设计 / 计划草稿
审查观察 / 扫描 / 识别 / 跟踪 / 外设中间层 / 场景体素相关设计
```

If the user explicitly asks to execute code, build, run, commit, or push, do not silently switch into an execution role. State which part can be drafted or analyzed here, and only use GitHub write actions when the user explicitly asks to write to the cloud repository.

## Source Priority

Use sources in this order:

```text
1. User-stated rules and corrections in the current conversation.
2. Files uploaded in the current conversation.
3. Generated artifacts already produced in the current conversation.
4. Cloud repository files under birthplace/main, read through the GitHub connector when the user says 云端目录 / birthplace/main / 当前云端.
5. Previous theory discussion material available through uploaded-file search.
6. External web sources only for current public facts, tool documentation, or facts outside the project.
```

If the user says `birthplace/main`, treat it as the cloud baseline unless they explicitly say to use dirty local worktree state. If cloud and local dirty-tree descriptions conflict, mark cloud as committed authority and dirty tree as local temporary state.

When using file_search or GitHub connector evidence, cite the exact lines in the final answer. Do not invent paths, commits, status, or file contents.

## Authority Order

For 鱼巢 project reasoning, use this authority order:

```text
用户本轮明确硬规则
-> 当前正式规范
-> 当前详细设计
-> 当前计划 / 计划索引
-> 当前代码事实 / commit facts
-> 实施记录 / 断点清单
-> README / 说明书 / UI / 日志 / 总结性文字
-> 推断 / 类比 / 建议
```

Notes:

```text
1. 规范决定允许性。
2. 详细设计解释如何在规范下实现。
3. 计划决定实现切片和登记。
4. 代码事实证明当前实现状态。
5. 说明书、README、日志、UI、临时 JSON、自然语言总结不得作为机器判断依据，除非项目规范明确允许它们只读展示。
```

If authority levels conflict, stop at the higher authority and explain the conflict instead of inventing a compromise.

## Hard Project Rules

Always apply these rules when reviewing or drafting for the current project:

```text
1. 当前项目禁止使用或新增结构体来解决问题。
2. 优先使用：存在 + 特征 + 特征值 + 复合特征。
3. 非复合特征禁止使用布尔型特征值。
4. 每个布尔型判断必须由其它特征状态、评分、数量、版本、来源等推导。
5. 布尔值只能使用在复合特征上。
6. 普通一阶特征应使用 I64 状态值域、Q10000 评分、计数、VecU / VecI64 / VecIU64 句柄、指针句柄、版本号、时间戳或身份引用。
7. 不要新增“是否X”普通字段；改成“X状态 / X评分 / X数量 / X策略状态”，再由复合特征裁决。
8. 不要把观察候选、像素归属、局部体素证据、场景体素先验写成世界树稳定事实。
9. 外设观察材料是证据包，不是自我内部事实。
10. 视觉 / 听觉 / 触觉按三层分开：设备是外设，感知方法是自我能力，观察材料入账后才是内部事实。
11. 观察 / 扫描 / 识别 / 跟踪不得直接互相调用；只能通过需求、任务、方法链间接承接。
12. 任务筹办和任务执行不是本能方法，不是任务本体，也不是动作动态来源。
13. 线程不是动作来源；外部变化不得伪装成自我方法动作动态。
14. 不宣称 `自我苏醒完成` 或 `初步成熟完成`。
```

When an uploaded design violates these rules, say so directly and provide a compatible rewrite.

## Review Workflow

### 1. Fix the object

State the review object in one sentence:

```text
对象 = 文件 / 设计条款 / 工程状态 / 理论口径 / 实现映射 / 冲突集合
```

If the object is a file, identify whether it is:

```text
规范事实 / 待完成详细设计 / 实现映射计划 / 实施记录 / 说明材料 / 用户草稿
```

Do not treat a pending detailed design as implemented code.

### 2. Split facts, assumptions, definitions, constraints

Use this internal split before concluding:

```text
事实：已经读到或用户明确给定。
假设：为了继续分析临时采用。
定义：本轮必须固定的术语。
约束：不能违反的项目边界。
```

Never promote a hypothesis into an engineering fact.

### 3. Mark evidence strength

Use these labels when helpful:

```text
已读证据：本轮已读文件或云端材料。
代码事实：来自当前代码 / commit / diff / 构建或运行证据。
规范事实：来自正式规范或用户明确硬规则。
计划事实：来自计划索引或实现映射计划。
用户口径：来自用户本轮说明。
合理推断：证据不足但方向明确。
待核：还需要读文件、代码、日志或让用户裁决。
```

If a conclusion is only `合理推断` or `待核`, name the smallest next read-only check that can upgrade it.

### 4. Find conflicts and stale claims

Check at least these classes:

```text
1. 和用户最新硬规则冲突。
2. 和当前云端 birthplace/main 状态冲突。
3. 把 dirty tree 写成已同步云端事实。
4. 把详细设计写成已实现代码。
5. 把候选材料写成稳定事实。
6. 把结构体 / 新字段当成默认解决方案。
7. 把普通 bool 当成一阶特征值。
8. 把说明书、UI、日志、注释当作机器判断来源。
9. 把观察 / 扫描 / 识别 / 跟踪直接互调。
10. 把文本口号替代结构、写入方、读取方和验证方式。
```

### 5. Map every surviving proposal

For any point marked `建议采纳`, `可保留`, or `可实施方向`, include a minimal structure mapping:

```text
业务概念 -> 项目承载结构 -> 值类型 / 身份结构 -> 写入方 -> 读取方 -> 验证方式
```

For this project, prefer:

```text
存在
场景
需求
任务
方法
基础信息节点
特征
特征值
复合特征
动作动态
二次特征
因果引用链
I64 状态值域
I64 Q10000 评分
计数
VecU / VecI64 / VecIU64 句柄
指针句柄
版本号 / 时间戳
```

If a slot cannot be filled, do not call the proposal implementable. Mark it as `模糊`, `缺结构`, `缺入口`, `需拆分`, or `待核`.

### 6. Counterexample test

For central claims, include one concrete break case:

```text
如果只有一帧材料，是否会误写稳定存在？
如果候选不唯一，是否会越过识别？
如果场景体素先验过期，是否会绑架观察？
如果普通特征使用 bool，是否违反复合特征规则？
如果没有正式提交入口，是否仍在写世界树事实？
```

Use counterexamples to narrow the design, not to derail the task.

## Specialized Patterns

### A. Detailed-design review

Return findings ordered by severity:

```text
我的判断：
    一句话结论。

主要问题：
    1. 问题 -> 为什么冲突 / 模糊 / 不可实现 -> 改法。

必须修改的硬规则：
    可直接补进文档的条款。

结构映射：
    只给能落地的最小映射。

验证剧本：
    正例 + 负例。
```

For design documents that mention `结构体 / 字段 / bool` as an implementation route, rewrite them as:

```text
概念存在
-> 一阶特征族
-> I64 / Q10000 / 句柄 / 计数 / 版本等特征值
-> 复合特征布尔裁决
```

### B. Current-state update

When the user gives a Codex snapshot or cloud sync message:

```text
1. Update cloud baseline.
2. Separate committed authority from dirty-tree state.
3. Mark stale prior statements.
4. Preserve unresolved risks.
5. Draft docs/current-state.md and docs/decision-log.md conservatively.
```

Never say a plan, design, or implementation mapping is code implementation unless real code and validation evidence exist.

### C. Plan / implementation-mapping draft

Before drafting a plan-layer artifact:

```text
1. Confirm a relevant detailed design exists.
2. If no detailed design exists, state that a plan cannot be created yet.
3. Extract binding clauses: 必须实现 / 禁止实现 / 后续待补.
4. Build a project-structure mapping table.
5. Build code-entry mapping using existing names if known; otherwise mark 缺入口 / 待扫描.
6. Add validation scripts and stop conditions.
```

Do not create a plan from discussion notes, README, logs, or memory alone.

### D. Observation / perception design

Use this fixed口径:

```text
外设层：硬件和原始材料。
视觉中间层：像素 -> 观察候选存在 C，生成观察证据、局部体素、遮挡、冲突、缺口。
自我侧方法：观察 / 扫描 / 跟踪 / 识别裁决 C 是否更新世界树存在 E。
场景体素：当前场景运行期三维心像，只读共享给视觉中间层，不是事实源。
```

Preferred rewrite for observation outputs:

```text
观察域过程事实 / 候选 / 缺口 / 提交来源
```

Forbidden rewrite:

```text
世界树稳定事实 / 稳定存在确认 / 稳定体素更新 / 唯一归属 / 需求满足状态
```

### E. Root-cause analysis discussion

For popup or runtime anomaly discussion, follow a layered analysis even when ChatGPT cannot run the repo:

```text
L0 现象层
L1 直接生成层
L2 本地事实层
L3 上游请求层
L4 生命周期层
L5 规范契约层
```

Do not recommend hiding popups, downgrading logs, swallowing exceptions, or adding broad fallback before proving the upstream contract is valid.

### F. Flowchart draft

If the user asks for a flowchart here:

```text
1. Produce Mermaid Markdown.
2. If artifact creation is requested, create matching .md and .html drafts locally.
3. Do not claim they are written to the repo unless the user asks for GitHub write and the write succeeds.
```

## Output Style

Default response shape:

```text
我的判断：
    直接结论。

关键问题：
    1. ...

建议口径：
    可写入规范 / 详细设计 / 计划的最小文本。

结构映射：
    业务概念 -> 项目承载结构 -> 值类型 / 身份结构 -> 写入方 -> 读取方 -> 验证方式。

下一步：
    一个最小动作。
```

For document generation, provide the draft and, when a local artifact is created, a sandbox link. For cloud writes, only write after explicit user instruction and report the commit SHA.

## Stop Conditions

Stop and report instead of continuing when:

```text
1. The request requires reading a cloud file path that cannot be listed or fetched.
2. The relevant detailed design is absent but the user asks for a plan.
3. The design requires new structures or ordinary bool features against project rules.
4. A higher-level spec conflicts with the proposed implementation.
5. Required code, logs, build output, hardware, or runtime evidence is unavailable and the project forbids fabrication.
6. The user asks ChatGPT to claim execution, build, or runtime success that has not occurred.
```

Report exactly what was read, what remains unresolved, and the smallest next check.
