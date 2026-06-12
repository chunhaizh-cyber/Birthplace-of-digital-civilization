param(
    [Parameter(Mandatory = $true)]
    [string]$DumpPath,

    [string]$OutputPath = "",

    [string]$CdbPath = "",

    [string]$SymbolCache = "$env:USERPROFILE\.codex\symbols",

    [ValidateSet("Stack", "Analyze")]
    [string]$Mode = "Stack",

    [ValidateSet("Current", "All")]
    [string]$ThreadScope = "Current",

    [int]$TimeoutSeconds = 120
)

$ErrorActionPreference = "Stop"

function Resolve-CdbPath {
    param([string]$ExplicitPath)

    if ($ExplicitPath -and (Test-Path -LiteralPath $ExplicitPath)) {
        return (Resolve-Path -LiteralPath $ExplicitPath).Path
    }

    if ($env:CDB_PATH -and (Test-Path -LiteralPath $env:CDB_PATH)) {
        return (Resolve-Path -LiteralPath $env:CDB_PATH).Path
    }

    $cmd = Get-Command cdb.exe -ErrorAction SilentlyContinue
    if ($cmd) {
        return $cmd.Source
    }

    $appxCandidates = @()
    try {
        $appxCandidates += Get-AppxPackage Microsoft.WinDbg -ErrorAction SilentlyContinue |
            Sort-Object Version -Descending |
            ForEach-Object {
                Join-Path -Path $_.InstallLocation -ChildPath "amd64\cdb.exe"
                Join-Path -Path $_.InstallLocation -ChildPath "x86\cdb.exe"
            }
    } catch {
        $appxCandidates = @()
    }

    $windowsApps = Join-Path -Path $env:ProgramFiles -ChildPath "WindowsApps"
    if (Test-Path -LiteralPath $windowsApps) {
        $appxCandidates += Get-ChildItem -LiteralPath $windowsApps -Directory -Filter "Microsoft.WinDbg_*__8wekyb3d8bbwe" -ErrorAction SilentlyContinue |
            Sort-Object Name -Descending |
            ForEach-Object {
                Join-Path -Path $_.FullName -ChildPath "amd64\cdb.exe"
                Join-Path -Path $_.FullName -ChildPath "x86\cdb.exe"
            }
    }

    foreach ($candidate in $appxCandidates) {
        if ($candidate -and (Test-Path -LiteralPath $candidate)) {
            return (Resolve-Path -LiteralPath $candidate).Path
        }
    }

    $candidates = @(
        "C:\Program Files (x86)\Windows Kits\10\Debuggers\x64\cdb.exe",
        "C:\Program Files\Windows Kits\10\Debuggers\x64\cdb.exe",
        "C:\Program Files (x86)\Windows Kits\11\Debuggers\x64\cdb.exe",
        "C:\Program Files\Windows Kits\11\Debuggers\x64\cdb.exe"
    )

    foreach ($candidate in $candidates) {
        if (Test-Path -LiteralPath $candidate) {
            return $candidate
        }
    }

    return ""
}

function ConvertTo-CommandArgument {
    param([string]$Value)

    if ($null -eq $Value) {
        return '""'
    }

    if ($Value -match '[\s"]') {
        return '"' + ($Value -replace '"', '\"') + '"'
    }

    return $Value
}

$dump = (Resolve-Path -LiteralPath $DumpPath).Path
if (-not $OutputPath) {
    $OutputPath = [System.IO.Path]::ChangeExtension($dump, ".cdb.txt")
}

$cdb = Resolve-CdbPath -ExplicitPath $CdbPath
if (-not $cdb) {
    $message = @"
cdb.exe not found.

Install "Debugging Tools for Windows" from the Windows SDK, or set CDB_PATH to the full cdb.exe path.
Expected common path:
  C:\Program Files (x86)\Windows Kits\10\Debuggers\x64\cdb.exe
  C:\Program Files\WindowsApps\Microsoft.WinDbg_*\amd64\cdb.exe

Dump is still available:
  $dump
"@
    New-Item -ItemType Directory -Path (Split-Path -Parent $OutputPath) -Force | Out-Null
    $message | Set-Content -LiteralPath $OutputPath -Encoding UTF8
    Write-Warning $message
    exit 2
}

New-Item -ItemType Directory -Path (Split-Path -Parent $OutputPath) -Force | Out-Null
New-Item -ItemType Directory -Path $SymbolCache -Force | Out-Null

$symbolPath = "srv*$SymbolCache*https://msdl.microsoft.com/download/symbols"
if ($Mode -eq "Analyze") {
    $commands = ".reload; !analyze -v; .ecxr; kpn"
} else {
    $commands = ".ecxr; kpn"
}
if ($ThreadScope -eq "All") {
    $commands += "; ~* kpn"
}
$commands += "; q"

$arguments = @("-z", $dump, "-lines", "-y", $symbolPath, "-c", $commands) |
    ForEach-Object { ConvertTo-CommandArgument $_ }
$argumentLine = $arguments -join " "

$tempBase = "$OutputPath.$([guid]::NewGuid().ToString('N'))"
$stdoutPath = "$tempBase.out"
$stderrPath = "$tempBase.err"
$process = Start-Process -FilePath $cdb -ArgumentList $argumentLine -NoNewWindow -PassThru -RedirectStandardOutput $stdoutPath -RedirectStandardError $stderrPath

if (-not $process.WaitForExit($TimeoutSeconds * 1000)) {
    Stop-Process -Id $process.Id -Force -ErrorAction SilentlyContinue
    $message = @"
cdb.exe timed out after $TimeoutSeconds seconds.

Mode:
  $Mode

ThreadScope:
  $ThreadScope

CDB:
  $cdb

Dump:
  $dump
"@
    $partial = @($message)
    if (Test-Path -LiteralPath $stdoutPath) {
        $partial += Get-Content -LiteralPath $stdoutPath -Raw
    }
    if (Test-Path -LiteralPath $stderrPath) {
        $partial += Get-Content -LiteralPath $stderrPath -Raw
    }
    $partial -join "`r`n" | Set-Content -LiteralPath $OutputPath -Encoding UTF8
    Remove-Item -LiteralPath $stdoutPath, $stderrPath -Force -ErrorAction SilentlyContinue
    Write-Warning $message
    exit 124
}

$output = @()
if (Test-Path -LiteralPath $stdoutPath) {
    $output += Get-Content -LiteralPath $stdoutPath -Raw
}
if (Test-Path -LiteralPath $stderrPath) {
    $output += Get-Content -LiteralPath $stderrPath -Raw
}
$outputText = $output -join "`r`n"
$outputText | Set-Content -LiteralPath $OutputPath -Encoding UTF8
Remove-Item -LiteralPath $stdoutPath, $stderrPath -Force -ErrorAction SilentlyContinue

Write-Output $outputText
exit $process.ExitCode
