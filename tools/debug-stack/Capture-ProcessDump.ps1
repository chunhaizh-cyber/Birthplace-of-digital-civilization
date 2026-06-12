param(
    [ValidateSet("Snapshot", "HungWindow", "Exception")]
    [string]$Mode = "Snapshot",

    [int]$ProcessId = 0,

    [string]$ProcessName = "鱼巢",

    [string]$DumpRoot = "",

    [string]$ProcDumpPath = "",

    [switch]$Analyze
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

function Resolve-Target {
    if ($ProcessId -gt 0) {
        return [string]$ProcessId
    }

    $name = [System.IO.Path]::GetFileNameWithoutExtension($ProcessName)
    $proc = Get-Process -Name $name -ErrorAction SilentlyContinue | Select-Object -First 1
    if (-not $proc) {
        throw "Process not found: $ProcessName"
    }
    return [string]$proc.Id
}

$repoRoot = Get-RepoRoot
if (-not $DumpRoot) {
    $DumpRoot = Join-Path $repoRoot "日志\dump"
}

$stamp = Get-Date -Format "yyyyMMdd_HHmmss"
$runDir = Join-Path $DumpRoot "attach_$Mode`_$stamp"
New-Item -ItemType Directory -Path $runDir -Force | Out-Null

$procdump = Resolve-ProcDumpPath -ExplicitPath $ProcDumpPath
$target = Resolve-Target
$logPath = Join-Path $runDir "procdump_attach_console.log"

$args = @("-accepteula", "-ma")
switch ($Mode) {
    "Snapshot" { }
    "HungWindow" { $args += "-h" }
    "Exception" { $args += "-e" }
}
$args += @($target, $runDir)

"ProcDump: $procdump" | Tee-Object -FilePath $logPath
"Mode: $Mode" | Tee-Object -FilePath $logPath -Append
"Target: $target" | Tee-Object -FilePath $logPath -Append
"DumpDir: $runDir" | Tee-Object -FilePath $logPath -Append

& $procdump @args 2>&1 | Tee-Object -FilePath $logPath -Append
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
    "No dump file was created." | Tee-Object -FilePath $logPath -Append
    exit $exitCode
}

exit 0
