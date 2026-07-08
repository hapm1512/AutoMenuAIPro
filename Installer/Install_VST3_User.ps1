$ErrorActionPreference = "Stop"

$ProductName = "Vocal Suite Ultra Pro"
$SourcePlugin = Join-Path $PSScriptRoot "..\Plugin\VocalSuiteUltraPro.vst3"
$TargetDir = Join-Path $env:LOCALAPPDATA "Programs\Common\VST3"
$TargetPlugin = Join-Path $TargetDir "$ProductName.vst3"
$PresetSource = Join-Path $PSScriptRoot "..\Presets"
$PresetTarget = Join-Path ([Environment]::GetFolderPath("MyDocuments")) "Hai Pham Audio\Vocal Suite Ultra Pro\Presets"

Write-Host "Installing $ProductName for current user..."

if (!(Test-Path $SourcePlugin)) {
    throw "Cannot find source plugin: $SourcePlugin"
}

New-Item -ItemType Directory -Force -Path $TargetDir | Out-Null
Copy-Item -Force -Recurse $SourcePlugin $TargetPlugin

if (Test-Path $PresetSource) {
    New-Item -ItemType Directory -Force -Path $PresetTarget | Out-Null
    Copy-Item -Force -Recurse (Join-Path $PresetSource "*") $PresetTarget
}

Write-Host "Installed to: $TargetPlugin"
Write-Host "Presets: $PresetTarget"
Write-Host "Rescan VST3 plugins in your DAW."
