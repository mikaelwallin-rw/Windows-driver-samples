#include "TemplateManager.h"

#include <algorithm>
#include <fstream>

#include "ConfigurationManager.h"

using namespace Microsoft::IndirectDisp;

TemplateManager::TemplateManager(const std::wstring& templatesDir)
{
    if (templatesDir.empty())
    {
        m_templatesDir = AutoDetectTemplatesDirectory();
    }
    else
    {
        m_templatesDir = templatesDir;
    }
}

std::vector<TemplateInfo> TemplateManager::ListTemplates() const
{
    std::vector<TemplateInfo> templates;

    WIN32_FIND_DATAW findData;
    HANDLE hFind = FindFirstFileW((m_templatesDir + L"\\*.json").c_str(), &findData);

    if (hFind != INVALID_HANDLE_VALUE)
    {
        do
        {
            std::wstring fileName = findData.cFileName;
            // Skip directories
            if (findData.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY)
            {
                continue;
            }

            // Only process .json files
            if (fileName.length() > 5 && fileName.substr(fileName.length() - 5) == L".json")
            {
                std::wstring fullPath = m_templatesDir + L"\\" + fileName;
                TemplateInfo info = ExtractTemplateInfo(fullPath);
                if (info.valid)
                {
                    templates.push_back(info);
                }
            }
        } while (FindNextFileW(hFind, &findData));
        FindClose(hFind);
    }

    // Sort templates by name for consistent ordering
    std::sort(templates.begin(), templates.end(),
              [](const TemplateInfo& a, const TemplateInfo& b) { return a.name < b.name; });

    return templates;
}

bool TemplateManager::TemplateExists(const std::wstring& templateName) const
{
    std::wstring templatePath = m_templatesDir + L"\\" + templateName + L".json";
    DWORD attr = GetFileAttributesW(templatePath.c_str());
    return (attr != INVALID_FILE_ATTRIBUTES && !(attr & FILE_ATTRIBUTE_DIRECTORY));
}

std::wstring TemplateManager::GetTemplatePath(const std::wstring& templateName) const
{
    std::wstring templatePath = m_templatesDir + L"\\" + templateName + L".json";
    if (TemplateExists(templateName))
    {
        return templatePath;
    }
    return L"";
}

bool TemplateManager::LoadTemplate(const std::wstring& templateName, std::vector<MonitorConfig>& outConfigs,
                                   std::wstring& outError) const
{
    std::wstring templatePath = GetTemplatePath(templateName);
    if (templatePath.empty())
    {
        outError = L"Template not found: " + templateName;
        return false;
    }

    return ConfigurationManager::LoadFromFile(templatePath, outConfigs, outError);
}

TemplateInfo TemplateManager::GetTemplateInfo(const std::wstring& templateName) const
{
    std::wstring templatePath = GetTemplatePath(templateName);
    if (templatePath.empty())
    {
        return TemplateInfo(); // Return invalid info
    }

    return ExtractTemplateInfo(templatePath);
}

std::wstring TemplateManager::AutoDetectTemplatesDirectory()
{
    // Get executable directory
    wchar_t exePath[MAX_PATH];
    GetModuleFileNameW(NULL, exePath, MAX_PATH);
    std::wstring fullPath(exePath);
    size_t pos = fullPath.find_last_of(L"\\/");
    std::wstring exeDir = (pos != std::wstring::npos) ? fullPath.substr(0, pos) : L".";

    // Try multiple possible locations (relative to executable)
    std::vector<std::wstring> possiblePaths = {exeDir + L"\\templates",
                                               exeDir + L"\\..\\templates",
                                               exeDir + L"\\..\\..\\templates",
                                               exeDir + L"\\..\\..\\..\\templates",
                                               L".\\templates"};

    for (const auto& path : possiblePaths)
    {
        DWORD attr = GetFileAttributesW(path.c_str());
        if (attr != INVALID_FILE_ATTRIBUTES && (attr & FILE_ATTRIBUTE_DIRECTORY))
        {
            return path;
        }
    }

    // Default to templates subdirectory even if it doesn't exist yet
    return exeDir + L"\\templates";
}

TemplateInfo TemplateManager::ExtractTemplateInfo(const std::wstring& filePath)
{
    TemplateInfo info;
    info.fullPath = filePath;

    // Extract template name from filename
    size_t pos = filePath.find_last_of(L"\\/");
    std::wstring fileName = (pos != std::wstring::npos) ? filePath.substr(pos + 1) : filePath;

    // Remove .json extension
    if (fileName.length() > 5 && fileName.substr(fileName.length() - 5) == L".json")
    {
        info.name = fileName.substr(0, fileName.length() - 5);
    }
    else
    {
        info.name = fileName;
    }

    // Quick scan to count monitors and extract description
    std::ifstream file(filePath);
    if (!file.is_open())
    {
        return info; // Return invalid info
    }

    info.monitorCount = 0;
    std::string line;
    bool foundDescription = false;

    // Read first 100 lines to get a quick overview
    for (int lineNum = 0; lineNum < 100 && std::getline(file, line); lineNum++)
    {
        // Count monitor entries by looking for "id" fields
        if (line.find("\"id\"") != std::string::npos)
        {
            info.monitorCount++;
        }

        // Look for a description field (if templates include this in the future)
        if (!foundDescription && line.find("\"description\"") != std::string::npos)
        {
            size_t start = line.find(':');
            if (start != std::string::npos)
            {
                start = line.find('"', start);
                if (start != std::string::npos)
                {
                    size_t end = line.find('"', start + 1);
                    if (end != std::string::npos)
                    {
                        std::string desc = line.substr(start + 1, end - start - 1);
                        info.description = std::wstring(desc.begin(), desc.end());
                        foundDescription = true;
                    }
                }
            }
        }
    }

    // Generate a description if none was found
    if (info.description.empty())
    {
        if (info.monitorCount == 1)
        {
            info.description = L"Single monitor configuration";
        }
        else if (info.monitorCount > 1)
        {
            info.description = std::to_wstring(info.monitorCount) + L" monitors";
        }
        else
        {
            info.description = L"Configuration template";
        }
    }

    info.valid = true;
    return info;
}
