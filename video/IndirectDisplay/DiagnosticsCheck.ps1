# Diagnostics check for IDD Sample Driver
Write-Host "=== IDD Sample Driver Diagnostics ===" -ForegroundColor Cyan
Write-Host ""

# 1. Check if device is in Device Manager
Write-Host "[1] Checking Device Manager..." -ForegroundColor Cyan
$iddDevice = Get-PnpDevice | Where-Object { $_.FriendlyName -like "*IddSample*" }
if ($iddDevice) {
    Write-Host "  Found: $($iddDevice.FriendlyName)" -ForegroundColor Green
    Write-Host "    Status: $($iddDevice.Status)" -ForegroundColor Gray
    Write-Host "    Instance ID: $($iddDevice.InstanceId)" -ForegroundColor Gray
}
else {
    Write-Host "  IddSampleDriver device not found" -ForegroundColor Red
}
Write-Host ""

# 2. Check if driver is in driver store
Write-Host "[2] Checking Driver Store..." -ForegroundColor Cyan
$driverInfo = pnputil /enum-drivers | Select-String -Pattern "IddSampleDriver" -Context 3, 3
if ($driverInfo) {
    Write-Host "  Driver found in driver store" -ForegroundColor Green
}
else {
    Write-Host "  Driver not in driver store" -ForegroundColor Red
}
Write-Host ""

# 3. Check for WUDFHost processes
Write-Host "[3] Checking WUDFHost processes..." -ForegroundColor Cyan
$wudfProcesses = Get-Process -Name "WUDFHost" -ErrorAction SilentlyContinue
if ($wudfProcesses) {
    Write-Host "  Found $($wudfProcesses.Count) WUDFHost process(es)" -ForegroundColor Green
}
else {
    Write-Host "  No WUDFHost processes running" -ForegroundColor Red
}
Write-Host ""

# 4. Check for monitors
Write-Host "[4] Checking Monitors..." -ForegroundColor Cyan
$monitors = Get-PnpDevice | Where-Object { $_.Class -eq "Monitor" }
Write-Host "  Found $($monitors.Count) monitor(s):" -ForegroundColor White
$monitors | ForEach-Object {
    Write-Host "    - $($_.FriendlyName) [$($_.Status)]" -ForegroundColor Gray
}
Write-Host ""

# 5. Check if IddSampleApp is running
Write-Host "[5] Checking IddSampleApp..." -ForegroundColor Cyan
$appProcess = Get-Process -Name "IddSampleApp" -ErrorAction SilentlyContinue
if ($appProcess) {
    Write-Host "  IddSampleApp is running (PID: $($appProcess.Id))" -ForegroundColor Green
}
else {
    Write-Host "  IddSampleApp is not running" -ForegroundColor Red
}
Write-Host ""

Write-Host "=== Diagnostics Complete ===" -ForegroundColor Cyan
