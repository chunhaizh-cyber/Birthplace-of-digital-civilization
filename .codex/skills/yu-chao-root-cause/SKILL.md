---
name: yu-chao-root-cause
description: Trace popup, Debug Error, abort, dump, and runtime-state root causes in D:\鱼巢 and similar C++ codebases. Use when the user asks to 追根因, 追查弹窗报错原因, 定位弹窗根因, 分析弹窗错误, 逻辑错误弹窗, MessageBox/项目弹窗错误提示, procdump/cdb/dump stack capture, crash-adjacent popup diagnosis, or any task where Codex must explain and fix why an error dialog appears instead of hiding it.
---

# 鱼巢根因追踪

## Core Meaning

Treat every error popup, debug error, abnormal tree growth, or runtime state anomaly as a layered root-cause investigation:

```text
弹窗标题 / 正文 / 出现路径
-> 弹窗调用点
-> 同窗口错误日志和上游事实
-> 触发条件与数据来源
-> 第一个违反规范、数据契约或生命周期预期的点
-> 最小修复
-> 复跑确认弹窗消失且同类错误日志消失
```

Do not complete by hiding the dialog, downgrading logs, adding a silent fallback, or only adding duplicate suppression. The result must explain why the popup was correct to complain, and which upstream state or contract was wrong.

Default assumption: an error popup is a downstream alarm caused by earlier logic damage. Treat "the popup should not exist" as an extreme-case conclusion only after proving the checked condition is a normal, recoverable business state and the project contract allows continued processing.

## Start Context

1. Confirm the workspace. If it is `D:\鱼巢`, read `AGENTS.md`, `规范/禁止性规范20260611.md`, `规范/规范目录.md`, and `计划/计划索引.md` before code changes.
2. If the issue touches internal loop, demand tree, task management, method selection, method execution, causal feedback, peripheral observation, remote continuation, or context recovery, read the relevant specs named by `AGENTS.md` and the plan index.
3. After compaction, network interruption, or a vague "继续", restore from `AGENTS.md`, specs, `计划/计划索引.md`, active `计划/追根因_*.md` records, recent `实施记录/*Codex断点清单.md` only when needed, and `git status --short --untracked-files=no`.
4. Use scoped searches first. Prefer `rg`, `rg --files`, path-limited `git status`, path-limited diffs, and short file reads. Avoid generated folders, `x64`, `Debug`, `.git`, and large artifacts unless they are the evidence surface. Logs are allowed when the popup itself is the evidence surface.

## Popup First Pass

1. Capture the observable:
   - exact popup title
   - exact popup body or screenshot text
   - command or UI action that produced it
   - whether it repeats every run or only once per process
2. Find the popup log window. In `D:\鱼巢`, start with:

```powershell
rg -n "弹窗错误提示|弹窗错误提示已返回|\\[ERR\\]|\\[FTL\\]|逻辑错误" 日志
```

3. Find the code entrypoint. In `D:\鱼巢`, start with:

```powershell
rg -n "项目弹窗错误提示|MessageBoxW|AfxMessageBox|弹窗|逻辑错误" -S --glob '!x64/**' --glob '!Debug/**' --glob '!.git/**'
```

4. Prefer the project wrapper path first:
   - `日志接入.h` declares `项目弹窗错误提示`.
   - `日志模块.ixx` records `弹窗错误提示 | 标题=... | 文本=...` before `MessageBoxW`, and `弹窗错误提示已返回 | 标题=...` after the user closes it.
   - Direct `MessageBoxW` sites are still valid, but check whether they should route through project logging.
5. When multiple popup sites share a title, match by unique body text, surrounding log keys, or nearby timestamp rather than guessing.

## Evidence Rules

- Write the expected behavior before running a validation command when the bug is behavioral.
- Use real build output, real runtime logs, real state snapshots, action dynamics, or existing error logs. Do not use mock tests, fake entries, fake samples, or fabricated action dynamics.
- For popup errors, crashes, and non-normal logic errors, treat the error as a root-cause task. Do not complete by hiding the popup, downgrading the log, swallowing the exception, adding one-shot suppression, or converting it to a business gap unless the governing spec says it is a business gap.
- Assume the previous logic path is wrong until evidence proves otherwise. Only relax or remove a popup guard when the traced input is valid, the state is expected, downstream processing has a defined contract, and continuing will not hide a lifecycle or causality break.
- If the same symptom can be caused by multiple layers, prove which layer first diverges. Do not jump from final symptom to final fix without an intermediate evidence chain.
- If the evidence is partial, label it as partial and continue to the next concrete probe instead of calling it resolved.

## Popup Root-Cause Loop

1. State the popup in one sentence: `标题=... | 正文锚点=... | 触发路径=...`.
2. Locate the exact popup call. Quote the function/file/line in the working notes, not long code blocks.
3. Read the guard condition that decides to pop up:
   - what invariant was checked
   - what actual value failed it
   - whether the code classifies it as logic error, missing parameter, missing bridge output, null pointer, stale state, wrong method, or another category
4. Identify the authoritative contract: spec, plan, type invariant, function comment, call protocol, log format, or runtime expectation.
5. Trace backward from the failing guard:
   - immediate caller and input package
   - producer / writer of the bad value
   - data owner or lifecycle owner
   - first point where expected and actual state diverge
6. Trace forward from the likely source:
   - input assumptions
   - state mutation
   - returned value / callback / message
   - logged evidence
   - downstream consumer
7. Classify the root cause:
   - upstream lifecycle / state transition created an invalid condition
   - upstream caller sent an incomplete or wrong package
   - wrong owner or boundary violation
   - missing initialization or stale state
   - wrong candidate selection / lookup path
   - incorrect condition, sentinel, or target-state semantics
   - concurrency / lifecycle ordering issue
   - log/reporting bug hiding a real state bug
   - popup guard too broad or wrong for the contract
   - build/configuration drift
8. If the candidate cause is "popup guard too broad", prove all earlier logic is valid first. This is the rare branch, not the default branch.
9. Prefer the smallest fix at the first divergent point. Do not add broad refactors or new structures unless existing structures cannot express the invariant.

## Layered Search Protocol

Root-cause tracing must proceed by layers, starting from the nearest observable layer. Do not jump directly to a deep architectural explanation after finding the first suspicious line.

Default layers:

```text
L0 现象层：弹窗正文、debug assertion、树视图、日志统计、退出码、用户可见异常。
L1 直接生成层：弹窗调用点、视图生成函数、日志打印点、需求节点显示/枚举入口。
L2 本地事实层：直接写入/入树/提交/合并/去重函数，以及本层字段值。
L3 上游请求层：调用者、输入包、派生请求、任务回执、外设报告、方法运行结果。
L4 生命周期层：任务筹办/执行、自我线程/任务管理线程/外设线程交接、调度和停止顺序。
L5 规范契约层：需求目标、方法动作动态、线程边界、业务缺口、哨兵值、因果归因规则。
```

For each layer:

1. List every plausible same-layer cause before selecting a deeper branch.
2. For each suspicion, immediately record:

```text
层级：
疑似原因：
证据：
反证 / 未证：
下一验证：
状态：候选 / 暂排除 / 需下钻 / 阻塞
```

3. Finish the same-layer pass before going deeper unless one candidate blocks reproduction or verification.
4. Choose one branch to drill into only after recording the same-layer set. Selection priority:
   - directly explains the current symptom;
   - has concrete code/log/state evidence;
   - blocks verification of other candidates;
   - can be probed with the smallest read-only check.
5. If no same-layer candidate explains the symptom, mark the layer as `暂未命中` and move one layer outward/upstream.
6. When drilling into a branch, keep other same-layer candidates in the plan record as parked alternatives. Do not re-discover them repeatedly in the conversation.
7. Do not edit code until the active layer, selected branch, and excluded same-layer candidates are written to the `计划/追根因_...` record.

## Stack Capture With ProcDump And CDB

Use stack capture when the nearest observable is a CRT `Debug Error!`, `abort() has been called`, unhandled exception, hang, crash, or popup/debug state where Codex cannot see the Visual Studio Call Stack.

In `D:\鱼巢`, prefer the repository scripts:

```powershell
.\tools\debug-stack\Invoke-FishnestProcDump.ps1 -Mode Crash
.\tools\debug-stack\Capture-ProcessDump.ps1 -Mode Snapshot -ProcessName 鱼巢
.\tools\debug-stack\Analyze-DumpWithCdb.ps1 -DumpPath .\日志\dump\<run>\<file>.dmp
.\tools\debug-stack\Analyze-DumpWithCdb.ps1 -DumpPath .\日志\dump\<run>\<file>.dmp -ThreadScope All -TimeoutSeconds 300
.\tools\debug-stack\Analyze-DumpWithCdb.ps1 -DumpPath .\日志\dump\<run>\<file>.dmp -Mode Analyze -TimeoutSeconds 300
```

Rules:

- Treat dump stack output as L0/L1 evidence: exception site, current frame, caller chain, and thread context.
- Do not treat `!analyze -v` as the root cause by itself. Continue the layered search into the upstream state, input package, lifecycle owner, and governing spec.
- If a Debug Error dialog is already visible and Visual Studio stack is unavailable, capture a snapshot dump before choosing `Abort`.
- `ProcDump` can capture a dump without `cdb`; `cdb` is only needed to export stack text.
- Use the default analyzer mode first for fast current-thread `.ecxr` and `kpn` output. Use `-ThreadScope All` when the popup, hang, or abnormal state may be on a different thread. Use `-Mode Analyze` only when full `!analyze -v` is needed or the quick stack is insufficient.
- If `cdb.exe` is missing, record that the dump was captured but stack analysis is blocked on installing Debugging Tools for Windows or setting `CDB_PATH`.
- Never commit `.dmp` / `.mdmp` files. Keep dump artifacts under `日志\dump\` or another ignored local path.
- After stack capture, update the active `计划/追根因_...` record with dump path, stack text path if available, top frame, selected layer, and next same-layer candidates.

## Context Control

Popup root-cause work often reveals several issues. Keep only one active causal chain in the main context.

For `D:\鱼巢`, do not wait for the context to become large before writing the root-cause plan record. Create or update the `计划/追根因_...` file and put it at the top of `计划/计划索引.md` before deep tracing whenever:

- the user says `追根因` with a concrete symptom;
- the issue can touch demand-tree growth, task management, method selection, method execution, causal feedback, external observation, or popup/debug-error handling;
- the first scan shows more than one plausible branch;
- a previous run already hit compact, stream disconnect, websocket, or network interruption.

Hard checkpoint thresholds:

- after every 6 tool commands in one root-cause run;
- before every build, `鱼巢.exe` run, or long log scan;
- before switching from one causal branch to another;
- after completing the same-layer candidate pass;
- immediately after identifying a first divergent point;
- immediately after discovering two or more non-main issues.

Classify every newly found issue immediately:

- `A 主链根因候选`: directly explains the current popup. Keep tracing this.
- `B 阻塞前置问题`: must be fixed before the popup can be reproduced or verified. Switch only when it blocks the main chain.
- `C 独立问题`: real bug, but not required to explain this popup. Record and park it.
- `D 噪声 / 重复症状`: downstream duplicate, stale log, or already explained symptom. Mention only if it affects verification.

When any checkpoint threshold is reached, stop expanding the conversation and update the root-cause plan record instead.

In `D:\鱼巢`, root-cause record files must:

- live in the plan root directory: `计划/`
- start with the filename prefix `追根因_`
- use a compact recoverable name such as `计划/追根因_YYYYMMDD_<弹窗短名>_v0.1.md`
- be added to `计划/计划索引.md` as the highest-priority active plan while the popup root cause is unresolved

If a matching `计划/追根因_...` file already exists for the same popup, update that file instead of creating a duplicate. Keep its `计划/计划索引.md` entry at the top priority until the popup root cause is fixed and verified.

The root-cause plan record should be short:

```text
当前弹窗：
最高优先级原因：
当前层级：
同层疑点记录：
选中下钻分支：
主链判断：
已确认事实：
已排除：
旁支问题池：
已改文件：
已运行命令：
下一条命令：
验收证据：
禁止宣称：
```

After writing or updating the `追根因_` plan record and plan index entry, continue from `下一条命令`; do not re-read the whole repo or re-summarize all branches.

If `remote compact`, `stream disconnected`, `websocket`, or a network error appears, do not treat it as a code/build/logic failure. Resume from:

```text
AGENTS.md
-> 规范/禁止性规范20260611.md
-> 规范/规范目录.md
-> 计划/计划索引.md
-> active 计划/追根因_*.md
-> git status --short --untracked-files=no
```

Keep tool output small:

- Search targeted files first; avoid broad repeated `rg`.
- For logs, save or use existing log files, then extract only timestamp, title, body, and 5-20 nearby key lines.
- For code, cite function/file/line and summarize the condition; do not paste long functions.
- For findings, report only the active branch plus parked issue names. Do not fully analyze parked issues until they become blockers.

## Common Fishnest Popup Anchors

Use these as search anchors, but always verify against current code:

- `鱼巢 - 空指逻辑错误`: trace the pointer owner and why the caller reached a path requiring a valid object.
- `鱼巢 - 派生需求逻辑错误`: trace parent demand, target host, and whether the request should have been rejected earlier.
- `鱼巢 - 执行桥逻辑错误`: trace method execution bridge output, target state, and whether the gap belongs to method learning or a real protocol violation.
- `鱼巢 - 任务状态动作动态逻辑错误`: trace task-state submitter, method binding, action-dynamic creation, and whether task lifecycle code is being treated as an instinct method.
- `鱼巢 - 方法域动作动态逻辑错误`: trace method handle, action source, and method-domain submit path.
- `鱼巢 - 自检发现代码缺陷`: trace the self-check assertion to the violated code invariant before changing the self-check.

## 鱼巢 Hard Boundaries

- Specs outrank plans; plans outrank temporary inference. Pause if the fix would conflict with higher-level specs.
- Threads are not action sources. Method action dynamics must bind to a concrete method.
- External changes must not masquerade as self method action dynamics.
- Task planning and task execution are task-management lifecycle functions, not instinct methods, tasks, or action-dynamic sources.
- Read-only diagnostics must not create structures. Keep read paths read-only unless the command is explicitly an ensure/write path.
- `I64_MAX` is an exceptional sentinel or unset value and must not enter real service settlement.
- Method lookup must populate a candidate set before selection; one candidate can bind directly, multiple candidates need deterministic selection.
- Remote compact, stream disconnect, websocket, or network errors are execution-channel failures, not project logic failures.
- Do not declare `自我苏醒完成` or `初步成熟完成` from a bug fix.

## Fix And Verify Popup Resolution

1. Before editing, say which files or functions will be touched and why.
2. Edit minimally with `apply_patch`.
3. Build and run sequentially; never run build and `鱼巢.exe` concurrently.
4. Validate at the right level:
   - popup: rerun the same path and confirm no same-title/same-body popup plus no matching `弹窗错误提示` log in the new window
   - popup source fixed but new popup appears: treat it as a new root-cause branch, not as success
   - compile error: rebuild and confirm the original compiler diagnostic is gone
   - state bug: compare predicted state vs real logs/snapshots/action dynamics
   - performance bug: compare before/after timing or hotspot counters
   - UI/static output: regenerate output and inspect generated files; use Browser only when visual interaction is required and available
5. If a closed slice is completed in `D:\鱼巢`, update the relevant `计划/追根因_...` record and `计划/计划索引.md` evidence. Follow project git rules: commit only the current Codex slice's files; do not push unless the user asks.

## Report Format

Keep the final report short and decision-oriented:

```text
根因：
弹窗入口：
证据：
修改：
验证：
剩余风险 / 下一步：
```

If no root cause was found, say exactly where tracing stopped, what was ruled out, and what single next probe would unblock the diagnosis.

## Stop Conditions

Stop only when:

- the root cause is found, fixed, and verified with real evidence;
- the root cause is found but the safe fix requires user confirmation because of a spec or architecture conflict;
- required runtime conditions, hardware, logs, credentials, or real inputs are unavailable and cannot be fabricated under project rules;
- the current evidence proves the issue is outside the codebase or outside the available environment.
