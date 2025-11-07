# ? Configuration Manager Files - Manual Project Integration Required

## ?? **Files Relocated Successfully**

The ConfigurationManager files have been moved to the correct location:

```
? IddSampleDriver\ConfigurationManager.h
? IddSampleDriver\ConfigurationManager.cpp
```

---

## ?? **Manual Steps Required**

The project files are currently open in Visual Studio. You need to:
1. **Close Visual Studio** (or unload the IddSampleDriver project)
2. **Edit the project files** as shown below
3. **Reload the project** in Visual Studio
4. **Rebuild the solution**

---

## ??? **Step 1: Edit IddSampleDriver.vcxproj**

### **File:** `IddSampleDriver\IddSampleDriver.vcxproj`

**Find this section** (around line 101-106):
```xml
<ItemGroup>
  <ClCompile Include="Driver.cpp" />
</ItemGroup>
<ItemGroup>
  <ClInclude Include="Driver.h" />
  <ClInclude Include="Trace.h" />
</ItemGroup>
```

**Replace with:**
```xml
<ItemGroup>
  <ClCompile Include="Driver.cpp" />
  <ClCompile Include="ConfigurationManager.cpp" />
</ItemGroup>
<ItemGroup>
  <ClInclude Include="Driver.h" />
  <ClInclude Include="ConfigurationManager.h" />
  <ClInclude Include="Trace.h" />
</ItemGroup>
```

---

## ??? **Step 2: Edit IddSampleDriver.vcxproj.filters**

### **File:** `IddSampleDriver\IddSampleDriver.vcxproj.filters`

**Find this section** (around line 25-37):
```xml
<ItemGroup>
  <ClInclude Include="Driver.h">
    <Filter>Header Files</Filter>
  </ClInclude>
  <ClInclude Include="Trace.h">
    <Filter>Header Files</Filter>
  </ClInclude>
</ItemGroup>
<ItemGroup>
  <ClCompile Include="Driver.cpp">
    <Filter>Source Files</Filter>
  </ClCompile>
</ItemGroup>
```

**Replace with:**
```xml
<ItemGroup>
  <ClInclude Include="Driver.h">
    <Filter>Header Files</Filter>
  </ClInclude>
  <ClInclude Include="ConfigurationManager.h">
    <Filter>Header Files</Filter>
  </ClInclude>
  <ClInclude Include="Trace.h">
    <Filter>Header Files</Filter>
  </ClInclude>
</ItemGroup>
<ItemGroup>
  <ClCompile Include="Driver.cpp">
    <Filter>Source Files</Filter>
  </ClCompile>
  <ClCompile Include="ConfigurationManager.cpp">
    <Filter>Source Files</Filter>
  </ClCompile>
</ItemGroup>
```

---

## ?? **Alternative: Use Visual Studio UI**

If you prefer to use Visual Studio instead of editing XML:

1. **In Solution Explorer:**
   - Right-click on **IddSampleDriver** project
   - Select **"Add" ? "Existing Item..."**

2. **Add ConfigurationManager.h:**
   - Navigate to `IddSampleDriver` folder
   - Select `ConfigurationManager.h`
   - Click **"Add"**

3. **Add ConfigurationManager.cpp:**
   - Right-click on **IddSampleDriver** project again
   - Select **"Add" ? "Existing Item..."**
   - Select `ConfigurationManager.cpp`
   - Click **"Add"**

4. **Verify in Solution Explorer:**
   ```
   IddSampleDriver
   ??? Header Files
   ?   ??? Driver.h
   ?   ??? ConfigurationManager.h ? Should appear here
   ?   ??? Trace.h
   ??? Source Files
       ??? Driver.cpp
??? ConfigurationManager.cpp   ? Should appear here
   ```

---

## ?? **Verification Checklist**

After making changes:

- [ ] `ConfigurationManager.h` appears under "Header Files" in Solution Explorer
- [ ] `ConfigurationManager.cpp` appears under "Source Files" in Solution Explorer
- [ ] Project builds without errors (`Build ? Rebuild Solution`)
- [ ] No linker errors about ConfigurationManager
- [ ] Both files compile (check build output)

---

## ?? **Build the Project**

After adding the files:

```powershell
# In Visual Studio
Build ? Rebuild Solution (Ctrl+Shift+B)

# Or from PowerShell
msbuild IddSampleDriver\IddSampleDriver.vcxproj /p:Configuration=Debug /p:Platform=x64 /t:Rebuild
```

**Expected output:**
```
1>------ Build started: Project: IddSampleDriver, Configuration: Debug x64 ------
1>Driver.cpp
1>ConfigurationManager.cpp
1>   Creating library x64\Debug\IddSampleDriver\IddSampleDriver.lib
1>IddSampleDriver.vcxproj -> D:\...\x64\Debug\IddSampleDriver\IddSampleDriver.dll
========== Build: 1 succeeded, 0 failed, 0 up-to-date, 0 skipped ==========
```

---

## ?? **Test After Build**

Once the build succeeds:

```powershell
# Update the driver
.\UpdateDriver.ps1

# Test with a template
.\RunDriver.ps1 --template dual-1080p

# Verify monitors
.\CheckDisplays.ps1
```

---

## ?? **Potential Build Issues**

### **Issue: "Cannot open include file: 'ConfigurationManager.h'"**

**Solution:**
- Verify `ConfigurationManager.h` is in `IddSampleDriver\` directory
- Check it's added to the .vcxproj file
- Rebuild the solution

### **Issue: Linker errors about ConfigurationManager**

**Solution:**
- Ensure `ConfigurationManager.cpp` is added to the project
- Check that it's being compiled (should appear in build output)
- Rebuild the solution

### **Issue: STL header errors**

**Solution:**
- The project should already have C++14 support
- These headers should work: `<fstream>`, `<sstream>`, `<algorithm>`, `<set>`
- If not, check project properties ? C/C++ ? Language Standard

---

## ?? **What These Files Provide**

**ConfigurationManager.h:**
- `MonitorConfig` struct - Stores monitor parameters
- `ConfigurationManager` class - Loads, parses, validates configs
- Static methods for file/template loading
- Error handling with descriptive messages

**ConfigurationManager.cpp:**
- Lightweight JSON parser (no external dependencies)
- Configuration validation logic
- Template discovery
- File I/O for JSON configs
- Overlap detection
- Default configuration fallback

---

## ? **Once Complete**

After successfully integrating and building:

? **Driver will support:**
- Dynamic configuration from JSON files
- Template-based monitor layouts
- Custom resolutions and refresh rates
- Up to 10 virtual monitors
- Comprehensive error validation

? **You can use:**
```powershell
.\RunDriver.ps1 --template dual-1080p
.\RunDriver.ps1 --template 2x2-grid
.\RunDriver.ps1 --config C:\my-config.json
.\RunDriver.ps1 --list-templates
```

---

## ?? **Project Integration Status**

| Component | Status | Notes |
|-----------|--------|-------|
| ConfigurationManager.h | ? Created & Moved | Ready to add to project |
| ConfigurationManager.cpp | ? Created & Moved | Ready to add to project |
| Template files | ? Created | 4 templates in `templates/` |
| JSON schema | ? Created | `config-schema.json` |
| IddSampleApp CLI | ? Complete | Supports all config options |
| RunDriver.ps1 | ? Enhanced | Forwards config arguments |

**Remaining:** Add ConfigurationManager files to IddSampleDriver.vcxproj

---

**Follow the steps above, then rebuild to complete the integration!** ??
