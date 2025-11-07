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

# Disable PowerShell script execution policy
$oldPolicy = Get-ExecutionPolicy
Set-ExecutionPolicy -Scope Process -ExecutionPolicy Bypass -Force

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

# Restore original execution policy
Set-ExecutionPolicy -Scope Process -ExecutionPolicy $oldPolicy -Force

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

# Parse and display configuration options
$configInfo = ""
if ($args.Count -gt 0) {
    # Check for help flags
    if ($args -contains "--help" -or $args -contains "-h" -or $args -contains "/?") {
        Write-Host "Usage: RunDriver.ps1 [options]" -ForegroundColor Cyan
        Write-Host ""
        Write-Host "Options:" -ForegroundColor Yellow
        Write-Host "  --config <path>     Load monitor configuration from JSON file"
        Write-Host "  --template <name>      Load predefined template (e.g., dual-1080p, 2x2-grid)"
        Write-Host "  --list-templates       List available configuration templates"
        Write-Host "  --help                 Show this help message"
        Write-Host ""
        Write-Host "Examples:" -ForegroundColor Yellow
        Write-Host "  .\RunDriver.ps1       # Use default 4-monitor configuration"
        Write-Host "  .\RunDriver.ps1 --template dual-1080p   # Use dual 1080p template"
        Write-Host "  .\RunDriver.ps1 --config C:\monitors.json  # Use custom configuration"
        Write-Host "  .\RunDriver.ps1 --list-templates             # List available templates"
        Write-Host ""
        exit 0
    }
    
    # Check for list-templates
    if ($args -contains "--list-templates") {
        Write-Host "Available templates:" -ForegroundColor Cyan
        Write-Host ""
        & $appPath --list-templates
        exit 0
    }
 
    # Build config info string
    for ($i = 0; $i -lt $args.Count; $i++) {
        if ($args[$i] -eq "--template") {
            $configInfo = "Using template: $($args[$i+1])"
            break
        }
        elseif ($args[$i] -eq "--config") {
            $configInfo = "Using config file: $($args[$i+1])"
            break
        }
    }
    
    if ($configInfo) {
        Write-Host $configInfo -ForegroundColor Green
        Write-Host ""
    }
}
else {
    Write-Host "Using default configuration (4 monitors)" -ForegroundColor Yellow
    Write-Host "Tip: Use --help to see configuration options" -ForegroundColor Gray
    Write-Host ""
}

Write-Host "Running IddSampleApp.exe from: $appPath" -ForegroundColor Cyan
Write-Host "The app will create virtual monitor(s)." -ForegroundColor Yellow
Write-Host "Check Display Settings (Win+P) to see them." -ForegroundColor Yellow
Write-Host ""

# Run the app with all arguments passed through
if ($args.Count -gt 0) {
    & $appPath $args
}
else {
    & $appPath
}

Write-Host ""
Write-Host "App terminated." -ForegroundColor Cyan
