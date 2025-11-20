#pragma once

#include <windows.h>

#include <memory>
#include <string>
#include <vector>

namespace Microsoft
{
namespace IndirectDisp
{
/// <summary>
/// Represents a monitor configuration
/// </summary>
struct MonitorConfig
{
    std::string id;
    DWORD width;
    DWORD height;
    DWORD refreshRate;
    LONG positionX;
    LONG positionY;
    bool primary;

    MonitorConfig() : width(2560), height(1440), refreshRate(60), positionX(0), positionY(0), primary(false)
    {
    }
};

/// <summary>
/// Manages loading and validation of monitor configurations from JSON
/// </summary>
class ConfigurationManager
{
  public:
    /// <summary>
    /// Load configuration from a JSON file
    /// </summary>
    static bool LoadFromFile(const std::wstring& filePath, std::vector<MonitorConfig>& outConfigs,
                             std::wstring& outError);

    /// <summary>
    /// Parse configuration from JSON string
    /// </summary>
    static bool ParseJSON(const std::string& jsonContent, std::vector<MonitorConfig>& outConfigs,
                          std::wstring& outError);

    /// <summary>
    /// Validate a configuration
    /// </summary>
    static bool ValidateConfiguration(const std::vector<MonitorConfig>& configs, std::wstring& outError);

    /// <summary>
    /// Get default configuration (4 monitors)
    /// </summary>
    static std::vector<MonitorConfig> GetDefaultConfiguration();

  private:
    static bool ValidateMonitor(const MonitorConfig& config, size_t index, std::wstring& outError);
    static bool CheckForOverlaps(const std::vector<MonitorConfig>& configs, std::wstring& outError);
};
} // namespace IndirectDisp
} // namespace Microsoft
