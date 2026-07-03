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

## 2026-07-03：判断型枚举不得冒充外设一阶事实

背景：

外设感知详细设计中原先存在 `材料质量状态` 等状态词，容易被实现成外设侧直接输出的 I64 枚举。用户指出：枚举型特征值和布尔型没有天然本质区别，只要它是根据其它材料判断出来的状态，就不能伪装成外设直接取得的一阶事实。

决定：

将外设感知共同边界和观察详细设计修订为三层：

```text
原始材料 / 一阶测量值 / 可追溯状态
复合特征判断
内部治理或方法收束状态
```

外设侧只直接提供原始材料句柄、设备状态、时间戳、帧序号、尺寸、深度 / 彩色 / 空间点材料句柄、有效 / 无效像素数量、缺失比例、噪声 / 曝光 / 模糊 / 对齐评分、坐标变换版本、外设错误码和低层诊断候选。

`材料质量状态`、`归属状态`、`稳定复现状态`、`提交候选判定状态`、`遮挡解释状态` 等判断型枚举，必须由复合特征、内部治理节点或方法执行后的阶段性结论收束产生，并保留依赖特征族、收束方法、写入方、读取方、失败分流和可回查证据。

原因：

- 4120 复合特征规则已经要求判断型 bool 展开子特征、判定规则、写入方和来源动态。
- 4140 枚举型实例特征值合法来源规范要求实例枚举值必须有合法生产者和证据。
- 6310 观察质量规范已经明确质量诊断摘要只能作为输入证据，不能直接把状态写成 `可用`。
- 仅把 bool 改成 I64 枚举会保留同一个问题：状态结论没有材料来路。

不采用的方案：

- 不把 `材料质量状态 = 可用 / 低质量 / 不可用 / 部分可用 / 需复验` 写成外设直接输出的一阶普通特征。
- 不为每个枚举值单独新建结构，避免结构体膨胀。
- 不让单个质量评分、外设错误码、日志文本或报告名称直接推出正式材料质量状态。

影响：

- 后续实现外设接口时，应先输出一阶材料值、测量值、评分和低层候选。
- 后续实现观察或中间层质量收束方法时，需要读取外设一阶材料并结合当前任务阈值、最大允许观察年龄、目标特征类型、坐标基准要求和可回查要求。
- 扫描、识别、跟踪读取“材料质量不足”时，只能读取质量收束状态或其可回查证据，不能直接读取外设报告文本下结论。

后续验证方式：

- 代码实现前生成实现映射计划，列出外设一阶材料字段、质量收束入口、写入方和读取方。
- 检查 `外设观察报告队列.ixx`、`自我动作实现.外设模块.ixx`、`任务模块.管理工作线程.ixx` 和扫描入口，确认不存在外设直接写判断型状态参与任务结算。
- 回归时证明材料质量状态的输入特征族、收束方法、状态写入和失败分流均可追溯。

## YYYY-MM-DD：决策标题

背景：

决定：

原因：

不采用的方案：

影响：

后续验证方式：
