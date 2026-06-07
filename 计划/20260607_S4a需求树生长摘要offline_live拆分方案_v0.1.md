# S4a 需求树生长摘要 offline/live 拆分方案 v0.1

日期：2026-06-07

## 1. 目标

```text
让需求树生长摘要类短命令不再默认支付完整自我环境冷启动成本。
```

## 2. 依据

```text
当前 --demand-tree-growth-summary 仍进入 私有_确保自我环境已初始化；
此前样本显示短摘要命令总耗时约 9-10 秒，主要成本在新进程完整初始化；
控制面板摘要轻量快照读取本身很快。
```

## 3. 边界

```text
1. 离线摘要不得初始化自我环境。
2. 离线摘要不得创建世界树结构、需求、任务、方法、状态、动态或价值账。
3. live 摘要保留完整初始化语义。
4. 输出必须标注 offline/live 口径、快照来源和缺快照原因。
5. 不混入任务管理、日志 flush、D455、候选 / 因果 / 基础信息索引改造。
```

## 4. 最小实现

```text
在 鱼巢.cpp 中拆分命令：
    --demand-tree-growth-summary
        默认走 offline 轻量快照读取。

    --demand-tree-growth-summary-live
        保留现有完整初始化路径。

若无可用离线快照：
    输出明确的无可用快照提示；
    退出码保持 0，表示诊断命令成功执行但当前无 live 快照。
```

## 5. 验证预测

```text
构建：
    Debug x64 应通过。

离线路径：
    .\x64\Debug\鱼巢.exe --demand-tree-growth-summary 应不调用 私有_确保自我环境已初始化；
    命令耗时应显著低于 live 冷启动；
    输出标注 offline。

live 路径：
    .\x64\Debug\鱼巢.exe --demand-tree-growth-summary-live 应保留现有初始化语义；
    输出标注 live。

负向核对：
    离线路径不应出现 命令行/确保自我环境初始化 日志；
    不应出现 日志模块低级错误；
    不应出现 状态提交确认失败、后台worker工作项进入死信、拒绝归并完成项。
```

## 6. 执行结果

```text
实现：
    --demand-tree-growth-summary 默认改为 offline，只读取当前进程已有的控制面板摘要线程最新快照；
    当前进程没有已采集快照时，输出无可用快照原因并 0 码返回；
    --demand-tree-growth-summary-live 新增为显式 live 入口，保留旧完整初始化和即时轻量快照读取语义。

构建：
    msbuild .\鱼巢.vcxproj /p:Configuration=Debug /p:Platform=x64 /p:LinkIncremental=false /m
    退出码=0，警告=既有 LNK4075。

真实运行：
    .\x64\Debug\鱼巢.exe --demand-tree-growth-summary
    退出码=0，耗时约 82ms，输出标注 offline、来源=控制面板摘要线程最新快照、快照序号=0、无可用快照原因。

    .\x64\Debug\鱼巢.exe --demand-tree-growth-summary-live
    退出码=0，耗时约 9554ms，输出标注 live，并输出完整需求树生长摘要。

日志核对：
    offline 窗口只出现 控制面板命令/需求树生长摘要 | 模式=offline 的开始 / 完成日志；
    live 窗口出现 命令行/确保自我环境初始化，标记=鱼巢::main/控制面板/需求树生长摘要/live，耗时ms=9448；
    S4a 最新窗口未出现 日志模块低级错误、状态提交确认失败、后台worker工作项进入死信、拒绝归并完成项。

结论：
    S4a 已闭合；
    默认短摘要不再支付完整自我环境冷启动成本；
    缺离线快照时不隐式创建结构，用户可显式调用 live 入口取得完整即时摘要。
```
