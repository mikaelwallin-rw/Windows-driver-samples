# Virtual Display Templates for Simulator Setups

## Physical Hardware Configuration

These templates are designed to work with the following physical monitors:
- **2x 2560x1440** monitors (Landscape mode)
- **1x 1280x800** touch screen (Landscape mode)

**Total Physical Displays: 3**

## Simulator Configurations

Each template creates only the **additional virtual displays** needed to complete the full simulator setup when combined with your physical monitors.

### Template: `sim-3x1080-landscape.json`
**Target Setup:** 3x 1920x1080 Landscape + 1x 1280x800 Touch Display Landscape

- **Virtual Displays Created:** 1x 1920x1080
- **Complete Setup:** 
  - 2x 2560x1440 (physical)
  - 1x 1920x1080 (virtual) ← Created by this template
  - 1x 1280x800 touch (physical)

```bash
.\RunDriver.ps1 --template sim-3x1080-landscape
```

---

### Template: `sim-3x1080-portrait.json`
**Target Setup:** 3x 1920x1080 Portrait + 1x 1280x800 Touch Display Landscape

- **Virtual Displays Created:** 1x 1920x1080
- **Complete Setup:**
  - 2x 2560x1440 (physical) 
  - 1x 1920x1080 (virtual) ← Created by this template
  - 1x 1280x800 touch (physical)
- **Note:** Rotate the 1920x1080 monitors to Portrait in Windows Display Settings

```bash
.\RunDriver.ps1 --template sim-3x1080-portrait
```

---

### Template: `sim-4x1080-landscape.json`
**Target Setup:** 4x 1920x1080 Landscape + 1x 1280x800 Touch Display Landscape

- **Virtual Displays Created:** 2x 1920x1080
- **Complete Setup:**
  - 2x 2560x1440 (physical)
  - 2x 1920x1080 (virtual) ← Created by this template
  - 1x 1280x800 touch (physical)

```bash
.\RunDriver.ps1 --template sim-4x1080-landscape
```

---

### Template: `sim-3x1080-portrait-1x1080-landscape.json`
**Target Setup:** 3x 1920x1080 Portrait + 1x 1920x1080 Landscape + 1x 1280x800 Touch Display Landscape

- **Virtual Displays Created:** 2x 1920x1080
- **Complete Setup:**
  - 2x 2560x1440 (physical)
  - 2x 1920x1080 (virtual) ← Created by this template
  - 1x 1280x800 touch (physical)
- **Note:** Rotate 3 of the 1920x1080 monitors to Portrait in Windows Display Settings

```bash
.\RunDriver.ps1 --template sim-3x1080-portrait-1x1080-landscape
```

---

## Display Orientation (Landscape ↔ Portrait)

The virtual display driver creates monitors at their specified resolution (width × height). To rotate displays between landscape and portrait:

### Option 1: Windows Display Settings (Manual)
1. Press `Win + P` or right-click Desktop → Display Settings
2. Select the monitor to rotate
3. Change "Display orientation" to Portrait or Landscape

### Option 2: PowerShell Script (Automated)
You can create a PowerShell script using Windows Display API or tools like:
- `DisplaySwitch.exe` (built-in Windows tool)
- Third-party tools like `MultiMonitorTool` by NirSoft
- Custom script using Windows Runtime APIs

### Option 3: Windows Settings URI
```powershell
# Open Display Settings directly
start ms-settings:display
```

## Display Positioning

After creating virtual displays, arrange them in Windows Display Settings:
1. Open Display Settings (`Win + I` → System → Display)
2. Drag and drop monitor icons to match your physical layout
3. Click "Apply" to save the configuration

## Notes

- All virtual displays are created at **60Hz refresh rate**
- Virtual displays are set to **non-primary** by default
- Initial positions are placeholders - arrange them in Windows Display Settings
- The driver creates displays with the specified resolution; orientation is controlled by Windows

## Creating Custom Templates

Template format:
```json
[
  {
    "id": "unique-monitor-id",
    "width": 1920,
    "height": 1080,
    "refreshRate": 60,
    "position": { "x": 0, "y": 0 },
    "primary": false
  }
]
```

Valid refresh rates: 60, 75, 90, 120, 144, 240 Hz
