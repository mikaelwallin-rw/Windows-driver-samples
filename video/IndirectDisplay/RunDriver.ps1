Write-Host "Starting IDD Sample Driver..." -ForegroundColor Cyan
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
$appPath = Join-Path $PSScriptRoot "$platform\$configuration\IddSampleApp.exe"

# Check if the app exists
if (-not (Test-Path $appPath)) {
    Write-Host "ERROR: IddSampleApp.exe not found at: $appPath" -ForegroundColor Red
    Write-Host "Please build the solution first." -ForegroundColor Yellow
    pause
    exit 1
}

# Display test signing status
Write-Host "Checking test signing status..." -ForegroundColor Cyan
$testSigning = bcdedit | Select-String "testsigning"
if ($testSigning -match "Yes") {
    Write-Host "? Test signing is ENABLED" -ForegroundColor Green
} else {
    Write-Host "? Test signing is DISABLED" -ForegroundColor Red
    Write-Host "  Run: bcdedit /set testsigning on" -ForegroundColor Yellow
    Write-Host "  Then reboot your computer" -ForegroundColor Yellow
}

Write-Host ""
Write-Host "Running IddSampleApp.exe from: $appPath" -ForegroundColor Cyan
Write-Host "The app will create virtual monitor(s)." -ForegroundColor Yellow
Write-Host "Check Display Settings (Win+P) to see them." -ForegroundColor Yellow
Write-Host ""

# Run the app
& $appPath

Write-Host ""
Write-Host "App terminated." -ForegroundColor Cyan
