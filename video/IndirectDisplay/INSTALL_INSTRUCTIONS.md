# Driver Installation Instructions

## The driver has been rebuilt successfully!

Since your computer was rebooted, the old cached driver is now gone (that's why you see 0 virtual monitors).

## Next Steps - Run as Administrator:

1. **Open PowerShell as Administrator** (right-click → Run as Administrator)

2. **Navigate to project directory:**
   ```powershell
   cd "D:\wkspaces\Windows-driver-samples\video\IndirectDisplay"
   ```

3. **Install and run the driver:**
   ```powershell
   .\RunDriver.ps1 --template sim-3x1080-landscape
   ```

## Expected Result:

You should see **1 new virtual monitor** (1920x1080 @ 60Hz) because the driver is hardcoded to create exactly 1 monitor right now.

**Total displays:** 4 (3 physical + 1 virtual)

## What This Test Proves:

- ✅ If you see 1 virtual monitor → Driver is working and loading fresh code
- ❌ If you see 0 virtual monitors → Check for errors in the RunDriver.ps1 output
- ❌ If you see 4 virtual monitors → Old driver is still cached somehow

## After This Test:

Once we confirm the driver creates 1 monitor, I'll remove the hardcoded logic and re-enable the JSON template system, which will allow you to use your templates properly.
