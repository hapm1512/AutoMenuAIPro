param(
    [string]$BuildDir = "build",
    [string]$OutDir = "BetaPackage"
)

$ErrorActionPreference = "Stop"

if (Test-Path $OutDir) { Remove-Item $OutDir -Recurse -Force }
New-Item -ItemType Directory -Path $OutDir | Out-Null
New-Item -ItemType Directory -Path "$OutDir\Docs" | Out-Null
New-Item -ItemType Directory -Path "$OutDir\Beta" | Out-Null

Copy-Item "README.md" "$OutDir\" -Force
Copy-Item "Docs\UserManual.md" "$OutDir\Docs\" -Force -ErrorAction SilentlyContinue
Copy-Item "Docs\QuickStart.md" "$OutDir\Docs\" -Force -ErrorAction SilentlyContinue
Copy-Item "Docs\Troubleshooting.md" "$OutDir\Docs\" -Force -ErrorAction SilentlyContinue
Copy-Item "Beta\*" "$OutDir\Beta\" -Recurse -Force

$manifest = @()
Get-ChildItem $OutDir -Recurse -File | ForEach-Object {
    $hash = Get-FileHash $_.FullName -Algorithm SHA256
    $manifest += "$($hash.Hash)  $($_.FullName.Replace((Resolve-Path $OutDir).Path + '\\',''))"
}
$manifest | Set-Content "$OutDir\SHA256SUMS.txt"

Write-Host "Beta package prepared: $OutDir"
