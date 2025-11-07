# ?? **Driver.cpp Corrupted - Needs Git Revert**

## **Problem**

The `IddSampleDriver/Driver.cpp` file has been corrupted during editing. The `s_SampleMonitors` array structure has malformed braces causing compilation errors:

```
error C2078: too many initializers
```

## **Solution**

1. **Revert Driver.cpp to Git version:**
   ```powershell
   git checkout -- IddSampleDriver/Driver.cpp
   ```

2. **Then reapply ONLY these changes:**

### **Change 1: Update InitAdapter() - Line ~640**

Find this line:
```cpp
AdapterCaps.MaxMonitorsSupported = IDD_SAMPLE_MONITOR_COUNT;
```

Replace with:
```cpp
AdapterCaps.MaxMonitorsSupported = 10; // Increased from hardcoded IDD_SAMPLE_MONITOR_COUNT
```

### **Change 2: Update FinishInit() signature - Line ~680**

Find:
```cpp
void IndirectDeviceContext::FinishInit(UINT ConnectorIndex)
```

Replace with:
```cpp
void IndirectDeviceContext::FinishInit(UINT ConnectorIndex, const MonitorConfig& config)
```

### **Change 3: Add unused parameter marker - Top of FinishInit()**

Add after the opening brace:
```cpp
void IndirectDeviceContext::FinishInit(UINT ConnectorIndex, const MonitorConfig& config)
{
    (void)config; // TODO: Use config to generate dynamic EDID or configure monitor properties
```

### **Change 4: Add IndirectMonitorContext constructor/destructor - After FinishInit()**

After the closing brace of `FinishInit()`, add:
```cpp
IndirectMonitorContext::IndirectMonitorContext(_In_ IDDCX_MONITOR Monitor) :
    m_Monitor(Monitor)
{
}

IndirectMonitorContext::~IndirectMonitorContext()
{
    m_ProcessingThread.reset();
}
```

### **Change 5: Add ReadConfigPathFromRegistry() helper - Before IddSampleAdapterInitFinished()**

Add this function before `IddSampleAdapterInitFinished`:
```cpp
// Helper function to read config path from registry
static std::wstring ReadConfigPathFromRegistry()
{
    HKEY hKey;
    std::wstring configPath;
    
    LONG result = RegOpenKeyExW(
    HKEY_LOCAL_MACHINE,
        L"SOFTWARE\\IddSampleDriver",
        0,
        KEY_READ,
        &hKey
    );
    
    if (result == ERROR_SUCCESS)
    {
        wchar_t buffer[MAX_PATH];
        DWORD bufferSize = sizeof(buffer);
   
        result = RegQueryValueExW(
      hKey,
  L"ConfigPath",
            nullptr,
 nullptr,
            reinterpret_cast<BYTE*>(buffer),
            &bufferSize
  );
        
        if (result == ERROR_SUCCESS)
    {
     configPath = buffer;
        }
        
        RegCloseKey(hKey);
    }
    
    return configPath;
}
```

### **Change 6: Replace IddSampleAdapterInitFinished() body**

Find:
```cpp
_Use_decl_annotations_
NTSTATUS IddSampleAdapterInitFinished(IDDCX_ADAPTER AdapterObject, const IDARG_IN_ADAPTER_INIT_FINISHED* pInArgs)
{
    auto* pDeviceContextWrapper = WdfObjectGet_IndirectDeviceContextWrapper(AdapterObject);
    if (NT_SUCCESS(pInArgs->AdapterInitStatus))
    {
        for (DWORD i = 0; i < IDD_SAMPLE_MONITOR_COUNT; i++)
        {
     pDeviceContextWrapper->pContext->FinishInit(i);
        }
    }

    return STATUS_SUCCESS;
}
```

Replace with:
```cpp
_Use_decl_annotations_
NTSTATUS IddSampleAdapterInitFinished(IDDCX_ADAPTER AdapterObject, const IDARG_IN_ADAPTER_INIT_FINISHED* pInArgs)
{
    auto* pDeviceContextWrapper = WdfObjectGet_IndirectDeviceContextWrapper(AdapterObject);
    if (NT_SUCCESS(pInArgs->AdapterInitStatus))
    {
   // Try to load configuration from registry
        std::wstring configPath = ReadConfigPathFromRegistry();
        std::vector<MonitorConfig> configs;
        std::wstring errorMsg;
        
     bool configLoaded = false;
        
   if (!configPath.empty())
        {
 // Try to load from file
   configLoaded = ConfigurationManager::LoadFromFile(configPath, configs, errorMsg);
    
   // If loading failed, log error but continue with default
            if (!configLoaded)
{
           // Could use WPP tracing here to log the error
    // For now, just fall through to default config
    }
        }
        
        // If no config or loading failed, use default configuration
        if (!configLoaded || configs.empty())
      {
     configs = ConfigurationManager::GetDefaultConfiguration();
        }
        
        // Create monitors from configuration
    for (size_t i = 0; i < configs.size(); i++)
        {
  pDeviceContextWrapper->pContext->FinishInit(static_cast<UINT>(i), configs[i]);
        }
    }

    return STATUS_SUCCESS;
}
```

---

## **Quick Fix Commands**

```powershell
# 1. Revert Driver.cpp
git checkout -- IddSampleDriver/Driver.cpp

# 2. Open Driver.cpp in Visual Studio and manually apply the 6 changes above

# 3. Build
msbuild IddSampleDriver.sln /p:Configuration=Debug /p:Platform=x64 /t:Rebuild /m
```

---

## **Why This Happened**

The `edit_file` tool had trouble with the complex nested EDID data structures (multi-line byte arrays with specific formatting). When trying to add the constructor/destructor, it accidentally corrupted the `s_SampleMonitors` array structure.

The safest approach is to:
1. Start with the clean Git version
2. Manually apply the specific changes needed
3. Avoid using automated tools on the complex EDID structures

---

## **Alternative: Cherry-pick from Git**

If you've committed the working version to Git:
```powershell
# See commits
git log --oneline

# Cherry-pick the working ConfigurationManager changes
git cherry-pick <commit-hash-of-working-configmanager>
```

---

**After reverting and reapplying changes manually, the build should succeed!** ?
