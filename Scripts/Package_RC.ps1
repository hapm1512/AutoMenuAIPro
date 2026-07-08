param(
    [string]$BuildDir = "build",
    [string]$Config = "Release",
    [string]$OutDir = "ReleasePackage"
)

$ErrorActionPreference = "Stop"

Write-Host "Vocal Suite Ultra Pro - RC Packaging"
Write-Host "BuildDir: $BuildDir"
Write-Host "Config:   $Config"
Write-Host "OutDir:   $OutDir"

if (!(Test-Path $OutDir)) {
    New-Item -ItemType Directory -Path $OutDir | Out-Null
}

$docsOut = Join-Path $OutDir "Docs"
if (!(Test-Path $docsOut)) {
    New-Item -ItemType Directory -Path $docsOut | Out-Null
}

Copy-Item "README.md" $OutDir -Force
Copy-Item "Docs\CHANGELOG.md" $docsOut -Force
Copy-Item "Docs\Release_Checklist.md" $docsOut -Force
Copy-Item "Docs\Validation_Test_Matrix.md" $docsOut -Force
Copy-Item "Scripts\Host_QA_Checklist.md" $docsOut -Force

$vst3Candidates = Get-ChildItem -Path $BuildDir -Recurse -Filter "*.vst3" -ErrorAction SilentlyContinue

if ($vst3Candidates.Count -eq 0) {
    Write-Warning "No VST3 artifact found. Build first, then rerun this script."
} else {
    $pluginOut = Join-Path $OutDir "Plugin"
    if (!(Test-Path $pluginOut)) {
        New-Item -ItemType Directory -Path $pluginOut | Out-Null
    }

    foreach ($item in $vst3Candidates) {
        Copy-Item $item.FullName $pluginOut -Recurse -Force
        Write-Host "Copied:" $item.FullName
    }
}

Write-Host "RC package prepared."
