---
name: yu-chao-execute
description: Execute or resume the D:\鱼巢 plan-index workflow. Use when the user says "执行", "继续", "按照计划索引执行", "继续执行计划", "继续按照计划索引执行", or asks Codex to proceed from `计划/计划索引.md` until all active work is complete or a concrete unsolved blocker is reached.
---

# 鱼巢执行

## Core Meaning

Treat "执行" as:

```text
读取当前权威计划索引
-> 按当前主动队列 / 推进顺序选择最小闭合切片
-> 实施、构建、真实运行或日志验证
-> 更新断点 / 计划证据
-> 继续下一项
-> 直到目标计划全部阶段完成或遇到无法解决的具体问题
```

Do not stop after only reading plans, summarizing the next item, closing P0, finishing one stage, committing, building successfully, or confirming a likely implementation path. Keep moving until the target plan is complete or a real blocker is reached.

If the user says "执行计划", "执行完整计划", or explicitly asks to execute a named plan, treat that as authorization to run the whole registered plan continuously. A plan section such as "第一轮只执行 P0" or "本轮只执行 P0" is a slice boundary, not a stop condition, unless it explicitly says "硬停止 / 必须等待用户确认 / 不得自动继续".

## Start Or Resume

1. Confirm the workspace is `D:\鱼巢`.
2. Read `AGENTS.md` if present.
3. Read at least `规范/规范目录.md` and `计划/计划索引.md`.
4. If the active item touches internal loop, demand tree, task management, method selection, method execution, causal feedback, peripheral observation, or remote continuation, read the relevant specs named by `AGENTS.md` and the index.
5. After context compaction, network interruption, or a user message like "继续", do not rely on hidden session context. Restore from `AGENTS.md`, specs, `计划/计划索引.md`, recent `实施记录/*Codex断点清单.md`, and `git status --short --untracked-files=no`.

## Execution Loop

1. Route only from `计划/计划索引.md`.
2. Respect the split between `当前主动队列` / `推进顺序` and `待条件具备后继续`; do not reopen parked history unless the active item explicitly requires it.
3. Choose the smallest closed slice that can advance the active item.
4. Before long work, broad edits, builds, or real runs, create or update a checkpoint file under `实施记录/YYYYMMDD_主题_Codex断点清单.md`.
5. Gather context with scoped `rg`, path-limited `git status`, path-limited diffs, and short file reads.
6. As soon as the first independent issue is found, write it into the current checkpoint or a temporary analysis file with: issue anchor, evidence lines, excluded paths, current judgment, and next validation step. Do this before broadening the search.
7. If a second independent issue appears while the first is still open, immediately close or park the first as its own slice, clear the active search context down to the necessary anchors, then continue with the second issue.
8. Before behavior validation, write the expected prediction first, then run the real path, then compare logs, state snapshots, action dynamics, and error logs against the prediction.
9. Edit minimally with `apply_patch`; keep terminology and module boundaries aligned with the specs.
10. Build and run sequentially. Do not run builds and `鱼巢.exe` concurrently.
11. Never add or depend on mock tests, fixture tests, fake samples, fake entries, or fake action dynamics.
12. When a slice closes, update the checkpoint / plan evidence with only real completed work and real evidence. Follow the repo git rule: commit only this Codex slice's related files; do not push unless the user asks to sync.
13. After each closed slice, re-read the current plan stop point and immediately enter the next executable slice of the same target plan. Do not wait for another user prompt at P0/P1/P2 boundaries, after commits, or after successful build/run checks.
14. If the next slice requires code changes, repeat the required code-edit preflight from `AGENTS.md` before editing. Continuous execution does not waive spec reads, file-header reads, function-document checks, locks, checkpoints, or git hygiene.

## Plan Registration And Locks

- Before executing a user-specified plan, confirm it is registered in `计划/计划索引.md`.
- If the plan is missing from the index, first perform a registration-only slice.
- Registration must not silently replace `当前主动队列`; retarget only when the user explicitly asks to execute or switch to that plan.
- Before editing `计划/计划索引.md`, acquire `计划/.计划索引.lock` with owner, target plan, action, base index hash, and timestamp.
- If another fresh plan-index lock exists, do not edit `计划/计划索引.md`; write a registration request under `计划/待注册项/` or stop with the lock owner and target plan.
- After editing the index, re-read it and verify the base hash did not change unexpectedly before continuing.

## Build And Run Lock

- In the same worktree, default Debug x64 builds and `鱼巢.exe` runs are mutually exclusive across Codex windows.
- Before `msbuild` or running `鱼巢.exe`, acquire `.codex_locks/build-run.lock` with owner, action, project, configuration, platform, output directory, exe path, pid, and timestamp.
- If another fresh build-run lock exists, do not build or run; report the owner/action or wait only if the user asked to continue waiting.
- A stale lock may be removed only after checking that its pid is gone and recording that decision in the current checkpoint.
- An isolated `OutDir`/`IntDir` build can be used only as a temporary compile-health check; it does not replace default-exe runtime validation.

## Stop Conditions

Continue execution until one of these is true:

- The target plan's required stages are completed and recorded with evidence.
- All current active items in `计划/计划索引.md` are completed and recorded with evidence.
- The plan explicitly declares a hard stop that must wait for user confirmation before the next stage.
- A concrete blocker cannot be solved in the current run.

Concrete blockers include:

- A spec / plan / code conflict where the higher-level rule is unclear and needs user confirmation.
- A missing contract or authority boundary that cannot be safely inferred.
- A required hardware, real sample, runtime condition, credential, or external dependency is unavailable and the project forbids fabrication.
- A build or runtime failure remains after focused diagnosis and needs information outside the current environment.

Non-blockers:

- The next active item is found.
- P0 or a single stage is completed.
- A checkpoint is written or a commit is created.
- A build or static check succeeds.
- The plan says "first round / this round only" without explicitly declaring a hard user-confirmation stop.
- The implementation path is large but bounded.
- A first build or run fails with diagnosable output.
- Evidence is partial but there is a clear next scoped step.

## Boundaries

- Do not declare `自我苏醒完成` or `初步成熟完成`.
- Do not turn `任务筹办` or `任务执行` into instinct methods, tasks, or action-dynamic sources.
- Do not let external changes masquerade as self method action dynamics.
- Do not put explanation text into machine-decision fields; keep diagnostics in independent logs or display-only text.
- Do not let `I64_MAX` enter real service settlement.
- Treat remote compact, stream disconnect, websocket, or network errors as execution-channel interruptions, not project logic failures.
