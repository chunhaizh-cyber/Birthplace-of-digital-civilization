param(
    [Parameter(Mandatory = $true)]
    [string]$DumpPath,

    [string]$OutputPath = "",

    [string]$CdbPath = "",

    [string]$SymbolCache = "$env:USERPROFILE\.codex\symbols"
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
$commands = ".symfix; .sympath $symbolPath; .reload; !analyze -v; .ecxr; kpn; ~* kpn; q"

& $cdb -z $dump -lines -c $commands 2>&1 | Tee-Object -FilePath $OutputPath
exit $LASTEXITCODE
