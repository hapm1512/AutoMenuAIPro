$ErrorActionPreference = "Stop"

$Root = Split-Path -Parent $PSScriptRoot
$Product = "VocalSuiteUltraPro"
$Version = "5.2.0"
$PackageRoot = Join-Path $Root "Package\${Product}_${Version}_Windows"
$PluginSourceCandidates = @(
    Join-Path $Root "build\VocalSuiteUltraPro_artefacts\Release\VST3\Vocal Suite Ultra Pro.vst3",
    Join-Path $Root "build\VocalSuiteUltraPro_artefacts\RelWithDebInfo\VST3\Vocal Suite Ultra Pro.vst3",
    Join-Path $Root "build\VST3\Vocal Suite Ultra Pro.vst3"
)

Write-Host "Creating commercial package..."
Remove-Item -Recurse -Force $PackageRoot -ErrorAction SilentlyContinue
New-Item -ItemType Directory -Force -Path $PackageRoot | Out-Null
New-Item -ItemType Directory -Force -Path (Join-Path $PackageRoot "Plugin") | Out-Null
New-Item -ItemType Directory -Force -Path (Join-Path $PackageRoot "Docs") | Out-Null
New-Item -ItemType Directory -Force -Path (Join-Path $PackageRoot "Presets") | Out-Null
New-Item -ItemType Directory -Force -Path (Join-Path $PackageRoot "Installer") | Out-Null

$PluginSource = $null
foreach ($Candidate in $PluginSourceCandidates) {
    if (Test-Path $Candidate) {
        $PluginSource = $Candidate
        break
    }
}

if ($PluginSource -ne $null) {
    Copy-Item -Recurse -Force $PluginSource (Join-Path $PackageRoot "Plugin\VocalSuiteUltraPro.vst3")
} else {
    Write-Warning "VST3 build output not found. Build Release first, then rerun this script."
}

Copy-Item -Force (Join-Path $Root "README.md") $PackageRoot
Copy-Item -Force (Join-Path $Root "Docs\CHANGELOG.md") $PackageRoot
Copy-Item -Force (Join-Path $Root "Docs\UserManual.md") (Join-Path $PackageRoot "Docs")
Copy-Item -Force (Join-Path $Root "Docs\QuickStart.md") (Join-Path $PackageRoot "Docs")
Copy-Item -Force (Join-Path $Root "Docs\PresetGuide.md") (Join-Path $PackageRoot "Docs")
Copy-Item -Force (Join-Path $Root "Docs\HostSetup.md") (Join-Path $PackageRoot "Docs")
Copy-Item -Force (Join-Path $Root "Docs\Troubleshooting.md") (Join-Path $PackageRoot "Docs")
Copy-Item -Recurse -Force (Join-Path $Root "Assets\Presets\*") (Join-Path $PackageRoot "Presets")
Copy-Item -Recurse -Force (Join-Path $Root "Installer\*") (Join-Path $PackageRoot "Installer")

$HashFile = Join-Path $PackageRoot "SHA256SUMS.txt"
Get-ChildItem -Path $PackageRoot -Recurse -File | ForEach-Object {
    $hash = Get-FileHash -Algorithm SHA256 $_.FullName
    "$($hash.Hash)  $($_.FullName.Replace($PackageRoot + '\\',''))"
} | Set-Content -Encoding UTF8 $HashFile

$ZipPath = Join-Path (Join-Path $Root "Package") "${Product}_${Version}_Windows.zip"
Remove-Item -Force $ZipPath -ErrorAction SilentlyContinue
Compress-Archive -Path (Join-Path $PackageRoot "*") -DestinationPath $ZipPath

Write-Host "Package created: $ZipPath"
