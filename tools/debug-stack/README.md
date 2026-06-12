# Debug stack capture tools

用途：在 Codex 看不到 Visual Studio Call Stack 时，用 ProcDump 抓取 `鱼巢.exe` 的 dump，并用 `cdb.exe` 导出调用栈。

## 本机工具位置

ProcDump 已按用户级工具安装到：

```text
C:\Users\zhchh\.codex\tools\sysinternals\procdump\procdump64.exe
```

`cdb.exe` 属于 Microsoft Debugging Tools for Windows。本机已通过 `Microsoft.WinDbg` Appx 包提供 `cdb.exe`，脚本会自动查找：

```text
C:\Program Files\WindowsApps\Microsoft.WinDbg_*\amd64\cdb.exe
```

如果自动查找失败，需要以管理员方式安装 Windows SDK 中的 `Debugging Tools for Windows`，或设置：

```powershell
$env:CDB_PATH = 'C:\Program Files (x86)\Windows Kits\10\Debuggers\x64\cdb.exe'
```

## 启动程序并自动抓异常 dump

```powershell
.\tools\debug-stack\Invoke-FishnestProcDump.ps1 -Mode Crash
```

更激进地抓 first-chance exception：

```powershell
.\tools\debug-stack\Invoke-FishnestProcDump.ps1 -Mode FirstChance
```

传递 `鱼巢.exe` 参数：

```powershell
.\tools\debug-stack\Invoke-FishnestProcDump.ps1 -Mode Crash -- --panel-html
```

## Debug Error 弹窗已出现时抓现场

不要先点 Ignore。另开一个 PowerShell：

```powershell
.\tools\debug-stack\Capture-ProcessDump.ps1 -Mode Snapshot -ProcessName 鱼巢
```

抓完后再在弹窗中选 Abort。

## 用 cdb 分析已有 dump

默认使用快速栈模式，导出当前线程调用栈：

```powershell
.\tools\debug-stack\Analyze-DumpWithCdb.ps1 -DumpPath .\日志\dump\<run>\xxx.dmp
```

需要查找弹窗 UI 线程、后台阻塞线程或非当前线程异常时，再导出全线程栈：

```powershell
.\tools\debug-stack\Analyze-DumpWithCdb.ps1 -DumpPath .\日志\dump\<run>\xxx.dmp -ThreadScope All -TimeoutSeconds 300
```

需要完整 `!analyze -v` 时再显式开启，避免符号服务器或扩展初始化拖慢弹窗现场排查：

```powershell
.\tools\debug-stack\Analyze-DumpWithCdb.ps1 -DumpPath .\日志\dump\<run>\xxx.dmp -Mode Analyze -TimeoutSeconds 300
```

分析输出默认写到同目录的 `.cdb.txt`。快速模式关键段落：

```text
.ecxr
kpn
```

完整模式会额外执行：

```text
!analyze -v
```

全线程模式会额外执行：

```text
~* kpn
```

## 输出位置

默认输出到：

```text
日志\dump\
```

该目录用于本机诊断产物，不应提交大 `.dmp` 文件。
