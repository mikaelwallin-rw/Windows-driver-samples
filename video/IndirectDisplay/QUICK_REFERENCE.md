# ?? IDD Virtual Display - Quick Reference

## **One-Line Commands** (Run as Administrator)

```powershell
# From project directory: D:\wkspaces\Windows-driver-samples\video\IndirectDisplay\

# 1. Check virtual monitors status
.\CheckDisplays.ps1

# 2. Update driver after code changes
.\UpdateDriver.ps1

# 3. Start virtual monitors (default configuration)
.\RunDriver.ps1

# 4. Start with a template
.\RunDriver.ps1 --template dual-1080p

# 5. Start with custom configuration
.\RunDriver.ps1 --config C:\my-monitors.json

# 6. List available templates
.\RunDriver.ps1 --list-templates
```

---

## **Current Setup**

**? Status:** Fully operational  
**? Virtual Monitors:** Configurable (1-10)  
**? Test Signing:** Enabled  
**? Driver:** Installed  
**? Dynamic Config:** Supported

---

## **Quick Configuration Options**

### Using Templates (No Rebuild Required!)

```powershell
# Single monitor
.\RunDriver.ps1 --template single-1080p

# Dual monitors
.\RunDriver.ps1 --template dual-1080p

# 2x2 grid (4 monitors)
.\RunDriver.ps1 --template 2x2-grid

# 4K + 1080p combo
.\RunDriver.ps1 --template 4k-plus-1080p

# List all templates
.\RunDriver.ps1 --list-templates
```

### Using Custom Configuration

```powershell
# Create your own config
notepad C:\monitors.json

# Load it
.\RunDriver.ps1 --config C:\monitors.json
```

### Default Configuration (Hardcoded)

```powershell
# Uses 4 monitors defined in Driver.cpp
.\RunDriver.ps1
```

---

## **Common Tasks**

### Quick: Use Different Monitor Layout (No Rebuild!)

```powershell
# Just run with different template
.\RunDriver.ps1 --template dual-1080p

# Press 'x' to stop, then run with different template
.\RunDriver.ps1 --template 2x2-grid
```

### Advanced: Change Hardcoded Default Configuration

1. Edit `IddSampleDriver\Driver.cpp` line ~25:
   ```cpp
   static constexpr DWORD IDD_SAMPLE_MONITOR_COUNT = 2;  // Change to 1, 2, 3, or 4
   ```
2. Rebuild in Visual Studio (F7)
3. Run: `.\UpdateDriver.ps1`
4. Run: `.\RunDriver.ps1`

### Stop Virtual Monitors

- Press **'x'** in the IddSampleApp console window

---

## **Files**

| Script | Purpose |
|--------|---------|
| `RunDriver.ps1` | Launch app (supports --template, --config, --list-templates) |
| `UpdateDriver.ps1` | Install/update driver after rebuild |
| `CheckDisplays.ps1` | Show all monitors and adapters |

| Documentation | Content |
|---------------|---------|
| `DYNAMIC_CONFIG_GUIDE.md` | Complete JSON configuration guide |
| `README_SCRIPTS.md` | Script documentation |
| `MONITOR_CONFIGURATION_GUIDE.md` | Driver internals |
| `QUICK_REFERENCE.md` | This file |

---

## **Configuration Templates**

| Template | Monitors | Description |
|----------|----------|-------------|
| `single-1080p` | 1 | Single 1920x1080 @ 60Hz |
| `dual-1080p` | 2 | Two 1080p side-by-side |
| `2x2-grid` | 4 | Four 1080p in 2x2 grid |
| `4k-plus-1080p` | 2 | 4K + 1080p combo |

**Location:** `templates/` directory

---

## **Troubleshooting**

| Problem | Solution |
|---------|----------|
| Scripts won't run | Run PowerShell as Administrator |
| Monitors don't appear | Check test signing: `bcdedit \| Select-String testsigning` |
| Driver changes ignored | Run `.\UpdateDriver.ps1` then `.\RunDriver.ps1` |
| Access denied | Ensure running as Administrator |
| Template not found | Use `.\RunDriver.ps1 --list-templates` |
| Config errors | Check JSON syntax at jsonlint.com |

---

## **Example JSON Configuration**

```json
[
  {
    "id": "main",
    "width": 2560,
    "height": 1440,
    "refreshRate": 144,
    "position": { "x": 0, "y": 0 },
    "primary": true
  },
  {
    "id": "secondary",
    "width": 1920,
    "height": 1080,
    "refreshRate": 60,
    "position": { "x": 2560, "y": 180 }
  }
]
```

**Save as:** `C:\my-monitors.json`  
**Run with:** `.\RunDriver.ps1 --config C:\my-monitors.json`

---

## **Supported Resolutions**

| Resolution | Width x Height | Aspect Ratio |
|------------|----------------|--------------|
| 720p (HD) | 1280 x 720 | 16:9 |
| 1080p (FHD) | 1920 x 1080 | 16:9 |
| 1440p (QHD) | 2560 x 1440 | 16:9 |
| 4K (UHD) | 3840 x 2160 | 16:9 |
| Ultrawide | 3440 x 1440 | 21:9 |

**Refresh Rates:** 60, 75, 90, 120, 144, 240 Hz

---

?? **Tips:**

- **No Rebuild Needed:** Use templates to switch layouts instantly
- **Create Shortcuts:** Make desktop shortcuts with different `--template` args
- **Test First:** Try templates before creating custom configs
- **Validate JSON:** Use jsonlint.com to check your custom configs
- **Pin This File:** For quick reference!

---

**Need More Info?** See `DYNAMIC_CONFIG_GUIDE.md` for complete documentation.
