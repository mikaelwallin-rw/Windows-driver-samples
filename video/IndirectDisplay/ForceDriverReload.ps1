# Force complete driver reload by uninstalling and cleaning up all cached copies
# Run as Administrator

Write-Host "=== Force Driver Reload ===" -ForegroundColor Cyan
Write-Host "This will completely uninstall and clean up the driver" -ForegroundColor Yellow
Write-Host ""

# 1. Stop any running app instances
Write-Host "[1/6] Stopping IddSampleApp processes..." -ForegroundColor Cyan
Get-Process -Name "IddSampleApp" -ErrorAction SilentlyContinue | Stop-Process -Force
Start-Sleep -Milliseconds 500

# 2. Disable and remove PnP devices
Write-Host "[2/6] Removing PnP devices..." -ForegroundColor Cyan
$devices = Get-PnpDevice | Where-Object { $_.InstanceId -like "*IddSample*" }
foreach ($device in $devices) {
    Write-Host "  Disabling: $($device.InstanceId)" -ForegroundColor Gray
    Disable-PnpDevice -InstanceId $device.InstanceId -Confirm:$false -ErrorAction SilentlyContinue
    # Give Windows time to process
    Start-Sleep -Milliseconds 200
}

# 3. Kill all UMDF host processes to unload DLLs
Write-Host "[3/6] Killing WUDFHost processes to unload driver DLL..." -ForegroundColor Cyan
$wudfProcesses = Get-Process -Name "WUDFHost" -ErrorAction SilentlyContinue
if ($wudfProcesses) {
    Write-Host "  Found $($wudfProcesses.Count) WUDFHost process(es)" -ForegroundColor Gray
    Stop-Process -Name "WUDFHost" -Force -ErrorAction SilentlyContinue
    Start-Sleep -Seconds 2
    Write-Host "  WUDFHost processes terminated" -ForegroundColor Green
}
else {
    Write-Host "  No WUDFHost processes found" -ForegroundColor Gray
}

# 4. Uninstall the driver using pnputil
Write-Host "[4/6] Uninstalling driver from driver store..." -ForegroundColor Cyan
$oeminf = pnputil /enum-drivers | Select-String -Pattern "IddSampleDriver" -Context 2, 0
if ($oeminf) {
    # Extract the OEM*.inf filename
    $infFile = $oeminf | Select-String -Pattern "Published Name\s*:\s*(oem\d+\.inf)" | ForEach-Object { $_.Matches.Groups[1].Value }
    if ($infFile) {
        Write-Host "  Found driver: $infFile" -ForegroundColor Gray
        pnputil /delete-driver $infFile /uninstall /force 2>&1 | Out-Null
        Write-Host "  Driver uninstalled" -ForegroundColor Green
    }
}
else {
    Write-Host "  No installed driver found in driver store" -ForegroundColor Gray
}

# 5. Clear registry configuration
Write-Host "[5/6] Clearing registry..." -ForegroundColor Cyan
$regPath = "HKLM:\SOFTWARE\IddSampleDriver"
if (Test-Path $regPath) {
    Remove-Item -Path $regPath -Recurse -Force -ErrorAction SilentlyContinue
    Write-Host "  Registry cleared" -ForegroundColor Green
}
else {
    Write-Host "  No registry entries found" -ForegroundColor Gray
}

# 6. Wait for system to settle
Write-Host "[6/6] Waiting for system to settle..." -ForegroundColor Cyan
Start-Sleep -Seconds 2

Write-Host ""
Write-Host "=== Driver Reload Complete ===" -ForegroundColor Green
Write-Host "The driver has been completely unloaded." -ForegroundColor Green
Write-Host ""
Write-Host "Next steps:" -ForegroundColor Cyan
Write-Host "  1. Rebuild the solution if you made changes" -ForegroundColor White
Write-Host "  2. Run: .\RunDriver.ps1 --template sim-3x1080-landscape" -ForegroundColor White
Write-Host ""
