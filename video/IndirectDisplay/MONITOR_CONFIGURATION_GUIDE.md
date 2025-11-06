# Virtual Monitor Configuration Guide

## How to Configure the Number of Virtual Monitors

### Location: `IddSampleDriver/Driver.cpp`

---

## 1. SET MONITOR COUNT (Line ~25)

Change this value to control how many virtual monitors are created:

```cpp
static constexpr DWORD IDD_SAMPLE_MONITOR_COUNT = 4;  // 1, 2, 3, or 4
```

**Options:**
- `= 1` ? Creates 1 virtual monitor
- `= 2` ? Creates 2 virtual monitors  
- `= 3` ? Creates 3 virtual monitors
- `= 4` ? Creates 4 virtual monitors (maximum in current configuration)

---

## 2. CONFIGURE RESOLUTIONS FOR EACH MONITOR

### Option A: Monitors with EDID (s_SampleMonitors array, Line ~36)

Each monitor definition includes:
- **EDID block** (128 bytes) - Identifies the monitor
- **Resolution list** (up to 3 resolutions per monitor)
- **Preferred mode index** (which resolution is default)

**Example - Monitor 1 (1440p 144Hz):**
```cpp
{
    { /* EDID bytes */ },
    {
  { 2560, 1440, 144 },  // Primary: 1440p @ 144Hz
        { 1920, 1080,  60 },  // Secondary: 1080p @ 60Hz
        { 1024,  768,  60 },  // Tertiary: 768p @ 60Hz
    },
    0  // Preferred mode = first resolution (1440p 144Hz)
}
```

**Current Monitors:**
- **Monitor 1**: 2560x1440 @ 144Hz (Dell S2719DGF)
- **Monitor 2**: 3840x2160 @ 60Hz (4K)
- **Monitor 3**: 1920x1080 @ 60Hz (1080p)
- **Monitor 4**: 1920x1080 @ 144Hz (1080p Gaming)

**To change a resolution:**
```cpp
{ Width, Height, RefreshRate }
```

### Option B: Default Modes for EDID-less Monitors (s_SampleDefaultModes, Line ~28)

If you create MORE monitors than defined in `s_SampleMonitors`, they use these defaults:

```cpp
static const struct IndirectSampleMonitor::SampleMonitorMode s_SampleDefaultModes[] = 
{
    { 1920, 1080, 60 },// 1080p @ 60Hz
    { 1600,  900, 60 },// 900p @ 60Hz
    { 1280,  720, 60 },  // 720p @ 60Hz
    { 1024,  768, 60 },  // 768p @ 60Hz (4:3)
};
```

---

## 3. DRIVER CAPABILITIES - Supported Resolutions (Line ~760)

Add or remove resolutions that the driver can process:

**Location:** `IddSampleMonitorQueryModes()` function

```cpp
TargetModes.push_back(CreateIddCxTargetMode(3840, 2160, 60));  // 4K @ 60Hz
TargetModes.push_back(CreateIddCxTargetMode(2560, 1440, 144)); // 1440p @ 144Hz
TargetModes.push_back(CreateIddCxTargetMode(1920, 1080, 144)); // 1080p @ 144Hz
TargetModes.push_back(CreateIddCxTargetMode(1920, 1080, 60));  // 1080p @ 60Hz
// Add more as needed...
```

**Windows will use the INTERSECTION of:**
- Monitor's supported modes (from EDID or defaults)
- Driver's target modes (from above)

---

## QUICK REFERENCE: Common Resolutions

| Resolution | Width | Height | Aspect Ratio | Common Name |
|------------|-------|--------|--------------|-------------|
| 3840x2160  | 3840  | 2160   | 16:9         | 4K UHD  |
| 2560x1440  | 2560  | 1440   | 16:9         | 1440p/QHD   |
| 1920x1080  | 1920  | 1080   | 16:9         | 1080p/FHD   |
| 1600x900   | 1600  | 900    | 16:9      | 900p  |
| 1280x720   | 1280  | 720    | 16:9         | 720p/HD     |
| 1024x768   | 1024  | 768    | 4:3          | XGA         |

**Refresh Rates:** 60Hz (standard), 75Hz, 90Hz, 120Hz, 144Hz (gaming)

---

## HOW TO APPLY CHANGES

### 1. Modify Driver.cpp
- Change `IDD_SAMPLE_MONITOR_COUNT`
- Optionally modify resolution arrays

### 2. Rebuild Driver
```powershell
# In Visual Studio (running as Administrator)
Build ? Rebuild Solution
```

### 3. Reinstall Driver
```powershell
# Run as Administrator
pnputil /delete-driver oem29.inf /uninstall /force
pnputil /add-driver "x64\Debug\IddSampleDriver\IddSampleDriver.inf" /install
```

### 4. Run Application
```powershell
cd x64\Debug
.\RunDriver.ps1
```

### 5. Verify in Display Settings
- Open Display Settings (Win + P)
- Click "Detect"
- You should see the configured number of virtual monitors

---

## EXAMPLES

### Example 1: Single 4K Monitor
```cpp
static constexpr DWORD IDD_SAMPLE_MONITOR_COUNT = 1;

// Use Monitor 2 configuration (4K)
// Resolution will be 3840x2160 @ 60Hz
```

### Example 2: Two 1080p Monitors
```cpp
static constexpr DWORD IDD_SAMPLE_MONITOR_COUNT = 2;

// Modify s_SampleMonitors[0] and [1] to both have:
{
    { 1920, 1080, 60 },
    { 1280,  720, 60 },
}
```

### Example 3: Four Different Monitors
```cpp
static constexpr DWORD IDD_SAMPLE_MONITOR_COUNT = 4;

// Already configured! You have:
// - 1440p @ 144Hz
// - 4K @ 60Hz
// - 1080p @ 60Hz
// - 1080p @ 144Hz
```

---

## TROUBLESHOOTING

**Q: Monitor doesn't appear in Display Settings?**
- Ensure test signing is enabled: `bcdedit | Select-String testsigning`
- Check Device Manager for "IddSampleDriver Device" under Display adapters
- Verify app is running (press 'x' to exit)

**Q: Resolution not available in Windows?**
- Check if resolution is in BOTH monitor modes AND target modes
- Verify monitor EDID includes the resolution
- Add resolution to `IddSampleMonitorQueryModes()` if missing

**Q: How to add more than 4 monitors?**
- Increase `IDD_SAMPLE_MONITOR_COUNT` to desired number
- Add more entries to `s_SampleMonitors` array (optional)
- Monitors beyond array size will use `s_SampleDefaultModes`

**Q: Can I change resolutions at runtime?**
- Not directly - requires driver rebuild and reinstall
- For dynamic configuration, you'd need to modify the driver to read from a config file
