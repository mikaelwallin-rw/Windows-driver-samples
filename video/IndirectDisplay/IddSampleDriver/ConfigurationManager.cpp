#include "ConfigurationManager.h"

#include <algorithm>
#include <fstream>
#include <set>
#include <sstream>

#include <nlohmann/json.hpp>

using namespace Microsoft::IndirectDisp;
using json = nlohmann::json;

// Helper to convert wstring to string
namespace
{
std::string WStringToString(const std::wstring& wstr)
{
    if (wstr.empty())
        return std::string();
    int size_needed = WideCharToMultiByte(CP_UTF8, 0, &wstr[0], (int)wstr.size(), NULL, 0, NULL, NULL);
    std::string result(size_needed, 0);
    WideCharToMultiByte(CP_UTF8, 0, &wstr[0], (int)wstr.size(), &result[0], size_needed, NULL, NULL);
    return result;
}

std::wstring StringToWString(const std::string& str)
{
    if (str.empty())
        return std::wstring();
    int size_needed = MultiByteToWideChar(CP_UTF8, 0, &str[0], (int)str.size(), NULL, 0);
    std::wstring result(size_needed, 0);
    MultiByteToWideChar(CP_UTF8, 0, &str[0], (int)str.size(), &result[0], size_needed);
    return result;
}

bool ReadFileContent(const std::wstring& filePath, std::string& outContent, std::wstring& outError)
{
    std::ifstream file(filePath);
    if (!file.is_open())
    {
        outError = L"Error: Unable to open file: " + filePath;
        return false;
    }

    std::stringstream buffer;
    buffer << file.rdbuf();
    outContent = buffer.str();
    return true;
}
} // namespace

bool ConfigurationManager::LoadFromFile(const std::wstring& filePath, std::vector<MonitorConfig>& outConfigs,
                                        std::wstring& outError)
{
    std::string jsonContent;
    if (!ReadFileContent(filePath, jsonContent, outError))
    {
        return false;
    }

    return ParseJSON(jsonContent, outConfigs, outError);
}

bool ConfigurationManager::ParseJSON(const std::string& jsonContent, std::vector<MonitorConfig>& outConfigs,
                                     std::wstring& outError)
{
    outConfigs.clear();

    try
    {
        // Parse JSON using nlohmann-json library
        json j = json::parse(jsonContent);

        if (!j.is_array())
        {
            outError = L"Error: Invalid JSON - expected array of monitors";
            return false;
        }

        size_t monitorIndex = 0;
        for (const auto& monitorJson : j)
        {
            // Parse monitor configuration
            MonitorConfig config;

            // Required fields
            if (!monitorJson.contains("id") || !monitorJson["id"].is_string())
            {
                outError = L"Error: Monitor " + std::to_wstring(monitorIndex) + L" is missing required field \"id\"";
                return false;
            }
            config.id = monitorJson["id"].get<std::string>();

            if (!monitorJson.contains("width") || !monitorJson["width"].is_number_integer())
            {
                outError =
                    L"Error: Monitor \"" + StringToWString(config.id) + L"\" is missing required field \"width\"";
                return false;
            }
            config.width = static_cast<DWORD>(monitorJson["width"].get<int>());

            if (!monitorJson.contains("height") || !monitorJson["height"].is_number_integer())
            {
                outError =
                    L"Error: Monitor \"" + StringToWString(config.id) + L"\" is missing required field \"height\"";
                return false;
            }
            config.height = static_cast<DWORD>(monitorJson["height"].get<int>());

            // Optional fields with defaults
            if (monitorJson.contains("refreshRate") && monitorJson["refreshRate"].is_number_integer())
            {
                config.refreshRate = static_cast<DWORD>(monitorJson["refreshRate"].get<int>());
            }

            if (monitorJson.contains("position") && monitorJson["position"].is_object())
            {
                const auto& pos = monitorJson["position"];
                if (pos.contains("x") && pos["x"].is_number_integer())
                {
                    config.positionX = static_cast<LONG>(pos["x"].get<int>());
                }
                if (pos.contains("y") && pos["y"].is_number_integer())
                {
                    config.positionY = static_cast<LONG>(pos["y"].get<int>());
                }
            }

            if (monitorJson.contains("primary") && monitorJson["primary"].is_boolean())
            {
                config.primary = monitorJson["primary"].get<bool>();
            }

            // Validate this monitor
            if (!ValidateMonitor(config, monitorIndex, outError))
            {
                return false;
            }

            outConfigs.push_back(config);
            monitorIndex++;
        }

        if (outConfigs.empty())
        {
            outError = L"Error: No monitors defined in configuration";
            return false;
        }

        // Validate entire configuration
        return ValidateConfiguration(outConfigs, outError);
    }
    catch (const json::parse_error& e)
    {
        outError = L"Error: JSON parse error: " + StringToWString(e.what());
        return false;
    }
    catch (const json::exception& e)
    {
        outError = L"Error: JSON error: " + StringToWString(e.what());
        return false;
    }
    catch (const std::exception& e)
    {
        outError = L"Error: " + StringToWString(e.what());
        return false;
    }
}

bool ConfigurationManager::ValidateMonitor(const MonitorConfig& config, size_t index, std::wstring& outError)
{
    (void)index; // Parameter reserved for future use

    // Validate width
    if (config.width < 640 || config.width > 7680)
    {
        outError = L"Error: Monitor \"" + std::wstring(config.id.begin(), config.id.end()) + L"\" has invalid width " +
                   std::to_wstring(config.width) + L" (must be 640-7680)";
        return false;
    }

    // Validate height
    if (config.height < 480 || config.height > 4320)
    {
        outError = L"Error: Monitor \"" + std::wstring(config.id.begin(), config.id.end()) + L"\" has invalid height " +
                   std::to_wstring(config.height) + L" (must be 480-4320)";
        return false;
    }

    // Validate refresh rate
    static const std::set<DWORD> validRates = {60, 75, 90, 120, 144, 240};
    if (validRates.find(config.refreshRate) == validRates.end())
    {
        outError = L"Error: Monitor \"" + std::wstring(config.id.begin(), config.id.end()) +
                   L"\" has invalid refresh rate " + std::to_wstring(config.refreshRate) +
                   L" (must be 60, 75, 90, 120, 144, or 240)";
        return false;
    }

    return true;
}

bool ConfigurationManager::ValidateConfiguration(const std::vector<MonitorConfig>& configs, std::wstring& outError)
{
    if (configs.size() > 10)
    {
        outError = L"Error: Too many monitors defined (" + std::to_wstring(configs.size()) + L"), maximum is 10";
        return false;
    }

    // Check for duplicate IDs
    std::set<std::string> ids;
    for (const auto& config : configs)
    {
        if (ids.find(config.id) != ids.end())
        {
            outError = L"Error: Duplicate monitor ID \"" + std::wstring(config.id.begin(), config.id.end()) + L"\"";
            return false;
        }
        ids.insert(config.id);
    }

    // Check for overlapping monitors (simplified check)
    return CheckForOverlaps(configs, outError);
}

bool ConfigurationManager::CheckForOverlaps(const std::vector<MonitorConfig>& configs, std::wstring& outError)
{
    for (size_t i = 0; i < configs.size(); i++)
    {
        for (size_t j = i + 1; j < configs.size(); j++)
        {
            const auto& m1 = configs[i];
            const auto& m2 = configs[j];

            // Simple AABB overlap check
            bool xOverlap = (m1.positionX < m2.positionX + (LONG)m2.width) &&
                            (m1.positionX + (LONG)m1.width > m2.positionX);
            bool yOverlap = (m1.positionY < m2.positionY + (LONG)m2.height) &&
                            (m1.positionY + (LONG)m1.height > m2.positionY);

            if (xOverlap && yOverlap)
            {
                outError = L"Error: Monitors \"" + std::wstring(m1.id.begin(), m1.id.end()) + L"\" and \"" +
                           std::wstring(m2.id.begin(), m2.id.end()) + L"\" overlap";
                return false;
            }
        }
    }

    return true;
}

std::vector<MonitorConfig> ConfigurationManager::GetDefaultConfiguration()
{
    std::vector<MonitorConfig> configs;

    // TEMPORARY DEBUG: Return 10 monitors to verify if this function is being called
    // If you see 10 monitors, the registry config is NOT being loaded!
    MonitorConfig mon1;
    mon1.id = "monitor1-1440p-144hz";
    mon1.width = 2560;
    mon1.height = 1440;
    mon1.refreshRate = 144;
    mon1.positionX = 0;
    mon1.positionY = 0;
    mon1.primary = true;
    configs.push_back(mon1);

    MonitorConfig mon2;
    mon2.id = "monitor2-4k-60hz";
    mon2.width = 3840;
    mon2.height = 2160;
    mon2.refreshRate = 60;
    mon2.positionX = 2560;
    mon2.positionY = 0;
    configs.push_back(mon2);

    MonitorConfig mon3;
    mon3.id = "monitor3-1440p-60hz";
    mon3.width = 2560;
    mon3.height = 1440;
    mon3.refreshRate = 60;
    mon3.positionX = 6400;
    mon3.positionY = 0;
    configs.push_back(mon3);

    MonitorConfig mon4;
    mon4.id = "monitor4-1080p-144hz";
    mon4.width = 1920;
    mon4.height = 1080;
    mon4.refreshRate = 144;
    mon4.positionX = 8320;
    mon4.positionY = 0;
    configs.push_back(mon4);

    // Add 6 more monitors for debug testing (total 10)
    for (int i = 5; i <= 10; i++)
    {
        MonitorConfig monDebug;
        monDebug.id = "debug-monitor-" + std::to_string(i);
        monDebug.width = 1024;
        monDebug.height = 768;
        monDebug.refreshRate = 60;
        monDebug.positionX = 10240 + (i - 5) * 1024;
        monDebug.positionY = 0;
        configs.push_back(monDebug);
    }

    return configs;
}
