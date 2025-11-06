# ?? IDD Virtual Display - Quick Reference

## **One-Line Commands** (Run as Administrator)

```powershell
# From project directory: D:\wkspaces\Windows-driver-samples\video\IndirectDisplay\

# 1. Check virtual monitors status
.\CheckDisplays.ps1

# 2. Update driver after code changes
.\UpdateDriver.ps1

# 3. Start virtual monitors
.\RunDriver.ps1
```

---

## **Current Setup**

**? Status:** Fully operational  
**? Virtual Monitors:** 4 active  
**? Test Signing:** Enabled  
**? Driver:** Installed (oem29.inf)

---

## **Common Tasks**

### Change Number of Monitors
1. Edit `IddSampleDriver\Driver.cpp` line ~25:
   ```cpp
   static constexpr DWORD IDD_SAMPLE_MONITOR_COUNT = 2;  // Change to 1, 2, 3, or 4
   ```
2. Rebuild in Visual Studio (F7)
3. Run: `.\UpdateDriver.ps1`
4. Run: `.\RunDriver.ps1`

### Change Monitor Resolution
1. Edit `IddSampleDriver\Driver.cpp` line ~50-120 (see MONITOR_CONFIGURATION_GUIDE.md)
2. Rebuild in Visual Studio (F7)
3. Run: `.\UpdateDriver.ps1`
4. Run: `.\RunDriver.ps1`

### Stop Virtual Monitors
- Press **'x'** in the IddSampleApp console window

---

## **Files**

| Script | Purpose |
|--------|---------|
| `RunDriver.ps1` | Launch app to create virtual monitors |
| `UpdateDriver.ps1` | Install/update driver after rebuild |
| `CheckDisplays.ps1` | Show all monitors and adapters |

| Documentation | Content |
|---------------|---------|
| `README_SCRIPTS.md` | Complete script documentation |
| `MONITOR_CONFIGURATION_GUIDE.md` | How to configure monitors |
| `QUICK_REFERENCE.md` | This file |

---

## **Troubleshooting**

| Problem | Solution |
|---------|----------|
| Scripts won't run | Run PowerShell as Administrator |
| Monitors don't appear | Check test signing: `bcdedit \| Select-String testsigning` |
| Driver changes ignored | Run `.\UpdateDriver.ps1` then `.\RunDriver.ps1` |
| Access denied | Ensure running as Administrator |

---

## **Configuration**

**Current:** 4 monitors with these resolutions:
1. 2560x1440 @ 144Hz
2. 3840x2160 @ 60Hz
3. 1920x1080 @ 60Hz
4. 1920x1080 @ 144Hz

**To change:** See `MONITOR_CONFIGURATION_GUIDE.md`

---

**?? Tip:** Pin this file for quick access!
