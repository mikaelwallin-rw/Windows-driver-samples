# IDD Sample Driver - Virtual Display Setup

This folder contains PowerShell scripts for managing the Indirect Display Driver (IDD) sample.

## ?? Script Files

### ?? **RunDriver.ps1**
Launches the IddSampleApp to create virtual monitors.

**Usage:**
```powershell
# Run as Administrator
.\RunDriver.ps1
```

**What it does:**
- Checks for administrator privileges
- Verifies test signing is enabled
- Launches IddSampleApp.exe from the build output
- Creates virtual monitors (count configured in Driver.cpp)

---

### ?? **UpdateDriver.ps1**
Updates the installed driver with the latest build.

**Usage:**
```powershell
# Run as Administrator
.\UpdateDriver.ps1
```

**What it does:**
- Stops any running IddSampleApp instances
- Uninstalls the previous driver version
- Installs the newly built driver from x64\Debug

**When to use:**
- After modifying Driver.cpp (monitor count, resolutions, etc.)
- After rebuilding the solution
- When driver settings aren't taking effect

---

### ?? **CheckDisplays.ps1**
Displays information about all monitors and display adapters.

**Usage:**
```powershell
.\CheckDisplays.ps1
```

**What it shows:**
- Physical screen count and properties
- Display adapter list
- Monitor device status
- Virtual monitor status

---

## ?? Configuration Files

### **MONITOR_CONFIGURATION_GUIDE.md**
Detailed guide for configuring virtual monitors.

**Topics covered:**
- How to set the number of monitors (1-4)
- How to configure resolutions per monitor
- EDID configuration explained
- Common resolution presets
- Troubleshooting tips

---

### **VALIDATION_SUMMARY.md**
Project status and implementation summary.

**Includes:**
- Current configuration status
- What's implemented
- Pending actions
- Next steps

---

## ?? Quick Start Workflow

### First Time Setup

1. **Enable test signing** (requires reboot):
   ```powershell
   # Run as Administrator
   bcdedit /set testsigning on
shutdown /r /t 0
   ```

2. **After reboot, install the driver**:
   ```powershell
 # Run as Administrator in this directory
   .\UpdateDriver.ps1
   ```

3. **Run the application**:
   ```powershell
   # Run as Administrator
   .\RunDriver.ps1
   ```

4. **Verify virtual monitors**:
   ```powershell
   .\CheckDisplays.ps1
   ```
   
   Or: Open Display Settings ? Click "Detect"

---

### Changing Monitor Configuration

1. **Edit** `IddSampleDriver\Driver.cpp`:
   ```cpp
   // Line ~25: Set monitor count (1-4)
   static constexpr DWORD IDD_SAMPLE_MONITOR_COUNT = 4;
   
// Line ~40-140: Configure resolutions per monitor
   // See MONITOR_CONFIGURATION_GUIDE.md for details
   ```

2. **Rebuild** the solution in Visual Studio (as Administrator)

3. **Update** the driver:
   ```powershell
   .\UpdateDriver.ps1
   ```

4. **Run** the application:
   ```powershell
   .\RunDriver.ps1
 ```

---

## ?? Current Configuration

**Monitor Count:** 4 virtual monitors

**Monitor Configurations:**
- **Monitor 1:** 2560x1440 @ 144Hz (1440p High Refresh)
- **Monitor 2:** 3840x2160 @ 60Hz (4K UHD)
- **Monitor 3:** 1920x1080 @ 60Hz (Standard 1080p)
- **Monitor 4:** 1920x1080 @ 144Hz (Gaming 1080p)

Each monitor supports 2-3 fallback resolutions.

---

## ?? Important Notes

### Administrator Rights Required
All scripts must be run as Administrator because:
- Driver installation/uninstallation requires elevated privileges
- Test certificate operations require system access
- Software device creation needs admin rights

### Test Signing
Windows must be in test mode to load unsigned drivers:
- Check status: `bcdedit | Select-String testsigning`
- You'll see "Test Mode" watermark on desktop when enabled

### Build Output Location
Scripts automatically locate binaries in:
```
x64\Debug\IddSampleDriver\    (driver files)
x64\Debug\IddSampleApp.exe(application)
```

If using Release build, update `$configuration` in scripts.

---

## ??? Troubleshooting

### "Access is denied" when running UpdateDriver.ps1
**Solution:** Run PowerShell as Administrator

### Virtual monitors don't appear
**Checklist:**
1. Is test signing enabled? (`bcdedit | Select-String testsigning`)
2. Is IddSampleApp.exe running? (Check Task Manager)
3. Is driver installed? Run `.\CheckDisplays.ps1`
4. Try: Settings ? Display ? Detect

### App crashes on startup
**Possible causes:**
1. Driver not installed - Run `.\UpdateDriver.ps1`
2. Certificate issue - Reinstall certificate from `x64\Debug\IddSampleDriver.cer`
3. Check Event Viewer ? System logs for errors

### Changes to Driver.cpp not taking effect
**Solution:**
```powershell
# Rebuild in Visual Studio
# Then run:
.\UpdateDriver.ps1
.\RunDriver.ps1
```

---

## ?? Additional Resources

- **Microsoft Docs:** [Indirect Display Driver](https://docs.microsoft.com/en-us/windows-hardware/drivers/display/indirect-display-driver-model-overview)
- **Sample Repository:** [Windows Driver Samples](https://github.com/microsoft/Windows-driver-samples)
- **WDK Documentation:** [Windows Driver Kit](https://docs.microsoft.com/en-us/windows-hardware/drivers/download-the-wdk)

---

## ?? Project Structure

```
IndirectDisplay\
??? RunDriver.ps1        ? Launch virtual monitors
??? UpdateDriver.ps1           ? Install/update driver
??? CheckDisplays.ps1? Display diagnostics
??? MONITOR_CONFIGURATION_GUIDE.md   ? Configuration reference
??? VALIDATION_SUMMARY.md          ? Project status
??? README_SCRIPTS.md      ? This file
??? IddSampleDriver\
?   ??? Driver.cpp          ? Monitor configuration
?   ??? Driver.h
?   ??? IddSampleDriver.inf
??? IddSampleApp\
?   ??? main.cpp     ? Application entry point
??? x64\Debug\
    ??? IddSampleApp.exe     ? Built application
    ??? IddSampleDriver\             ? Built driver package
        ??? IddSampleDriver.dll
        ??? IddSampleDriver.inf
        ??? iddsampledriver.cat
```

---

## ?? Tips

1. **Keep scripts in project root** - They won't be deleted during clean/rebuild
2. **Use CheckDisplays.ps1 for debugging** - Quickly verify monitor status
3. **Customize resolutions** - See MONITOR_CONFIGURATION_GUIDE.md
4. **Monitor the app output** - It shows device creation status
5. **Press 'x' to exit** - Cleanly removes virtual monitors

---

**Need help?** Check `MONITOR_CONFIGURATION_GUIDE.md` for detailed configuration instructions.
