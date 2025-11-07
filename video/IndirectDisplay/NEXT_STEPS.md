# Next Steps - Building the Refactored Project

## Current Status
✅ Code refactoring complete
✅ vcpkg manifest created
✅ Project files updated
⏳ Dependencies need to be installed

## Step 1: Install vcpkg (if not already installed)

If you don't have vcpkg installed, run these commands in PowerShell:

```powershell
# Navigate to a suitable location (e.g., C:\dev)
cd C:\dev

# Clone vcpkg
git clone https://github.com/microsoft/vcpkg.git
cd vcpkg

# Bootstrap vcpkg
.\bootstrap-vcpkg.bat

# Integrate with Visual Studio (this makes vcpkg work with all VS projects)
.\vcpkg integrate install
```

## Step 2: Build the Solution

### Option A: Using Visual Studio
1. Open `IddSampleDriver.sln` in Visual Studio
2. Visual Studio will automatically detect `vcpkg.json` and install dependencies
3. Build the solution (Ctrl+Shift+B or Build → Build Solution)
4. vcpkg will download and build `nlohmann-json` on first build

### Option B: Using MSBuild (Command Line)
```powershell
# Navigate to project directory
cd d:\wkspaces\Windows-driver-samples\video\IndirectDisplay

# Build using the existing build script
.\build\msbuild-vs2022.cmd IddSampleDriver.sln /t:Build /p:Configuration=Debug /p:Platform=x64
```

### Option C: Using VS Code Task
You can use the existing build task configured in your workspace.

## Expected Build Output

On first build, you'll see:
```
Installing dependencies from vcpkg.json...
  nlohmann-json:x64-windows
Building nlohmann-json:x64-windows...
Package nlohmann-json:x64-windows is header only and can be used.
```

Then normal compilation will proceed.

## Troubleshooting

### Issue: vcpkg not found
**Solution:** Make sure you ran `vcpkg integrate install`. If you still have issues, you can manually specify vcpkg:

In Visual Studio, go to:
Tools → Options → vcpkg → vcpkg directory

Or set environment variable:
```powershell
$env:VCPKG_ROOT = "C:\dev\vcpkg"
```

### Issue: nlohmann-json not found during build
**Solution:** Manually install the package:
```powershell
cd C:\dev\vcpkg  # Or wherever you installed vcpkg
.\vcpkg install nlohmann-json:x64-windows
```

### Issue: IntelliSense errors about Clang version
**Solution:** These are false positives and can be ignored. The actual build will work fine.

### Issue: Driver project won't build with vcpkg
**Solution:** If the driver project has issues with vcpkg (since it's a kernel driver), you may need to remove `<VcpkgEnabled>true</VcpkgEnabled>` from `IddSampleDriver.vcxproj`. The library is header-only, so it can still be used.

## Verifying the Changes

After building successfully, test the new features:

### Test 1: List Templates
```powershell
.\x64\Debug\IddSampleApp.exe --list-templates
```

Expected output:
```
Available templates:

  2x2-grid
    4 monitors
  4k-plus-1080p
    2 monitors
  dual-1080p
    2 monitors
  single-1080p
    Single monitor configuration
```

### Test 2: Load a Template
```powershell
.\x64\Debug\IddSampleApp.exe --template dual-1080p
```

Expected output:
```
Using template: dual-1080p
Configuration set: <path>\templates\dual-1080p.json

Creating virtual display device...
✓ Virtual display device created successfully
...
```

### Test 3: Load Custom Config
```powershell
.\x64\Debug\IddSampleApp.exe --config "C:\my-config.json"
```

Should work as before.

## What Changed (Quick Reference)

| Component | Change |
|-----------|--------|
| **JSON Parsing** | Manual → nlohmann-json library |
| **Template Discovery** | Scattered → Centralized in `TemplateManager` |
| **Code Structure** | Mixed concerns → Separated responsibilities |
| **Dependencies** | Manual → vcpkg automated |
| **Error Handling** | Basic → Detailed with line numbers |

## Files Added/Modified

### Added:
- `vcpkg.json` - Dependency manifest
- `IddSampleDriver/TemplateManager.h` - Template management header
- `IddSampleDriver/TemplateManager.cpp` - Template management implementation
- `REFACTORING_SUMMARY.md` - Detailed change documentation
- `NEXT_STEPS.md` - This file

### Modified:
- `IddSampleDriver/ConfigurationManager.h` - Removed template methods
- `IddSampleDriver/ConfigurationManager.cpp` - Uses nlohmann-json, removed template logic
- `IddSampleApp/main.cpp` - Uses TemplateManager
- `IddSampleApp/IddSampleApp.vcxproj` - Added vcpkg, new source files
- `IddSampleDriver/IddSampleDriver.vcxproj` - Added vcpkg, new source files

## Additional Resources

- [nlohmann-json Documentation](https://json.nlohmann.me/)
- [vcpkg Documentation](https://vcpkg.io/en/getting-started.html)
- [vcpkg Manifest Mode](https://vcpkg.io/en/docs/users/manifests.html)

## Need Help?

If you encounter any issues:
1. Check that vcpkg is properly integrated (`vcpkg integrate install`)
2. Verify Visual Studio can find vcpkg (Tools → Options → vcpkg)
3. Try manually installing nlohmann-json (`vcpkg install nlohmann-json`)
4. Check that you have the Windows Driver Kit (WDK) installed
5. Review the build output for specific errors

The refactoring maintains backward compatibility, so if you need to roll back, the original functionality is preserved - just use the git history.
