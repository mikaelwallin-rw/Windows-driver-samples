# Copilot Instructions for IddSampleDriver Project

## Project Overview

This is a Windows Indirect Display Driver (IDD) sample that creates virtual monitors using the Windows Driver Framework (WDF) and Indirect Display Driver Class Extension (IddCx). The driver has been enhanced with a dynamic configuration system that allows JSON-based monitor configuration without recompiling.

**Language**: C++14  
**Platform**: Windows 10/11, User Mode Driver Framework (UMDF)  
**Architecture**: x64  
**Build System**: MSBuild / Visual Studio 2022

---

## Project Structure

```
IndirectDisplay/
??? IddSampleDriver/          # Main driver DLL
?   ??? Driver.cpp       # Core driver implementation
?   ??? Driver.h          # Driver interfaces and types
?   ??? ConfigurationManager.cpp  # JSON configuration loader
? ??? ConfigurationManager.h    # Configuration types and API
?   ??? Trace.h        # WPP tracing macros
?   ??? *.inf         # Driver installation files
??? IddSampleApp/ # User-mode test application
?   ??? main.cpp     # Sample app entry point
??? templates/     # JSON configuration templates
    ??? default-4monitors.json    # Default 4-monitor setup
 ??? dual-1080p.json      # 2x 1920x1080 @ 60Hz
    ??? triple-monitor.json      # 3-monitor setup
    ??? single-4k.json           # Single 3840x2160 @ 60Hz
```

---

## Key Architecture Concepts

### 1. **Indirect Display Driver Model**

The driver uses Windows IddCx (Indirect Display Driver Class Extension) which provides:
- Virtual monitor creation without physical hardware
- Integration with Windows Display subsystem
- EDID (Extended Display Identification Data) handling
- Swap chain management for frame processing

### 2. **Core Components**

#### **IndirectDeviceContext**
- Represents the display adapter device
- Manages adapter initialization and capabilities
- Creates and manages monitor instances
- **Key Method**: `InitAdapter()` - Sets max monitors to 10
- **Key Method**: `FinishInit()` - Creates individual monitor with config

#### **IndirectMonitorContext**
- Represents a single virtual monitor
- Manages swap chain for frame processing
- Handles monitor arrival/removal events
- **Key Members**:
  - `m_Monitor` - IddCx monitor handle
  - `m_ProcessingThread` - Frame processing thread

#### **ConfigurationManager** (New)
- Loads monitor configurations from JSON files
- Validates monitor settings (resolution, refresh rate, position)
- Provides default configuration fallback
- **Static Methods**:
  - `LoadFromFile()` - Load config from JSON file
  - `LoadFromTemplate()` - Load from templates directory
  - `GetDefaultConfiguration()` - Returns 4-monitor default

#### **SwapChainProcessor**
- Processes frames from the Windows compositor
- Runs on dedicated thread for performance
- Acquires and releases display buffers
- **Critical Path**: Minimize work in `RunCore()`

### 3. **Configuration System**

#### **MonitorConfig Structure**
```cpp
struct MonitorConfig {
    std::string id;       // Unique monitor identifier
    DWORD width;       // Resolution width (640-7680)
    DWORD height;             // Resolution height (480-4320)
    DWORD refreshRate;        // Refresh rate (60, 75, 90, 120, 144, 240)
    LONG positionX;         // Screen position X
    LONG positionY;    // Screen position Y
  bool primary;         // Is primary monitor
};
```

#### **Configuration Loading Flow**
1. `IddSampleAdapterInitFinished()` called by IddCx
2. `ReadConfigPathFromRegistry()` reads registry key
3. `ConfigurationManager::LoadFromFile()` parses JSON
4. Falls back to `GetDefaultConfiguration()` if load fails
5. Calls `FinishInit()` for each monitor with config

#### **Registry Configuration**
```
HKEY_LOCAL_MACHINE\SOFTWARE\IddSampleDriver
  ConfigPath = "C:\path\to\config.json"
```

---

## Critical Code Patterns

### 1. **EDID Handling**

**Static EDID Blocks**: The driver includes 4 hardcoded EDID blocks for sample monitors in `s_SampleMonitors[]`. These are 128-byte arrays containing:
- Manufacturer ID, model, serial
- Supported resolutions and refresh rates
- Display capabilities

**Dynamic Monitors**: For monitors beyond the first 4, the driver uses "EDID-less mode":
```cpp
if (ConnectorIndex < ARRAYSIZE(s_SampleMonitors)) {
    // Use hardcoded EDID
    MonitorInfo.MonitorDescription.pData = s_SampleMonitors[ConnectorIndex].pEdidBlock;
} else {
    // EDID-less mode - OS will use default modes
    MonitorInfo.MonitorDescription.DataSize = 0;
    MonitorInfo.MonitorDescription.pData = nullptr;
}
```

### 2. **Error Handling**

**NTSTATUS**: Kernel-mode status codes
- `NT_SUCCESS(status)` - Check for success
- `STATUS_SUCCESS` - Operation succeeded
- Return appropriate NTSTATUS from callbacks

**HRESULT**: COM-style errors
- `SUCCEEDED(hr)` / `FAILED(hr)` - Check result
- `S_OK` - Success
- Use for DirectX operations

### 3. **Memory Management**

**WDF Object Lifetime**: Objects are managed by WDF framework
```cpp
WDF_OBJECT_ATTRIBUTES_INIT_CONTEXT_TYPE(&Attr, IndirectDeviceContextWrapper);
Attr.EvtCleanupCallback = [](WDFOBJECT Object) {
    auto* pContext = WdfObjectGet_IndirectDeviceContextWrapper(Object);
    pContext->Cleanup();
};
```

**Smart Pointers**: Use for C++ objects
- `std::unique_ptr<>` - Exclusive ownership
- `std::shared_ptr<>` - Shared ownership
- `ComPtr<>` (WRL) - COM interface pointers

### 4. **Threading**

**Swap Chain Thread**: Each monitor has dedicated frame processing thread
- Created in `SwapChainProcessor` constructor
- Runs at elevated priority (Multimedia Class Scheduler)
- Must release frames quickly to avoid compositor stalls

**Synchronization**: Use WDF synchronization primitives
- Events for thread signaling
- Critical sections for shared data

---

## Code Style Guidelines

### 1. **Naming Conventions**

**Classes**: PascalCase
```cpp
class IndirectDeviceContext { };
```

**Methods**: PascalCase
```cpp
void InitAdapter();
```

**Member Variables**: m_PrefixPascalCase
```cpp
IDDCX_ADAPTER m_Adapter;
```

**Parameters**: camelCase or PascalCase for Windows types
```cpp
void FinishInit(UINT ConnectorIndex, const MonitorConfig& config);
```

**Constants**: UPPER_SNAKE_CASE or constexpr PascalCase
```cpp
static constexpr DWORD IDD_SAMPLE_MONITOR_COUNT = 4;
```

### 2. **Indentation and Formatting**

- **Indentation**: 4 spaces (no tabs)
- **Braces**: Opening brace on same line for control structures
- **Line Length**: Aim for 120 characters max
- **Comments**: Use `//` for single-line, `/* */` for multi-line

### 3. **SAL Annotations**

Use Microsoft Source Annotation Language (SAL) for parameters:
```cpp
void FinishInit(_In_ UINT ConnectorIndex, _In_ const MonitorConfig& config);
```

Common annotations:
- `_In_` - Input parameter
- `_Out_` - Output parameter
- `_Inout_` - Input/output parameter
- `_Use_decl_annotations_` - Use annotations from declaration

---

## Common Tasks and Patterns

### 1. **Adding a New Monitor Configuration Field**

**Step 1**: Update `MonitorConfig` structure in `ConfigurationManager.h`:
```cpp
struct MonitorConfig {
    // ...existing fields...
    DWORD brightnessLevel;  // Add new field
};
```

**Step 2**: Update JSON parsing in `ConfigurationManager.cpp`:
```cpp
int brightness;
if (GetIntValue(objectContent, "brightnessLevel", brightness)) {
    config.brightnessLevel = static_cast<DWORD>(brightness);
}
```

**Step 3**: Use in `IndirectDeviceContext::FinishInit()`:
```cpp
void IndirectDeviceContext::FinishInit(UINT ConnectorIndex, const MonitorConfig& config) {
    // Use config.brightnessLevel here
}
```

### 2. **Handling Unreferenced Parameters**

For parameters that will be used later:
```cpp
void SomeFunction(UINT param) {
    (void)param;  // Suppress warning
    // TODO: Implement functionality using param
}
```

### 3. **Validating Configuration Data**

Always validate in `ConfigurationManager::ValidateMonitor()`:
```cpp
if (config.width < 640 || config.width > 7680) {
    outError = L"Invalid width: " + std::to_wstring(config.width);
    return false;
}
```

---

## Build and Debug

### Building

```powershell
# Clean build
msbuild IddSampleDriver.sln /p:Configuration=Debug /p:Platform=x64 /t:Clean
msbuild IddSampleDriver.sln /p:Configuration=Debug /p:Platform=x64 /t:Rebuild /m

# Quick build
msbuild IddSampleDriver.sln /p:Configuration=Debug /p:Platform=x64 /t:Build /m
```

### Common Build Issues

**Issue**: `error C2220: warning treated as error`  
**Solution**: Fix the warning or add `(void)param;` for unused parameters

**Issue**: `error LNK2019: unresolved external symbol`  
**Solution**: Ensure all declared methods have implementations

**Issue**: `error C2078: too many initializers`  
**Solution**: Check EDID array structure - must match exactly 128 bytes

### Debugging

**WPP Tracing**: Enable tracing with:
```cpp
#include "Driver.tmh"  // WPP tracing header
```

**DebugView**: Use Sysinternals DebugView to see OutputDebugString messages

**Event Viewer**: Check Windows Logs ? System for driver errors

---

## Testing

### Manual Testing

1. **Build the driver**
2. **Install the driver**:
   ```powershell
   pnputil /add-driver IddSampleDriver.inf /install
   ```

3. **Start the driver** (run IddSampleApp.exe)

4. **Verify monitors**:
   - Open Display Settings
   - Check for virtual monitors
   - Test resolution changes
   - Verify multi-monitor layout

### Configuration Testing

1. **Default configuration**: No registry key, should create 4 monitors
2. **Custom JSON**: Set registry key, load custom config
3. **Invalid config**: Test validation (bad resolution, overlaps, etc.)
4. **Template loading**: Test template-based configs

---

## Known Limitations

1. **Maximum 10 monitors**: Hard limit in `InitAdapter()`
2. **EDID blocks**: Only first 4 monitors have full EDID, rest use EDID-less mode
3. **Configuration changes**: Require driver restart (app restart)
4. **Refresh rates**: Limited to 60, 75, 90, 120, 144, 240 Hz
5. **No rotation**: Monitor orientation not supported yet

---

## Future Enhancements (TODOs)

1. **Dynamic EDID generation**: Generate EDID from `MonitorConfig`
2. **Hot-plug support**: Add/remove monitors without restart
3. **HDR support**: High Dynamic Range configurations
4. **Custom refresh rates**: Allow arbitrary refresh rates
5. **Monitor rotation**: Portrait/landscape orientation
6. **Performance profiling**: Add telemetry for frame timing

---

## Important Files to Never Edit Directly with AI Tools

These files contain complex, precise data structures that are easily corrupted:

1. **`s_SampleMonitors[]` array** in Driver.cpp
   - Contains 128-byte EDID blocks
   - Must maintain exact byte alignment
   - If editing needed, revert from Git first

2. **`.inf` files** - Driver installation metadata
   - Requires specific formatting
   - Version numbers must be incremented properly

---

## References and Documentation

- [IddCx Documentation](https://docs.microsoft.com/windows-hardware/drivers/display/indirect-display-driver-model-overview)
- [WDF Documentation](https://docs.microsoft.com/windows-hardware/drivers/wdf/)
- [EDID Specification](https://en.wikipedia.org/wiki/Extended_Display_Identification_Data)
- [Display Configuration API](https://docs.microsoft.com/windows/win32/api/wingdi/ns-wingdi-displayconfig_video_signal_info)

---

## Quick Command Reference

```powershell
# Build
msbuild IddSampleDriver.sln /p:Configuration=Debug /p:Platform=x64 /t:Rebuild /m

# Install driver
pnputil /add-driver IddSampleDriver.inf /install

# Remove driver
pnputil /delete-driver oem123.inf /uninstall

# Set config path in registry
reg add "HKLM\SOFTWARE\IddSampleDriver" /v ConfigPath /t REG_SZ /d "C:\configs\my-setup.json" /f

# Check current displays
Get-CimInstance -Namespace root\wmi -ClassName WmiMonitorID
```

---

## Contact and Support

- **GitHub Repository**: https://github.com/microsoft/Windows-driver-samples
- **Issues**: Use GitHub Issues for bugs and feature requests
- **Discussions**: Use GitHub Discussions for questions

---

**Last Updated**: 2025-01-07  
**Driver Version**: 1.0 with Dynamic Configuration Support  
**Maintained By**: Community Contributors
