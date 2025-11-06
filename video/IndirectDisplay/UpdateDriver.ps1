Write-Host "=== Updating IddSampleDriver ===" -ForegroundColor Cyan
Write-Host ""

# Check if running as admin
$isAdmin = ([Security.Principal.WindowsPrincipal] [Security.Principal.WindowsIdentity]::GetCurrent()).IsInRole([Security.Principal.WindowsBuiltInRole]::Administrator)
if (-not $isAdmin) {
    Write-Host "ERROR: This script must be run as Administrator!" -ForegroundColor Red
    Write-Host "Please right-click PowerShell and select 'Run as Administrator'" -ForegroundColor Yellow
    pause
    exit 1
}

# Determine build configuration
$configuration = "Debug"
$platform = "x64"
$driverPath = Join-Path $PSScriptRoot "$platform\$configuration\IddSampleDriver\IddSampleDriver.inf"

# Check if the driver package exists
if (-not (Test-Path $driverPath)) {
    Write-Host "ERROR: Driver not found at: $driverPath" -ForegroundColor Red
    Write-Host "Please build the solution first." -ForegroundColor Yellow
    pause
    exit 1
}

# Stop running app
Write-Host "Stopping IddSampleApp..." -ForegroundColor Yellow
Get-Process | Where-Object { $_.ProcessName -eq "IddSampleApp" } | Stop-Process -Force -ErrorAction SilentlyContinue
Start-Sleep -Seconds 2

# Find the OEM inf
Write-Host "Finding installed driver..." -ForegroundColor Yellow
$drivers = pnputil /enum-drivers
$oemInf = $null
for ($i = 0; $i -lt $drivers.Count; $i++) {
    if ($drivers[$i] -match "iddsampledriver.inf") {
        $oemInf = $drivers[$i-1] -replace "Published Name:\s+", ""
        Write-Host "Found: $oemInf" -ForegroundColor Green
        break
    }
}

if ($oemInf) {
    # Uninstall
    Write-Host "Uninstalling old driver..." -ForegroundColor Yellow
    pnputil /delete-driver $oemInf /uninstall | Out-Null
    
    Write-Host "Deleting old driver package..." -ForegroundColor Yellow
    pnputil /delete-driver $oemInf /force 2>$null | Out-Null
    
    Start-Sleep -Seconds 2
}

# Install new version
Write-Host "Installing new driver from: $driverPath" -ForegroundColor Yellow
$result = pnputil /add-driver $driverPath /install

if ($result -match "successfully") {
    Write-Host "? Driver installed successfully!" -ForegroundColor Green
Write-Host ""
    Write-Host "You can now run RunDriver.ps1 to test the updated driver." -ForegroundColor Cyan
} else {
    Write-Host "? Driver installation failed!" -ForegroundColor Red
    Write-Host $result -ForegroundColor Yellow
}

Write-Host ""
