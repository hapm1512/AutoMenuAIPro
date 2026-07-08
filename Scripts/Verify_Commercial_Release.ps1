$ErrorActionPreference = "Stop"

Write-Host "Verifying Vocal Suite Ultra Pro commercial release files..."

$required = @(
    "CMakeLists.txt",
    "README.md",
    "Source/Core/ReleaseInfo.h",
    "Docs/UserManual.md",
    "Docs/Commercial_Release_Checklist.md",
    "Docs/EULA_DRAFT.md",
    "Docs/Support_Policy.md",
    "Assets/Presets"
)

$failed = $false
foreach ($item in $required) {
    if (!(Test-Path $item)) {
        Write-Host "MISSING: $item"
        $failed = $true
    } else {
        Write-Host "OK: $item"
    }
}

if ($failed) { throw "Commercial release verification failed." }

Write-Host "Commercial release verification passed."
