param(
    [Parameter(Mandatory=$true)][string]$Name,
    [Parameter(Mandatory=$true)][string]$Email,
    [Parameter(Mandatory=$true)][string]$MachineCode
)

Write-Host "Offline key generation must match Source/Core/LicenseManager.cpp"
Write-Host "Customer: $Name <$Email>"
Write-Host "Machine:  $MachineCode"
Write-Host "Use the compiled internal generator for production keys."
