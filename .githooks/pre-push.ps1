$ErrorActionPreference = "Stop"

$repoRoot = (Resolve-Path (Join-Path $PSScriptRoot "..")).Path
$projectPath = Join-Path $repoRoot "鱼巢.vcxproj"
$exePath = Join-Path $repoRoot "x64\Debug\鱼巢.exe"
$jsonPath = Join-Path $repoRoot "日志\acceptance-pre-push.json"

Write-Host "[pre-push] repo root: $repoRoot"
Write-Host "[pre-push] building Debug|x64"
& msbuild $projectPath /t:Build /p:Configuration=Debug /p:Platform=x64 /m:1 /nologo
if ($LASTEXITCODE -ne 0) {
    Write-Error "[pre-push] build failed with exit code $LASTEXITCODE"
    exit $LASTEXITCODE
}

if (-not (Test-Path $exePath)) {
    Write-Error "[pre-push] executable not found: $exePath"
    exit 8
}

Write-Host "[pre-push] running acceptance gate"
& $exePath --acceptance-report --acceptance-report-json=$jsonPath
if ($LASTEXITCODE -ne 0) {
    Write-Error "[pre-push] acceptance gate failed with exit code $LASTEXITCODE"
    exit $LASTEXITCODE
}

Write-Host "[pre-push] acceptance JSON: $jsonPath"
Write-Host "[pre-push] gate passed"
exit 0
