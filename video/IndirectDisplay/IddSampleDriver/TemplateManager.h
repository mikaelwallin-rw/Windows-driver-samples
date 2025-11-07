#pragma once

#include <windows.h>
#include <string>
#include <vector>

namespace Microsoft
{
    namespace IndirectDisp
    {
        struct MonitorConfig; // Forward declaration

        /// <summary>
        /// Information about a template
        /// </summary>
        struct TemplateInfo
        {
            std::wstring name;
            std::wstring fullPath;
            int monitorCount;
            std::wstring description;
            bool valid; // Indicates if this is a valid template info

            TemplateInfo() : monitorCount(0), valid(false) {}
        };

        /// <summary>
        /// Centralized manager for template discovery, loading, and validation
        /// </summary>
        class TemplateManager
        {
        public:
            /// <summary>
            /// Initialize the template manager with the templates directory path
            /// </summary>
            /// <param name="templatesDir">Optional custom templates directory. If empty, will auto-detect.</param>
            explicit TemplateManager(const std::wstring& templatesDir = L"");

            /// <summary>
            /// Get the templates directory path
            /// </summary>
            std::wstring GetTemplatesDirectory() const { return m_templatesDir; }

            /// <summary>
            /// List all available templates with their metadata
            /// </summary>
            std::vector<TemplateInfo> ListTemplates() const;

            /// <summary>
            /// Check if a template exists by name
            /// </summary>
            bool TemplateExists(const std::wstring& templateName) const;

            /// <summary>
            /// Get the full path to a template file (returns empty string if not found)
            /// </summary>
            std::wstring GetTemplatePath(const std::wstring& templateName) const;

            /// <summary>
            /// Load a template by name
            /// </summary>
            bool LoadTemplate(const std::wstring& templateName, std::vector<MonitorConfig>& outConfigs, std::wstring& outError) const;

            /// <summary>
            /// Get template information by name (check valid field to see if found)
            /// </summary>
            TemplateInfo GetTemplateInfo(const std::wstring& templateName) const;

        private:
            std::wstring m_templatesDir;

            /// <summary>
            /// Auto-detect the templates directory based on executable location
            /// </summary>
            static std::wstring AutoDetectTemplatesDirectory();

            /// <summary>
            /// Extract quick metadata from a template file without full parsing
            /// </summary>
            static TemplateInfo ExtractTemplateInfo(const std::wstring& filePath);
        };
    }
}
