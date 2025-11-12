# Cleanup script to remove all IDD Sample Driver instances
# Run this if virtual displays are stuck or not being removed properly

Write-Host "Cleaning up IDD Sample Driver..." -ForegroundColor Cyan

# Try to disable and remove the device
$devices = Get-PnpDevice -FriendlyName "*Idd Sample*" -ErrorAction SilentlyContinue

if ($devices) {
    Write-Host "Found $($devices.Count) IDD Sample device(s)" -ForegroundColor Yellow
    
    foreach ($device in $devices) {
        Write-Host "  Removing: $($device.FriendlyName)" -ForegroundColor Gray
        Disable-PnpDevice -InstanceId $device.InstanceId -Confirm:$false -ErrorAction SilentlyContinue
        # Note: Actual removal happens when the app closes the device handle
    }
    
    Write-Host "`nDevices disabled. Virtual displays should be removed." -ForegroundColor Green
}
else {
    Write-Host "No IDD Sample devices found." -ForegroundColor Green
}

# Clear the registry configuration
Write-Host "`nClearing registry configuration..." -ForegroundColor Cyan
$regPath = "HKLM:\SOFTWARE\IddSampleDriver"
if (Test-Path $regPath) {
    Remove-ItemProperty -Path $regPath -Name "ConfigPath" -ErrorAction SilentlyContinue
    Remove-ItemProperty -Path $regPath -Name "ConfigJSON" -ErrorAction SilentlyContinue
    Write-Host "Registry configuration cleared." -ForegroundColor Green
}
else {
    Write-Host "No registry configuration found." -ForegroundColor Gray
}

# Force unload of any cached driver DLLs by killing the UMDF host process
Write-Host "`nForcing driver DLL reload..." -ForegroundColor Cyan
$wudfProcesses = Get-Process -Name "WUDFHost" -ErrorAction SilentlyContinue
if ($wudfProcesses) {
    Write-Host "Stopping WUDFHost processes to reload driver DLL..." -ForegroundColor Yellow
    Stop-Process -Name "WUDFHost" -Force -ErrorAction SilentlyContinue
    Start-Sleep -Milliseconds 500
    Write-Host "WUDFHost processes stopped." -ForegroundColor Green
}
else {
    Write-Host "No WUDFHost processes running." -ForegroundColor Gray
}

Write-Host "`nCleanup complete!" -ForegroundColor Green
Write-Host "You can now run .\RunDriver.ps1 with a new template." -ForegroundColor Cyan
