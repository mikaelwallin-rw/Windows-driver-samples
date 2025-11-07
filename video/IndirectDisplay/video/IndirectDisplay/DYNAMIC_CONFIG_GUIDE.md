# ??? Dynamic Monitor Configuration Guide

## Overview

The IddSampleDriver now supports dynamic monitor configuration through JSON files. You can define custom monitor layouts, resolutions, and positions without recompiling the driver.

---

## ?? Quick Start

### Using Templates

```powershell
# List available templates
.\RunDriver.ps1 --list-templates

# Use a predefined template
.\RunDriver.ps1 --template dual-1080p
.\RunDriver.ps1 --template 2x2-grid
```

### Using Custom Configuration

```powershell
# Load custom configuration
.\RunDriver.ps1 --config "C:\path\to\your\config.json"
```

### Default Behavior

```powershell
# Run without arguments uses default 4-monitor configuration
.\RunDriver.ps1
```

### Getting Help

```powershell
# Show available options
.\RunDriver.ps1 --help
```

---

## ?? Configuration File Format

### Basic Structure

```json
[
  {
    "id": "monitor1",
    "width": 1920,
    "height": 1080,
    "refreshRate": 60,
    "position": { "x": 0, "y": 0 },
    "primary": true
  },
  {
  "id": "monitor2",
    "width": 2560,
    "height": 1440,
    "refreshRate": 144,
    "position": { "x": 1920, "y": 0 }
  }
]
```

### Required Fields

| Field | Type | Description | Constraints |
|-------|------|-------------|-------------|
| `id` | string | Unique monitor identifier | 1-50 characters, alphanumeric plus `-_` |
| `width` | integer | Monitor width in pixels | 640-7680 |
| `height` | integer | Monitor height in pixels | 480-4320 |

### Optional Fields

| Field | Type | Default | Description | Valid Values |
|-------|------|---------|-------------|--------------|
| `refreshRate` | integer | 60 | Refresh rate in Hz | 60, 75, 90, 120, 144, 240 |
| `position` | object | {x:0, y:0} | Monitor position | Any integer coordinates |
| `position.x` | integer | 0 | Horizontal position in pixels | Any integer |
| `position.y` | integer | 0 | Vertical position in pixels | Any integer |
| `primary` | boolean | false | Whether this is the primary monitor | true/false |

---

## ?? Built-in Templates

### `single-1080p`
One 1920x1080 @ 60Hz monitor

```json
[
  {
    "id": "primary",
    "width": 1920,
    "height": 1080,
    "refreshRate": 60,
    "position": { "x": 0, "y": 0 },
    "primary": true
  }
]
```

### `dual-1080p`
Two 1920x1080 @ 60Hz monitors side by side

```json
[
  {
    "id": "left",
    "width": 1920,
    "height": 1080,
    "refreshRate": 60,
    "position": { "x": 0, "y": 0 },
    "primary": true
  },
  {
    "id": "right",
    "width": 1920,
    "height": 1080,
    "refreshRate": 60,
    "position": { "x": 1920, "y": 0 }
  }
]
```

### `2x2-grid`
Four 1920x1080 @ 60Hz monitors in a 2x2 grid

```json
[
  {
    "id": "top-left",
    "width": 1920,
    "height": 1080,
    "refreshRate": 60,
  "position": { "x": 0, "y": 0 },
    "primary": true
  },
  {
"id": "top-right",
    "width": 1920,
    "height": 1080,
    "refreshRate": 60,
    "position": { "x": 1920, "y": 0 }
  },
  {
    "id": "bottom-left",
    "width": 1920,
    "height": 1080,
    "refreshRate": 60,
    "position": { "x": 0, "y": 1080 }
  },
  {
    "id": "bottom-right",
    "width": 1920,
    "height": 1080,
    "refreshRate": 60,
    "position": { "x": 1920, "y": 1080 }
  }
]
```

### `4k-plus-1080p`
One 4K @ 60Hz + one 1080p @ 60Hz side by side

```json
[
  {
    "id": "primary-4k",
    "width": 3840,
    "height": 2160,
    "refreshRate": 60,
    "position": { "x": 0, "y": 0 },
    "primary": true
  },
  {
    "id": "secondary-1080p",
    "width": 1920,
    "height": 1080,
    "refreshRate": 60,
    "position": { "x": 3840, "y": 0 }
  }
]
```

---

## ?? Common Layouts

### Triple Monitor (1 + 2)
One center monitor with two side monitors in portrait mode

```json
[
  {
    "id": "center",
 "width": 2560,
    "height": 1440,
    "refreshRate": 144,
    "position": { "x": 1080, "y": 0 },
    "primary": true
  },
  {
    "id": "left-portrait",
    "width": 1080,
    "height": 1920,
    "refreshRate": 60,
    "position": { "x": 0, "y": 0 }
  },
  {
    "id": "right-portrait",
    "width": 1080,
    "height": 1920,
    "refreshRate": 60,
    "position": { "x": 3640, "y": 0 }
  }
]
```

### Stacked Monitors
One monitor on top of another

```json
[
  {
  "id": "top",
    "width": 1920,
    "height": 1080,
    "refreshRate": 60,
    "position": { "x": 0, "y": 0 },
    "primary": true
  },
  {
    "id": "bottom",
    "width": 1920,
    "height": 1080,
    "refreshRate": 60,
    "position": { "x": 0, "y": 1080 }
  }
]
```

### Ultra-wide Simulated
Simulated ultra-wide using two monitors

```json
[
  {
    "id": "left-half",
    "width": 1920,
    "height": 1080,
    "refreshRate": 60,
    "position": { "x": 0, "y": 0 },
    "primary": true
  },
  {
    "id": "right-half",
    "width": 1920,
    "height": 1080,
    "refreshRate": 60,
    "position": { "x": 1920, "y": 0 }
  }
]
```

---

## ?? Error Handling

### Common Errors

**Invalid JSON Syntax**
```
Error: Unable to parse config.json: Invalid JSON - expected array
```
**Solution:** Validate your JSON using a JSON validator (e.g., jsonlint.com)

**Missing Required Field**
```
Error: Monitor "monitor2" is missing required field "width"
```
**Solution:** Add the missing field to the monitor configuration

**Invalid Resolution**
```
Error: Monitor "monitor1" has invalid width 500 (must be 640-7680)
```
**Solution:** Use a resolution within the valid range

**Overlapping Monitors**
```
Error: Monitors "monitor1" and "monitor2" overlap
```
**Solution:** Adjust monitor positions so they don't overlap

**Duplicate Monitor ID**
```
Error: Duplicate monitor ID "monitor1"
```
**Solution:** Ensure each monitor has a unique `id`

**Too Many Monitors**
```
Error: Too many monitors defined (12), maximum is 10
```
**Solution:** Reduce the number of monitors to 10 or fewer

**Template Not Found**
```
Error: Template not found: my-template
Use --list-templates to see available templates
```
**Solution:** Check available templates with `--list-templates`

---

## ??? Creating Custom Templates

### Step 1: Create JSON File

Create a new `.json` file in the `templates/` directory:

```
video/IndirectDisplay/templates/my-custom-layout.json
```

### Step 2: Define Monitors

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

### Step 3: Use Template

```powershell
.\RunDriver.ps1 --template my-custom-layout
```

---

## ?? Supported Resolutions

### Common 16:9 Resolutions

| Resolution | Width | Height | Aspect Ratio |
|------------|-------|--------|--------------|
| HD (720p) | 1280 | 720 | 16:9 |
| FHD (1080p) | 1920 | 1080 | 16:9 |
| QHD (1440p) | 2560 | 1440 | 16:9 |
| 4K UHD | 3840 | 2160 | 16:9 |
| 5K | 5120 | 2880 | 16:9 |

### Common 16:10 Resolutions

| Resolution | Width | Height | Aspect Ratio |
|------------|-------|--------|--------------|
| WXGA | 1280 | 800 | 16:10 |
| WSXGA+ | 1680 | 1050 | 16:10 |
| WUXGA | 1920 | 1200 | 16:10 |
| WQXGA | 2560 | 1600 | 16:10 |

### Common 4:3 Resolutions

| Resolution | Width | Height | Aspect Ratio |
|------------|-------|--------|--------------|
| VGA | 640 | 480 | 4:3 |
| XGA | 1024 | 768 | 4:3 |
| SXGA | 1280 | 1024 | 5:4 |
| UXGA | 1600 | 1200 | 4:3 |

### Ultra-wide Resolutions

| Resolution | Width | Height | Aspect Ratio |
|------------|-------|--------|--------------|
| UWQHD | 3440 | 1440 | 21:9 |
| UW4K | 3840 | 1600 | 24:10 |
| UW5K | 5120 | 2160 | 21:9 |

---

## ?? Advanced Usage

### Dynamic Configuration Switching

```powershell
# Switch between different layouts
.\RunDriver.ps1 --template dual-1080p
# Close app with 'x'

.\RunDriver.ps1 --template 2x2-grid
# Close app with 'x'
```

### Using Both Script and Direct App

You can use either approach:

```powershell
# Option 1: Using RunDriver.ps1 (recommended - includes checks)
.\RunDriver.ps1 --template dual-1080p

# Option 2: Direct app execution
cd x64\Debug
.\IddSampleApp.exe --template dual-1080p
```

---

## ?? Best Practices

1. **Always validate JSON** before loading (use jsonlint.com or similar)
2. **Use descriptive monitor IDs** (e.g., "main-display", "left-portrait")
3. **Test with small configurations first** (1-2 monitors) before complex layouts
4. **Keep refresh rates realistic** (60Hz for most displays, 144Hz for gaming)
5. **Align monitor positions** to avoid gaps or overlaps
6. **Set one monitor as primary** for proper Windows behavior
7. **Document custom templates** with comments (in separate docs, JSON doesn't support comments)

---

## ?? Example Workflows

### Workflow 1: Testing a Custom Configuration

```powershell
# 1. Create config file
notepad C:\my-monitors.json

# 2. Test configuration
.\RunDriver.ps1 --config C:\my-monitors.json

# 3. If errors, fix and retry
# 4. Once working, save as template for reuse
copy C:\my-monitors.json templates\my-layout.json
```

### Workflow 2: Quickly Switching Layouts

```powershell
# Create shortcuts with different templates
# Shortcut 1: Target = RunDriver.ps1 --template single-1080p
# Shortcut 2: Target = RunDriver.ps1 --template dual-1080p
# Shortcut 3: Target = RunDriver.ps1 --template 2x2-grid
```

---

## ?? Troubleshooting

### Configuration Not Loading

**Symptom:** Driver uses default configuration instead of custom one

**Solutions:**
1. Check registry value: `reg query "HKLM\SOFTWARE\IddSampleDriver" /v ConfigPath`
2. Verify file path is absolute (e.g., `C:\path\to\config.json`)
3. Ensure file has `.json` extension
4. Run application as Administrator

### Monitors Not Appearing

**Symptom:** Virtual monitors don't show in Display Settings

**Solutions:**
1. Check Event Viewer for driver errors
2. Verify test signing is enabled: `bcdedit | Select-String testsigning`
3. Ensure driver is properly installed: `.\UpdateDriver.ps1`
4. Try default configuration first: `.\IddSampleApp.exe` (no arguments)

### Invalid Position/Overlap Errors

**Symptom:** Error about overlapping monitors

**Solutions:**
1. Draw out monitor layout on paper first
2. Calculate positions: Next monitor X = Previous X + Previous Width
3. Use online monitor layout tools for planning
4. Start with simple side-by-side layouts

---

## ?? Additional Resources

- **JSON Schema:** See `config-schema.json` for full validation rules
- **Template Examples:** Browse `templates/` directory for inspiration
- **Monitor Configuration Guide:** See `MONITOR_CONFIGURATION_GUIDE.md` for driver details
- **Quick Reference:** See `QUICK_REFERENCE.md` for commands

---

## ?? Tips

- **Test Mode Watermark:** You'll see "Test Mode" watermark while using test-signed drivers
- **Windows Display Settings:** After running, open Settings ? Display to configure virtual monitors
- **Multiple Instances:** Can't run multiple instances simultaneously
- **Performance:** More monitors = more system resources used
- **Persistence:** Configuration persists until you run with different config

---

**Questions or Issues?** Check the main README.md or create an issue on GitHub.
