param(
    [string]$BuildDir = "build",
    [string]$OutDir = "ReleasePackage"
)

$ErrorActionPreference = "Stop"

Write-Host "Packaging Vocal Suite Ultra Pro commercial release..."

if (!(Test-Path $OutDir)) { New-Item -ItemType Directory -Force -Path $OutDir | Out-Null }

$docs = @(
    "Docs/UserManual.md",
    "Docs/QuickStart.md",
    "Docs/PresetGuide.md",
    "Docs/Troubleshooting.md",
    "Docs/HostSetup.md",
    "Docs/CHANGELOG.md",
    "Docs/EULA_DRAFT.md",
    "Docs/Support_Policy.md",
    "Docs/Commercial_Release_Checklist.md"
)

foreach ($doc in $docs) {
    if (Test-Path $doc) { Copy-Item $doc $OutDir -Force }
}

if (Test-Path "Assets/Presets") {
    Copy-Item "Assets/Presets" "$OutDir/Presets" -Recurse -Force
}

Get-ChildItem $OutDir -Recurse -File | Get-FileHash -Algorithm SHA256 |
    ForEach-Object { "$($_.Hash)  $($_.Path)" } |
    Set-Content "$OutDir/SHA256SUMS.txt"

Compress-Archive -Path "$OutDir/*" -DestinationPath "VocalSuiteUltraPro_v1.0_Commercial.zip" -Force

Write-Host "Commercial package created: VocalSuiteUltraPro_v1.0_Commercial.zip"
