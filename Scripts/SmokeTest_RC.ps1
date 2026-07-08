$ErrorActionPreference = "Stop"

Write-Host "Vocal Suite Ultra Pro - RC Smoke Test"

$required = @(
    "CMakeLists.txt",
    "README.md",
    "Source\PluginProcessor.cpp",
    "Source\PluginEditor.cpp",
    "Source\DSP\Engine\DspEngine.cpp",
    "Source\DSP\OutputModule.cpp",
    "Source\Core\PresetManager.cpp",
    "Source\Core\ReleaseInfo.h",
    "Docs\Release_Checklist.md",
    "Docs\CHANGELOG.md"
)

$failed = $false

foreach ($path in $required) {
    if (Test-Path $path) {
        Write-Host "OK   $path"
    } else {
        Write-Host "MISS $path" -ForegroundColor Red
        $failed = $true
    }
}

if ($failed) {
    throw "Smoke test failed. Missing files detected."
}

Write-Host "Smoke test passed."
