# ProcDump / CDB 堆栈抓取工具 Codex 断点清单

日期：2026-06-12

## 当前切片

为追根因和 Debug Error / CRT abort 场景补充命令行堆栈抓取工具链。

目标：

```text
1. 安装 ProcDump。
2. 提供启动鱼巢并自动抓异常 dump 的脚本。
3. 提供弹窗已出现时对运行中进程抓快照 dump 的脚本。
4. 提供 cdb dump 分析脚本，自动导出 !analyze -v 和调用栈。
5. cdb 未安装时必须明确提示，不伪装成已分析。
```

## 依据文件

```text
规范/禁止性规范20260611.md
规范/规范目录.md
计划/计划索引.md
```

## 已安装工具

ProcDump：

```text
C:\Users\zhchh\.codex\tools\sysinternals\procdump\procdump64.exe
版本：ProcDump v12.0
```

cdb：

```text
当前未找到 cdb.exe。
已尝试 Windows SDK / Debugging Tools 安装；当前 shell 非管理员，winget / winsdksetup 超时并已清理挂起进程。
脚本已实现 CDB_PATH / PATH / Windows Kits 常见路径自动检测。
```

## 已改文件

```text
.gitignore
tools/debug-stack/Analyze-DumpWithCdb.ps1
tools/debug-stack/Capture-ProcessDump.ps1
tools/debug-stack/Invoke-FishnestProcDump.ps1
tools/debug-stack/README.md
实施记录/20260612_ProcDump_CDB堆栈抓取工具_Codex断点清单.md
```

## 已运行命令

```text
读取：
Get-Content 规范/禁止性规范20260611.md
Get-Content 规范/规范目录.md
Get-Content 计划/计划索引.md

工具检查：
Get-Command procdump64.exe,procdump.exe,cdb.exe
检查 Windows Kits cdb 常见路径

安装：
Invoke-WebRequest https://download.sysinternals.com/files/Procdump.zip
Expand-Archive Procdump.zip
procdump64.exe -accepteula -?

cdb 尝试：
winget search / show Windows SDK
winsdksetup /layout ... /features OptionId.WindowsDesktopDebuggers
winget install Microsoft.WindowsSDK.10.0.18362 --silent ...
```

## 验证结果

```text
1. 三个 PowerShell 脚本通过 Parser.ParseFile 语法检查。
2. ProcDump v12.0 可运行。
3. Capture-ProcessDump.ps1 对临时 cmd 进程成功生成约 27 MB dump。
4. Capture-ProcessDump.ps1 在 dump 已生成时退出码归一为 0，并在日志保留 ProcDumpExitCode。
5. Analyze-DumpWithCdb.ps1 在 cdb 缺失时稳定退出 2，并写出明确提示文件。
6. 临时测试 dump 已删除。
```

## 使用入口

程序启动监控：

```powershell
.\tools\debug-stack\Invoke-FishnestProcDump.ps1 -Mode Crash
```

Debug Error 弹窗已出现时：

```powershell
.\tools\debug-stack\Capture-ProcessDump.ps1 -Mode Snapshot -ProcessName 鱼巢
```

cdb 分析：

```powershell
.\tools\debug-stack\Analyze-DumpWithCdb.ps1 -DumpPath .\日志\dump\<run>\<file>.dmp
```

## 下一步

```text
若需要自动导出真实调用栈，需要以管理员方式安装 Windows SDK 的 Debugging Tools for Windows，或把已安装的 cdb.exe 路径写入 CDB_PATH。
安装后直接重跑 Analyze-DumpWithCdb.ps1，无需改脚本。
```

## 禁止宣称

```text
1. 当前不宣称 cdb 已安装。
2. 当前不宣称已能自动分析 dump 调用栈；只能自动抓 dump，并在 cdb 可用时自动分析。
3. 当前未修改业务代码，未验证任何业务根因。
```
