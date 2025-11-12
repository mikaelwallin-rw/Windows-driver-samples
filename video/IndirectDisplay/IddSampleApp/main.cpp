#include <windows.h>

#include <fstream>
#include <iostream>
#include <string>
#include <vector>

#include <conio.h>
#include <swdevice.h>
#include <wrl.h>

// Include the driver's configuration management classes
#include "../IddSampleDriver/ConfigurationManager.h"
#include "../IddSampleDriver/TemplateManager.h"

using namespace Microsoft::IndirectDisp;

// Configuration file path to pass to driver via registry
const wchar_t* CONFIG_REGISTRY_KEY = L"SOFTWARE\\IddSampleDriver";
const wchar_t* CONFIG_VALUE_NAME = L"ConfigPath";

void PrintUsage()
{
    std::wcout << L"IddSampleDriver - Virtual Display Driver\n\n";
    std::wcout << L"Usage:\n";
    std::wcout << L"  IddSampleApp.exe [options]\n\n";
    std::wcout << L"Options:\n";
    std::wcout << L"  --config <path>        Load monitor configuration from "
                  L"JSON file\n";
    std::wcout << L"  --template <name>      Load predefined template (e.g., "
                  L"dual-1080p, 2x2-grid)\n";
    std::wcout << L"  --list-templates       List available configuration templates\n";
    std::wcout << L"  --help        Show this help message\n\n";
    std::wcout << L"Examples:\n";
    std::wcout << L"  IddSampleApp.exe --config \"C:\\monitors.json\"\n";
    std::wcout << L"  IddSampleApp.exe --template dual-1080p\n";
    std::wcout << L"  IddSampleApp.exe --list-templates\n\n";
}

void ListTemplates()
{
    std::wcout << L"Available templates:\n\n";

    // Use TemplateManager to list templates
    TemplateManager templateMgr;
    std::vector<TemplateInfo> templates = templateMgr.ListTemplates();

    if (templates.empty())
    {
        std::wcout << L"  (No templates found)\n";
        std::wcout << L"  Templates directory: " << templateMgr.GetTemplatesDirectory() << L"\n";
        return;
    }

    for (const auto& info : templates)
    {
        std::wcout << L"  " << info.name << L"\n";
        std::wcout << L"    " << info.description << L"\n";
    }

    std::wcout << L"\n";
}

bool SetConfigPath(const std::wstring& configPath)
{
    // Read the JSON content from the file
    std::ifstream file(configPath);
    if (!file.is_open())
    {
        std::wcerr << L"Error: Unable to open configuration file: " << configPath << L"\n";
        return false;
    }

    std::string jsonContent((std::istreambuf_iterator<char>(file)), std::istreambuf_iterator<char>());
    file.close();

    if (jsonContent.empty())
    {
        std::wcerr << L"Error: Configuration file is empty: " << configPath << L"\n";
        return false;
    }

    std::wcout << L"JSON content read from file:\n" << jsonContent.c_str() << L"\n\n";

    // Convert to wide string for registry
    int wideSize = MultiByteToWideChar(CP_UTF8, 0, jsonContent.c_str(), -1, nullptr, 0);
    if (wideSize == 0)
    {
        std::wcerr << L"Error: Failed to convert JSON to wide string\n";
        return false;
    }

    std::vector<wchar_t> wideJson(wideSize);
    MultiByteToWideChar(CP_UTF8, 0, jsonContent.c_str(), -1, wideJson.data(), wideSize);

    HKEY hKey;
    // Use KEY_WOW64_64KEY to explicitly write to 64-bit registry view
    LONG result = RegCreateKeyExW(HKEY_LOCAL_MACHINE, CONFIG_REGISTRY_KEY, 0, nullptr, REG_OPTION_NON_VOLATILE,
                                  KEY_WRITE | KEY_WOW64_64KEY, nullptr, &hKey, nullptr);

    if (result != ERROR_SUCCESS)
    {
        std::wcerr << L"Error: Failed to create registry key (error " << result << L")\n";
        std::wcerr << L"Note: You may need to run as Administrator\n";
        return false;
    }

    // Store the JSON content directly in the registry instead of file path
    result = RegSetValueExW(hKey, L"ConfigJSON", 0, REG_SZ, reinterpret_cast<const BYTE*>(wideJson.data()),
                            static_cast<DWORD>(wideJson.size() * sizeof(wchar_t)));

    if (result != ERROR_SUCCESS)
    {
        RegCloseKey(hKey);
        std::wcerr << L"Error: Failed to set registry value (error " << result << L")\n";
        return false;
    }

    // Clear the old ConfigPath value if it exists
    RegDeleteValueW(hKey, CONFIG_VALUE_NAME);

    // Flush the registry to ensure the value is written before the driver reads it
    RegFlushKey(hKey);
    RegCloseKey(hKey);

    // Small delay to ensure registry value is available to the driver
    Sleep(100);

    std::wcout << L"Configuration JSON stored in registry (" << jsonContent.size() << L" bytes)\n";

    return true;
}

bool ValidateConfigFile(const std::wstring& configPath)
{
    // Check if file exists
    DWORD attr = GetFileAttributesW(configPath.c_str());
    if (attr == INVALID_FILE_ATTRIBUTES)
    {
        std::wcerr << L"Error: Configuration file not found: " << configPath << L"\n";
        return false;
    }

    if (attr & FILE_ATTRIBUTE_DIRECTORY)
    {
        std::wcerr << L"Error: Path is a directory, not a file: " << configPath << L"\n";
        return false;
    }

    // Basic validation - check if it's valid JSON (simplified)
    std::ifstream file(configPath);
    if (!file.is_open())
    {
        std::wcerr << L"Error: Unable to open configuration file: " << configPath << L"\n";
        return false;
    }

    std::string content((std::istreambuf_iterator<char>(file)), std::istreambuf_iterator<char>());

    if (content.find('[') == std::string::npos || content.find(']') == std::string::npos)
    {
        std::wcerr << L"Error: Invalid JSON format - expected array of monitors\n";
        return false;
    }

    if (content.find("\"id\"") == std::string::npos || content.find("\"width\"") == std::string::npos ||
        content.find("\"height\"") == std::string::npos)
    {
        std::wcerr << L"Error: Configuration missing required fields (id, width, "
                      L"height)\n";
        return false;
    }

    std::wcout << L"Configuration file validated successfully\n";
    return true;
}

VOID WINAPI CreationCallback(_In_ HSWDEVICE hSwDevice, _In_ HRESULT hrCreateResult, _In_opt_ PVOID pContext,
                             _In_opt_ PCWSTR pszDeviceInstanceId)
{
    HANDLE hEvent = *(HANDLE*)pContext;

    SetEvent(hEvent);
    UNREFERENCED_PARAMETER(hSwDevice);
    UNREFERENCED_PARAMETER(hrCreateResult);
    UNREFERENCED_PARAMETER(pszDeviceInstanceId);
}

int __cdecl wmain(int argc, wchar_t* argv[])
{
    // Parse command line arguments
    bool showHelp = false;
    bool listTemplates = false;
    std::wstring configPath;
    std::wstring templateName;

    for (int i = 1; i < argc; i++)
    {
        std::wstring arg = argv[i];

        if (arg == L"--help" || arg == L"-h" || arg == L"/?")
        {
            showHelp = true;
        }
        else if (arg == L"--list-templates")
        {
            listTemplates = true;
        }
        else if (arg == L"--config" && i + 1 < argc)
        {
            configPath = argv[++i];
        }
        else if (arg == L"--template" && i + 1 < argc)
        {
            templateName = argv[++i];
        }
        else
        {
            std::wcerr << L"Error: Unknown argument: " << arg << L"\n\n";
            PrintUsage();
            return 1;
        }
    }

    if (showHelp)
    {
        PrintUsage();
        return 0;
    }

    if (listTemplates)
    {
        ListTemplates();
        return 0;
    }

    // Handle configuration
    if (!configPath.empty() && !templateName.empty())
    {
        std::wcerr << L"Error: Cannot specify both --config and --template\n\n";
        PrintUsage();
        return 1;
    }

    if (!templateName.empty())
    {
        // Use TemplateManager to find template
        TemplateManager templateMgr;
        configPath = templateMgr.GetTemplatePath(templateName);

        if (configPath.empty())
        {
            std::wcerr << L"Error: Template not found: " << templateName << L"\n";
            std::wcerr << L"Use --list-templates to see available templates\n";
            return 1;
        }

        std::wcout << L"Using template: " << templateName << L"\n";
    }

    // Validate and set config path if provided
    if (!configPath.empty())
    {
        if (!ValidateConfigFile(configPath))
        {
            return 1;
        }

        if (!SetConfigPath(configPath))
        {
            return 1;
        }

        std::wcout << L"Configuration set: " << configPath << L"\n\n";
    }
    else
    {
        std::wcout << L"Using default configuration (4 monitors)\n";
        std::wcout << L"Tip: Use --help to see configuration options\n\n";
    }

    // Original device creation code
    HANDLE hEvent = CreateEvent(nullptr, FALSE, FALSE, nullptr);
    HSWDEVICE hSwDevice = nullptr;
    SW_DEVICE_CREATE_INFO createInfo = {0};
    PCWSTR description = L"Idd Sample Driver";

    // These match the Pnp id's in the inf file so OS will load the driver when
    // the device is created
    PCWSTR instanceId = L"IddSampleDriver";
    PCWSTR hardwareIds = L"IddSampleDriver\0\0";
    PCWSTR compatibleIds = L"IddSampleDriver\0\0";

    createInfo.cbSize = sizeof(createInfo);
    createInfo.pszzCompatibleIds = compatibleIds;
    createInfo.pszInstanceId = instanceId;
    createInfo.pszzHardwareIds = hardwareIds;
    createInfo.pszDeviceDescription = description;

    createInfo.CapabilityFlags = SWDeviceCapabilitiesRemovable | SWDeviceCapabilitiesSilentInstall |
                                 SWDeviceCapabilitiesDriverRequired;

    // Create the device (with retry logic for cleanup delays)
    std::wcout << L"Creating virtual display device...\n";
    HRESULT hr = E_FAIL;
    int retryCount = 0;
    const int maxRetries = 5;

    while (retryCount < maxRetries)
    {
        hr = SwDeviceCreate(L"IddSampleDriver", L"HTREE\\ROOT\\0", &createInfo, 0, nullptr, CreationCallback, &hEvent,
                            &hSwDevice);

        if (SUCCEEDED(hr))
        {
            break;
        }

        // Error 0x800700b7 = "Cannot create a file when that file already exists"
        // This happens when Windows hasn't finished cleaning up the previous instance
        if (hr == HRESULT_FROM_WIN32(ERROR_ALREADY_EXISTS))
        {
            retryCount++;
            if (retryCount < maxRetries)
            {
                std::wcout << L"  Device cleanup in progress, retrying in 1 second... (" << retryCount << L"/"
                           << maxRetries << L")\n";
                Sleep(1000);
                continue;
            }
        }

        // Other error or max retries reached
        std::wcerr << L"Error: SwDeviceCreate failed with 0x" << std::hex << hr << L"\n";
        if (hr == HRESULT_FROM_WIN32(ERROR_ALREADY_EXISTS))
        {
            std::wcerr << L"The device already exists. Try waiting a few seconds and running again.\n";
            std::wcerr << L"Or use: pnputil /remove-device \"SWD\\IddSampleDriver\\IddSampleDriver\"\n";
        }
        return 1;
    }

    // Wait for callback to signal that the device has been created
    DWORD waitResult = WaitForSingleObject(hEvent, 10 * 1000);
    if (waitResult != WAIT_OBJECT_0)
    {
        std::wcerr << L"Error: Wait for device creation failed\n";
        return 1;
    }
    std::wcout << L"? Virtual display device created successfully\n\n";

    // Now wait for user to indicate the device should be stopped
    std::wcout << L"Virtual monitors are now active\n";
    std::wcout << L"Check Display Settings (Win+P) to configure them\n\n";
    std::wcout << L"Press 'x' to exit and remove virtual displays\n";

    bool bExit = false;
    do
    {
        // Wait for key press
        int key = _getch();

        if (key == 'x' || key == 'X')
        {
            bExit = true;
        }
    } while (!bExit);

    // Stop the device, this will cause the sample to be unloaded
    std::wcout << L"\nRemoving virtual displays...\n";
    SwDeviceClose(hSwDevice);
    std::wcout << L"? Virtual displays removed\n";

    return 0;
}