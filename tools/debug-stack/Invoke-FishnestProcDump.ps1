param(
    [string]$ExePath = "",

    [ValidateSet("Crash", "FirstChance", "CrashAndExit")]
    [string]$Mode = "Crash",

    [string]$DumpRoot = "",

    [string]$ProcDumpPath = "",

    [switch]$Analyze,

    [Parameter(ValueFromRemainingArguments = $true)]
    [string[]]$Arguments
)

$ErrorActionPreference = "Stop"

function Get-RepoRoot {
    return (Resolve-Path -LiteralPath (Join-Path $PSScriptRoot "..\..")).Path
}

function Resolve-ProcDumpPath {
    param([string]$ExplicitPath)

    if ($ExplicitPath -and (Test-Path -LiteralPath $ExplicitPath)) {
        return (Resolve-Path -LiteralPath $ExplicitPath).Path
    }

    if ($env:PROCDUMP_PATH -and (Test-Path -LiteralPath $env:PROCDUMP_PATH)) {
        return (Resolve-Path -LiteralPath $env:PROCDUMP_PATH).Path
    }

    $local = "$env:USERPROFILE\.codex\tools\sysinternals\procdump\procdump64.exe"
    if (Test-Path -LiteralPath $local) {
        return $local
    }

    $cmd = Get-Command procdump64.exe -ErrorAction SilentlyContinue
    if ($cmd) {
        return $cmd.Source
    }

    $cmd = Get-Command procdump.exe -ErrorAction SilentlyContinue
    if ($cmd) {
        return $cmd.Source
    }

    throw "ProcDump not found. Install Sysinternals ProcDump or set PROCDUMP_PATH."
}

$repoRoot = Get-RepoRoot
if (-not $ExePath) {
    $ExePath = Join-Path $repoRoot "x64\Debug\鱼巢.exe"
}
if (-not (Test-Path -LiteralPath $ExePath)) {
    throw "Executable not found: $ExePath"
}

if (-not $DumpRoot) {
    $DumpRoot = Join-Path $repoRoot "日志\dump"
}

$stamp = Get-Date -Format "yyyyMMdd_HHmmss"
$runDir = Join-Path $DumpRoot "run_$Mode`_$stamp"
New-Item -ItemType Directory -Path $runDir -Force | Out-Null

$procdump = Resolve-ProcDumpPath -ExplicitPath $ProcDumpPath
$logPath = Join-Path $runDir "procdump_run_console.log"

$procArgs = @("-accepteula", "-ma")
switch ($Mode) {
    "Crash" { $procArgs += "-e" }
    "FirstChance" { $procArgs += @("-e", "1") }
    "CrashAndExit" { $procArgs += @("-e", "-t") }
}
$procArgs += @("-x", $runDir, (Resolve-Path -LiteralPath $ExePath).Path)
if ($Arguments) {
    $procArgs += $Arguments
}

"ProcDump: $procdump" | Tee-Object -FilePath $logPath
"Mode: $Mode" | Tee-Object -FilePath $logPath -Append
"Exe: $ExePath" | Tee-Object -FilePath $logPath -Append
"Args: $($Arguments -join ' ')" | Tee-Object -FilePath $logPath -Append
"DumpDir: $runDir" | Tee-Object -FilePath $logPath -Append

& $procdump @procArgs 2>&1 | Tee-Object -FilePath $logPath -Append
$exitCode = $LASTEXITCODE
"ProcDumpExitCode: $exitCode" | Tee-Object -FilePath $logPath -Append

$dumps = Get-ChildItem -LiteralPath $runDir -Filter "*.dmp" -File -ErrorAction SilentlyContinue | Sort-Object LastWriteTime -Descending
if ($Analyze -and $dumps) {
    $analyzer = Join-Path $PSScriptRoot "Analyze-DumpWithCdb.ps1"
    foreach ($dump in $dumps) {
        & $analyzer -DumpPath $dump.FullName
    }
}

if (-not $dumps) {
    "No dump file was created. If a CRT Debug Error dialog is waiting, run Capture-ProcessDump.ps1 -Mode Snapshot in another shell before choosing Abort." | Tee-Object -FilePath $logPath -Append
    exit $exitCode
}

exit 0
