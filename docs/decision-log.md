# Decision Log

## 2026-07-03：建立工程状态同步记录机制

背景：

ChatGPT「理论探讨」项目和 Codex 工程执行项目之间需要共享当前工程事实。仅靠旧对话记忆容易把已经变化的代码、规范、计划或工作区状态误当成当前事实。

决定：

采用三文件分工：

```text
AGENTS.md 管规则
docs/current-state.md 管现实
docs/decision-log.md 管为什么
```

每次 Codex 任务结束前，应更新 `docs/current-state.md`；如果本次产生重要技术选择，应同步更新 `docs/decision-log.md`。最终回复中提供一份可复制给 ChatGPT「理论探讨」项目的同步摘要。

原因：

- `AGENTS.md` 适合承载工作规则、必读文件和收尾要求。
- `docs/current-state.md` 适合承载当前目标、分支、已完成、未完成、测试状态、风险和下一步。
- `docs/decision-log.md` 适合承载重要设计选择的背景、决定、原因、不采用方案、影响和后续验证方式。
- 将现实状态写入仓库文档，可以减少理论讨论项目沿用旧对话假设的风险。

不采用的方案：

- 不把当前工程状态只放在对话记忆中，因为对话可能压缩、迁移或滞后。
- 不把状态写入 README，因为 README 是对外项目入口，不适合承载频繁变更的执行状态。
- 不把状态写入说明书，因为说明书是人读材料和代码反向说明，不作为实现权威。
- 不把重要技术选择混入 `current-state.md`，避免现实快照和决策理由混在一起。

影响：

- 后续 Codex 任务需要在收尾时检查并更新 `docs/current-state.md`。
- 重要技术选择需要追加到 `docs/decision-log.md`。
- ChatGPT「理论探讨」项目应优先读取这两个文件来获取当前工程现实和决策原因，而不是沿用旧讨论想象。

后续验证方式：

- 每次任务结束检查 `docs/current-state.md` 的最后更新时间、当前目标、测试状态和已知问题是否仍准确。
- 产生重要设计选择时，检查 `docs/decision-log.md` 是否新增对应条目。
- 需要同步给 ChatGPT「理论探讨」项目时，直接复制最终同步摘要，并以仓库文档为事实来源。

## 2026-07-03：自定义 skill 实体进入项目目录

背景：

`C:\Users\zhchh\.codex\skills` 下的自定义 skill 是本地文件，不在 `D:\鱼巢` 仓库中；因此普通 `git push birthplace main` 不会把 `yu-chao-plan` 等自定义 skill 同步到云端。

决定：

将本项目相关自定义 skill 的实体目录移动到：

```text
D:\鱼巢\.codex\skills\
```

同时在原 Codex 默认路径下保留同名目录联接：

```text
C:\Users\zhchh\.codex\skills\<skill> -> D:\鱼巢\.codex\skills\<skill>
```

移动范围只包括自定义 skill：

```text
digital-life-reasoning
discuss
yu-chao-execute
yu-chao-flowchart
yu-chao-plan
yu-chao-root-cause
```

不移动 `.system` 和 `codex-primary-runtime`。

原因：

- 项目内实体目录可以被 Git 跟踪并同步到云端。
- 保留全局目录联接可以兼容 Codex 当前默认 skill 加载路径。
- 不移动系统 skill 和运行时目录，避免把 Codex 自带或缓存内容混入项目仓库。

不采用的方案：

- 不只复制一份到项目目录，因为容易出现全局副本和项目副本分叉。
- 不直接整体移动 `C:\Users\zhchh\.codex\skills`，因为会影响 `.system`、运行时目录和 Codex 默认发现路径。
- 不把 skill 塞进 `docs/`，因为 skill 本身是可执行协作规则，不是普通文档说明。

影响：

- 后续修改自定义 skill 时，应修改 `.codex/skills/` 中的实体文件。
- 克隆到新机器后，仓库能带上自定义 skill 内容，但仍需要建立全局目录联接，Codex 才能从默认路径发现这些 skill。
- 若目录联接失效，Codex 可能无法加载对应自定义 skill。

后续验证方式：

- `git status -- .codex/skills` 能看到自定义 skill 文件。
- `C:\Users\zhchh\.codex\skills\<skill>\SKILL.md` 可读，且目录属性包含 `ReparsePoint`。
- 后续云端同步后，在远端仓库可看到 `.codex/skills/` 下的自定义 skill。

## 2026-07-03：新增 ChatGPT 侧鱼巢理论探讨 skill

背景：

ChatGPT「理论探讨」项目需要一个稳定入口，用于读取云端仓库状态、审查鱼巢理论口径、详细设计、实现映射计划和当前工程状态，避免沿用旧讨论或把未实现设计当成代码事实。

决定：

新增项目内自定义 skill：

```text
.codex/skills/chatgpt-yu-chao-theory
```

并建立全局目录联接：

```text
C:\Users\zhchh\.codex\skills\chatgpt-yu-chao-theory -> D:\鱼巢\.codex\skills\chatgpt-yu-chao-theory
```

原因：

- 该 skill 明确区分 ChatGPT 侧理论审查和 Codex 侧代码执行。
- 它要求以云端 `birthplace/main`、当前规范、详细设计、计划和代码事实为依据，降低旧对话漂移风险。
- 放入 `.codex/skills/` 后可以随仓库同步，同时通过全局联接兼容默认 skill 加载路径。

不采用的方案：

- 不把它并入 `discuss`，因为 `discuss` 是通用讨论入口，而该 skill 专门面向 ChatGPT 侧理论项目和云端状态审查。
- 不让该 skill 承担代码执行、构建、提交或推送职责，避免混淆 ChatGPT 讨论和 Codex 工程执行边界。

影响：

- ChatGPT 侧讨论可用 `$chatgpt-yu-chao-theory` 作为固定入口。
- 后续修改该 skill 时，应直接修改 `.codex/skills/chatgpt-yu-chao-theory/` 下实体文件。
- 克隆到新机器后仍需重建全局目录联接，Codex 才能从默认路径发现该 skill。

后续验证方式：

- `SKILL.md` frontmatter 中 `name` 与目录名一致，且存在 `description`。
- `agents/openai.yaml` 的 `short_description` 保持短描述，`default_prompt` 明确包含 `$chatgpt-yu-chao-theory`。
- 全局路径 `C:\Users\zhchh\.codex\skills\chatgpt-yu-chao-theory\SKILL.md` 可读，且父目录为 `ReparsePoint`。

## YYYY-MM-DD：决策标题

背景：

决定：

原因：

不采用的方案：

影响：

后续验证方式：
