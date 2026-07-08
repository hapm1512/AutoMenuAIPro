$ErrorActionPreference = "Stop"

$ProductName = "Vocal Suite Ultra Pro"
$UserPlugin = Join-Path $env:LOCALAPPDATA "Programs\Common\VST3\$ProductName.vst3"
$SystemPlugin = "C:\Program Files\Common Files\VST3\$ProductName.vst3"

Write-Host "Uninstalling $ProductName..."

if (Test-Path $UserPlugin) {
    Remove-Item -Recurse -Force $UserPlugin
    Write-Host "Removed: $UserPlugin"
}

if (Test-Path $SystemPlugin) {
    Remove-Item -Recurse -Force $SystemPlugin
    Write-Host "Removed: $SystemPlugin"
}

Write-Host "Preset files are kept in Documents for safety."
