$ErrorActionPreference = "Stop"

$Root = Split-Path -Parent $PSScriptRoot
$PackageDir = Join-Path $Root "Package"

Write-Host "Verifying package directory..."

if (!(Test-Path $PackageDir)) {
    throw "Package directory not found. Run Scripts\Package_Commercial.ps1 first."
}

$LatestPackage = Get-ChildItem $PackageDir -Filter "VocalSuiteUltraPro_*_Windows" -Directory | Sort-Object LastWriteTime -Descending | Select-Object -First 1
if ($null -eq $LatestPackage) {
    throw "No package folder found."
}

$Required = @(
    "README.md",
    "CHANGELOG.md",
    "Docs\UserManual.md",
    "Docs\QuickStart.md",
    "Docs\PresetGuide.md",
    "Docs\HostSetup.md",
    "Docs\Troubleshooting.md",
    "Installer\Install_VST3_User.ps1",
    "Installer\Install_VST3_System.ps1",
    "Installer\Uninstall_VST3.ps1",
    "SHA256SUMS.txt"
)

foreach ($Item in $Required) {
    $Path = Join-Path $LatestPackage.FullName $Item
    if (!(Test-Path $Path)) {
        throw "Missing package item: $Item"
    }
}

Write-Host "Package verification passed: $($LatestPackage.FullName)"
