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
PATH 中未找到裸 cdb.exe。
已确认 Microsoft.WinDbg Appx 包提供 cdb.exe：
    C:\Program Files\WindowsApps\Microsoft.WinDbg_1.2603.20001.0_x64__8wekyb3d8bbwe\amd64\cdb.exe
Analyze-DumpWithCdb.ps1 已实现 CDB_PATH / PATH / WinDbg Appx / Windows Kits 常见路径自动检测。
默认使用快速当前线程栈；需要完整 !analyze 或全线程栈时显式开启对应参数，避免现场验证被符号服务器或全线程托管栈拖住。
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
6. Analyze-DumpWithCdb.ps1 已能自动发现 WinDbg Appx cdb.exe。
7. 对临时 powershell 进程 dump 运行：
   powershell -NoProfile -ExecutionPolicy Bypass -File .\tools\debug-stack\Analyze-DumpWithCdb.ps1 -DumpPath <temp>.dmp -Mode Stack -ThreadScope Current -TimeoutSeconds 60
   退出码 0，生成 .cdb.txt，并包含 `Microsoft (R) Windows Debugger Version 10.0.29547.1002 AMD64`、`Child-SP`、`ntdll!NtWaitForMultipleObjects`、`KERNELBASE!WaitForMultipleObjectsEx` 等栈证据。
8. `-ThreadScope All` 在托管 PowerShell 测试进程上会持续输出大量线程栈；脚本已增加超时杀进程并保留部分输出，避免残留 cdb 进程。
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
.\tools\debug-stack\Analyze-DumpWithCdb.ps1 -DumpPath .\日志\dump\<run>\<file>.dmp -ThreadScope All -TimeoutSeconds 300
.\tools\debug-stack\Analyze-DumpWithCdb.ps1 -DumpPath .\日志\dump\<run>\<file>.dmp -Mode Analyze -TimeoutSeconds 300
```

## 下一步

```text
后续遇到真实 Debug Error / CRT abort 弹窗时：
1. 先用 Capture-ProcessDump.ps1 -Mode Snapshot -ProcessName 鱼巢 抓现场。
2. 先运行默认 Analyze-DumpWithCdb.ps1 获取当前线程栈。
3. 若当前线程不是弹窗 / abort 线程，再用 -ThreadScope All -TimeoutSeconds 300 导出全线程栈并在 .cdb.txt 中检索 MessageBox / abort / 项目弹窗错误提示 / 鱼巢业务帧。
```

## 禁止宣称

```text
1. 不宣称 cdb 在 PATH 中可裸调用；当前是脚本自动发现 WinDbg Appx cdb.exe。
2. 不宣称完整 !analyze-v 每次都会快速完成；默认先取快速栈，必要时再完整分析。
3. 当前未修改业务代码，未验证任何业务根因。
```
