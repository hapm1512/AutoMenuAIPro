param(
    [string]$BuildDir = "build",
    [string]$JuceDir = "../JUCE"
)

$ErrorActionPreference = "Stop"

Write-Host "Vocal Suite Ultra Pro - Epic 4E Validation" -ForegroundColor Cyan
Write-Host "Project: $PWD"
Write-Host "BuildDir: $BuildDir"
Write-Host "JUCE_DIR:  $JuceDir"

if (!(Test-Path "CMakeLists.txt")) {
    throw "CMakeLists.txt not found. Run this script from project root."
}

if (!(Test-Path $BuildDir)) {
    New-Item -ItemType Directory -Path $BuildDir | Out-Null
}

Write-Host "`n[1/4] Configure Release" -ForegroundColor Yellow
cmake -B $BuildDir -S . -DJUCE_DIR=$JuceDir -DCMAKE_BUILD_TYPE=Release

Write-Host "`n[2/4] Build Release" -ForegroundColor Yellow
cmake --build $BuildDir --config Release

Write-Host "`n[3/4] Locate artifacts" -ForegroundColor Yellow
Get-ChildItem -Recurse $BuildDir -Include *.vst3,*.exe | Select-Object FullName,Length,LastWriteTime

Write-Host "`n[4/4] Validation checklist" -ForegroundColor Yellow
Write-Host "- Open Standalone"
Write-Host "- Scan VST3 in DAW"
Write-Host "- Test sample rates: 44.1, 48, 88.2, 96, 176.4, 192 kHz"
Write-Host "- Test buffers: 32, 64, 128, 256, 512, 1024, 2048"
Write-Host "- Test preset save/load"
Write-Host "- Test automation and bypass"
Write-Host "`nEpic 4E build validation completed." -ForegroundColor Green
