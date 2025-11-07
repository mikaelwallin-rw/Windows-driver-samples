# ? RunDriver.ps1 Enhancement - Complete

## ?? **Changes Made**

`RunDriver.ps1` has been updated to support configuration arguments, making it the primary and most convenient way to launch the virtual display driver with different configurations.

---

## ?? **New Features**

### **1. Configuration Argument Pass-Through**

All command-line arguments are now forwarded to `IddSampleApp.exe`:

```powershell
.\RunDriver.ps1 --template dual-1080p
.\RunDriver.ps1 --config C:\monitors.json
.\RunDriver.ps1 --list-templates
.\RunDriver.ps1 --help
```

### **2. Built-in Help**

```powershell
PS> .\RunDriver.ps1 --help

Usage: RunDriver.ps1 [options]

Options:
  --config <path>        Load monitor configuration from JSON file
  --template <name>      Load predefined template (e.g., dual-1080p, 2x2-grid)
  --list-templates       List available configuration templates
  --help               Show this help message

Examples:
  .\RunDriver.ps1       # Use default 4-monitor configuration
  .\RunDriver.ps1 --template dual-1080p    # Use dual 1080p template
  .\RunDriver.ps1 --config C:\monitors.json  # Use custom configuration
  .\RunDriver.ps1 --list-templates         # List available templates
```

### **3. Configuration Display**

The script now shows what configuration is being used:

```powershell
PS> .\RunDriver.ps1 --template dual-1080p

Starting IDD Sample Driver...

? Test signing is ENABLED

Using template: dual-1080p

Running IddSampleApp.exe from: D:\...\x64\Debug\IddSampleApp.exe
The app will create virtual monitor(s).
Check Display Settings (Win+P) to see them.
```

### **4. Template Listing**

```powershell
PS> .\RunDriver.ps1 --list-templates

Available templates:

  2x2-grid
    (4 monitor(s))
  4k-plus-1080p
  (2 monitor(s))
dual-1080p
    (2 monitor(s))
  single-1080p
    (1 monitor(s))
```

---

## ?? **Updated Workflow**

### **Before (Old Way)**

```powershell
# Had to navigate to build directory
cd x64\Debug
.\IddSampleApp.exe --template dual-1080p
```

### **After (New Way)**

```powershell
# Can run from project root with all features
.\RunDriver.ps1 --template dual-1080p
```

---

## ?? **Complete Usage Examples**

### **1. Default Configuration**

```powershell
# Run with default 4-monitor setup
.\RunDriver.ps1
```

**Output:**
```
Starting IDD Sample Driver...
? Test signing is ENABLED

Using default configuration (4 monitors)
Tip: Use --help to see configuration options

Running IddSampleApp.exe...
```

### **2. Using Templates**

```powershell
# Single monitor
.\RunDriver.ps1 --template single-1080p

# Dual monitors
.\RunDriver.ps1 --template dual-1080p

# 2x2 grid (4 monitors)
.\RunDriver.ps1 --template 2x2-grid

# 4K + 1080p combo
.\RunDriver.ps1 --template 4k-plus-1080p
```

### **3. Custom Configuration**

```powershell
# Load from file
.\RunDriver.ps1 --config "C:\my-monitors.json"

# Relative path
.\RunDriver.ps1 --config ".\configs\triple-portrait.json"
```

### **4. Discovery**

```powershell
# List all available templates
.\RunDriver.ps1 --list-templates

# Show help
.\RunDriver.ps1 --help
```

---

## ?? **Features Added to Script**

| Feature | Description | Example |
|---------|-------------|---------|
| **Argument Forwarding** | Passes all args to IddSampleApp.exe | `--template dual-1080p` |
| **Help Display** | Shows usage without running app | `--help` |
| **Template Listing** | Lists templates without running app | `--list-templates` |
| **Config Info Display** | Shows active configuration | Displays "Using template: X" |
| **Default Notification** | Informs about default config | Shows when no args provided |
| **Tip Display** | Helpful hints for users | Shows --help tip |

---

## ? **Validation**

### **Script Features**

- ? Administrator check (existing)
- ? Test signing check (existing)
- ? Executable existence check (existing)
- ? Argument pass-through (NEW)
- ? Help display (NEW)
- ? Template listing (NEW)
- ? Configuration feedback (NEW)

### **Compatibility**

- ? Works with existing `IddSampleApp.exe`
- ? Backward compatible (no args = default behavior)
- ? All original checks preserved
- ? Colorized output maintained
- ? Exit codes preserved

---

## ?? **Updated Documentation**

The following files have been updated to reflect the new workflow:

- ? `RunDriver.ps1` - Script enhanced with argument support
- ? `DYNAMIC_CONFIG_GUIDE.md` - Updated examples to use `RunDriver.ps1`
- ? `RUNDRIVER_ENHANCEMENT.md` - This summary document

---

## ?? **Benefits**

### **For Users:**

1. **Single Command** - No need to navigate to build directory
2. **Consistent Interface** - Same script for all operations
3. **Built-in Help** - `--help` shows all options
4. **Discovery** - `--list-templates` shows what's available
5. **Visual Feedback** - See what configuration is being used

### **For Development:**

1. **Easier Testing** - Quick switching between configurations
2. **Better UX** - Users don't need to know internal paths
3. **Extensible** - Easy to add more options in future
4. **Documented** - Help built into the script

---

## ?? **Migration Guide**

### **If You Were Using IddSampleApp.exe Directly:**

**Old:**
```powershell
cd x64\Debug
.\IddSampleApp.exe --template dual-1080p
```

**New:**
```powershell
.\RunDriver.ps1 --template dual-1080p
```

### **If You Were Using RunDriver.ps1:**

**Still Works:**
```powershell
.\RunDriver.ps1  # Default configuration
```

**Now Also Works:**
```powershell
.\RunDriver.ps1 --template dual-1080p
.\RunDriver.ps1 --config C:\config.json
.\RunDriver.ps1 --list-templates
```

---

## ?? **Tips**

### **Creating Shortcuts**

You can now create desktop shortcuts with different configurations:

**Shortcut 1 - Single Monitor:**
```
Target: powershell -ExecutionPolicy Bypass -File "D:\...\RunDriver.ps1" -template single-1080p
Run as: Administrator
```

**Shortcut 2 - Dual Monitors:**
```
Target: powershell -ExecutionPolicy Bypass -File "D:\...\RunDriver.ps1" -template dual-1080p
Run as: Administrator
```

**Shortcut 3 - 2x2 Grid:**
```
Target: powershell -ExecutionPolicy Bypass -File "D:\...\RunDriver.ps1" -template 2x2-grid
Run as: Administrator
```

### **Batch Scripts**

Create batch files for quick access:

**single-monitor.bat:**
```batch
@echo off
powershell -ExecutionPolicy Bypass -Command "cd '%~dp0'; .\RunDriver.ps1 --template single-1080p"
```

**dual-monitor.bat:**
```batch
@echo off
powershell -ExecutionPolicy Bypass -Command "cd '%~dp0'; .\RunDriver.ps1 --template dual-1080p"
```

---

## ?? **Technical Details**

### **Argument Handling**

The script uses PowerShell's `$args` automatic variable:

```powershell
# Check for specific flags
if ($args -contains "--help") { ... }
if ($args -contains "--list-templates") { ... }

# Forward all arguments
if ($args.Count -gt 0) {
    & $appPath $args
}
```

### **Exit Handling**

- `--help` exits immediately (exit 0)
- `--list-templates` exits immediately (exit 0)
- Configuration errors from app propagate through
- Script maintains admin check before forwarding

---

## ?? **Before & After Comparison**

| Aspect | Before | After |
|--------|--------|-------|
| **Invocation** | `cd x64\Debug; .\IddSampleApp.exe --template X` | `.\RunDriver.ps1 --template X` |
| **Help** | Run app and read docs | `.\RunDriver.ps1 --help` |
| **Templates** | Check docs or browse files | `.\RunDriver.ps1 --list-templates` |
| **Feedback** | None | Shows active configuration |
| **Location** | Must be in build directory | Run from project root |

---

## ? **Status: Complete & Ready to Use**

The `RunDriver.ps1` script is now the **recommended way** to launch the virtual display driver with any configuration. It provides:

- ? All functionality of direct `IddSampleApp.exe` usage
- ? Additional convenience features (help, listing)
- ? Better user experience (feedback, tips)
- ? Simplified workflow (one command from anywhere)
- ? Backward compatible (old usage still works)

**You can now use `RunDriver.ps1` for everything!** ??
