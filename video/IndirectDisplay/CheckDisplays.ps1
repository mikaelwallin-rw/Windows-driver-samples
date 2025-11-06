Write-Host "=== Checking Display Configuration ===" -ForegroundColor Cyan
Write-Host ""

# Load System.Windows.Forms to get screen information
Add-Type -AssemblyName System.Windows.Forms

Write-Host "Physical Screens detected by Windows:" -ForegroundColor Yellow
$screens = [System.Windows.Forms.Screen]::AllScreens
foreach ($screen in $screens) {
    Write-Host "  Monitor: $($screen.DeviceName)" -ForegroundColor Green
    Write-Host "    Bounds: $($screen.Bounds)" 
    Write-Host "    Primary: $($screen.Primary)"
    Write-Host "    BitsPerPixel: $($screen.BitsPerPixel)"
    Write-Host ""
}

Write-Host "Total screens: $($screens.Count)" -ForegroundColor Cyan
Write-Host ""

# Check PnP devices
Write-Host "=== Display Adapters ===" -ForegroundColor Cyan
Get-PnpDevice -Class "Display" | Format-Table FriendlyName, Status -AutoSize

Write-Host ""
Write-Host "=== Monitors ===" -ForegroundColor Cyan
Get-PnpDevice -Class "Monitor" | Format-Table FriendlyName, Status -AutoSize
