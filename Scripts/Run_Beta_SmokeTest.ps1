$ErrorActionPreference = "Stop"

$checks = @(
    "CMakeLists.txt",
    "README.md",
    "Docs\UserManual.md",
    "Docs\Epic05E_Beta_Test.md",
    "Beta\BETA_README.md",
    "Beta\BetaFeedbackForm.md",
    "Beta\BetaIssueTracker.csv",
    "Beta\BetaTestMatrix.csv",
    "Source\Core\ReleaseInfo.h"
)

$failed = $false
foreach ($item in $checks) {
    if (Test-Path $item) {
        Write-Host "OK: $item"
    } else {
        Write-Host "MISSING: $item"
        $failed = $true
    }
}

if ($failed) { exit 1 }
Write-Host "Beta smoke test passed."
