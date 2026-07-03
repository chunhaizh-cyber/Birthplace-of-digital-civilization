---
name: digital-life-reasoning
description: Use when the user says "推理", "逻辑推理", "请推理", "梳理逻辑", or asks to reason about digital life theory, definitions, causal chains, implementation schemes, safety/service boundaries, self/need/task/method concepts, or contradictions in the `D:\数字生命思路` project. This skill fixes a reusable workflow for turning discussion into consistent premises, definitions, inferences, counterexamples, conclusions, and a Markdown document saved under the current project directory once the口径 is aligned.
---

# Digital Life Reasoning

Use this skill to reason about digital life theory and implementation plans in `D:\数字生命思路`.

## Core Rule

Treat the project goal as:

```text
探讨和记录数字生命的理论和实施方案。
```

After the discussion口径 is aligned, create or update a Markdown document under the current project directory. Chat is for clarification and alignment; durable conclusions belong in files.

## Source Boundaries

Use sources in this priority order:

```text
1. User-provided statements in the current conversation.
2. Existing files under D:\数字生命思路.
3. Searchable read-only material under D:\鱼巢.
4. Searchable read-only material under D:\人工智能之数字生命文稿.
5. External web sources only when current public facts are required.
```

When citing external project material, keep the source path and file name visible. Do not modify `D:\鱼巢` or `D:\人工智能之数字生命文稿` unless the user explicitly asks.

## Reasoning Workflow

### 1. Define the Reasoning Object

State what is being reasoned about in one sentence:

```text
对象 = 概念 / 机制 / 因果链 / 实施方案 / 边界 / 反例 / 评价标准
```

If the object is unclear, ask one concise clarification question or make the smallest explicit assumption.

### 2. Separate Facts, Assumptions, and Terms

Use this split before inference:

```text
事实：来自当前目录、允许引用目录、代码/规范/文稿证据或用户明确给定内容。
假设：为了继续推理临时采用、尚未验证的前提。
定义：本轮需要固定含义的术语。
约束：不能违反的边界、规则、场景或项目目标。
```

Do not let a hypothesis enter the conclusion as if it were verified.

### 3. Build the Inference Chain

Prefer this form:

```text
前提 P1
前提 P2
约束 C1
=> 中间结论 M1
=> 可验证后果 V1
=> 结论 K1
```

For causal reasoning, make the direction explicit:

```text
触发事实 -> 中间状态 -> 动作 / 变化 -> 结果状态 -> 回写 / 评价
```

For implementation reasoning, map theory to structure:

```text
概念 -> 信息结构 -> 流程 -> 验证证据 -> 文档落点
```

### 4. Check Contradictions

Before accepting a conclusion, check:

```text
1. 是否和用户刚给定的口径冲突。
2. 是否和当前目录已有文档冲突。
3. 是否把工程证据写成理论证明。
4. 是否把自然语言描述直接当成机器定义。
5. 是否把假设、类比、目标或愿景写成事实。
6. 是否存在反例或边界场景。
```

If a contradiction is found, stop and present the conflict with source paths or exact user-stated boundaries.

### 5. Use Counterexamples

For important concepts, test at least one negative case:

```text
如果 X 缺失，结论是否仍成立？
如果只有自然语言描述，没有事实结构，是否仍可执行？
如果工程样本只证明局部能力，是否被扩大成总证明？
```

Use counterexamples to narrow definitions, not to derail the whole task.

### 6. Align the口径

Summarize the candidate口径 in short form:

```text
本轮口径：
1. ...
2. ...
3. ...

仍待确认：
1. ...
```

If the user accepts or the discussion clearly converges, treat the口径 as aligned and write the document.

### 7. Write the Document

Save the aligned result under `D:\数字生命思路`. Choose a focused Markdown filename:

```text
概念_<主题>_YYYYMMDD.md
方案_<主题>_YYYYMMDD.md
推理_<主题>_YYYYMMDD.md
```

Use this document shape unless the user specifies another:

```text
# 标题

## 1. 目标
## 2. 当前口径
## 3. 前提与依据
## 4. 推理链
## 5. 反例与边界
## 6. 结论
## 7. 待核验问题
## 8. 下一步
```

Keep durable text free of internal shorthand and temporary search noise.

## Output Style

During alignment, be concise:

```text
结论：
依据：
冲突 / 待定：
建议落点：
```

After writing a document, reply with:

```text
已生成 / 已更新：<path>
核心口径：...
未完成项：...
```
