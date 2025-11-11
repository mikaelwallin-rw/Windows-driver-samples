# Code Formatting Guide

This project uses **clang-format** for consistent C/C++ code formatting.

## Quick Start

### Auto-format on Save (VS Code)
The `.vscode/settings.json` is configured to automatically format C/C++ files when you save them.

### Manual Formatting

#### Format All Files
```powershell
.\format-code.ps1
```

#### Check Formatting (without changing files)
```powershell
.\format-code.ps1 -Check
```

#### Format Single File
```powershell
clang-format -i <filename>
```

## Installation

### Windows
1. **Download LLVM** (includes clang-format):
   - https://releases.llvm.org/
   - Or install via Chocolatey: `choco install llvm`

2. **Verify installation**:
   ```powershell
   clang-format --version
   ```

### Visual Studio
clang-format is included with Visual Studio 2022. It's automatically available.

## Configuration Files

### `.clang-format`
Main configuration file defining code style rules:
- Based on Microsoft coding style
- 4-space indentation
- Allman brace style (braces on new lines)
- 120 character line limit
- Sorted includes with grouping

### `.clang-format-ignore`
Lists patterns for files/directories to exclude from formatting.

### `.vscode/settings.json`
VS Code editor configuration:
- Enables format-on-save
- Sets clang-format as default C++ formatter
- Configures tab size and indentation

## Style Guidelines

### Key Formatting Rules

**Indentation**: 4 spaces, no tabs
```cpp
void Function()
{
    if (condition)
    {
        DoSomething();
    }
}
```

**Braces**: Allman style (new line)
```cpp
if (condition)
{
    // code
}
else
{
    // code
}
```

**Line Length**: Maximum 120 characters
```cpp
// Long lines are automatically wrapped
SomeLongFunctionName(parameter1, parameter2, parameter3,
                     parameter4, parameter5);
```

**Pointer/Reference Alignment**: Left-aligned
```cpp
int* pointer;
const std::string& reference;
```

**Include Order**:
1. Windows SDK headers (`<windows.h>`, etc.)
2. WDK headers (`<wdf.h>`, `<iddcx.h>`, etc.)
3. Standard C++ headers (`<vector>`, `<string>`, etc.)
4. Third-party headers (`<nlohmann/json.hpp>`)
5. Project headers (`"Driver.h"`, `"ConfigurationManager.h"`)

## Disabling Formatting

### For Specific Lines
```cpp
// clang-format off
int array[] = {
    0x00, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0x00,
    0x10, 0xAC, 0xE6, 0xD0, 0x55, 0x5A, 0x4A, 0x30
};
// clang-format on
```

### For Entire Files
Add the file pattern to `.clang-format-ignore`

## VS Code Usage

### Format Document
- **Windows**: `Shift + Alt + F`
- **Command Palette**: `Format Document`

### Format Selection
- **Windows**: `Ctrl + K, Ctrl + F`
- **Command Palette**: `Format Selection`

### View Formatting Changes
Before saving, you can see what will be formatted in the diff view.

## Integration with Git

### Pre-commit Hook (Optional)
You can add a git pre-commit hook to check formatting:

1. Create `.git/hooks/pre-commit`:
```bash
#!/bin/sh
pwsh -File format-code.ps1 -Check
```

2. Make it executable:
```bash
chmod +x .git/hooks/pre-commit
```

### CI/CD (Optional)
Add to your CI pipeline:
```yaml
- name: Check formatting
  run: pwsh format-code.ps1 -Check
```

## Troubleshooting

### "clang-format not found"
- Install LLVM or Visual Studio 2022
- Add clang-format to PATH
- Restart VS Code after installation

### "Formatting doesn't match expected style"
- Verify `.clang-format` file exists in project root
- Check VS Code settings: `C_Cpp.clang_format_style` should be `"file"`
- Reload VS Code window

### "Some files aren't being formatted"
- Check if file is in `.clang-format-ignore`
- Verify file extension is `.cpp`, `.h`, or `.hpp`
- Check if file is in excluded directory (x64, ARM64, etc.)

## Additional Resources

- [clang-format Documentation](https://clang.llvm.org/docs/ClangFormat.html)
- [clang-format Options](https://clang.llvm.org/docs/ClangFormatStyleOptions.html)
- [VS Code C++ Extension](https://code.visualstudio.com/docs/cpp/cpp-ide)

## Customization

To modify the formatting rules, edit `.clang-format`:
```yaml
# Example: Change line length
ColumnLimit: 100

# Example: Change brace style
BreakBeforeBraces: Attach
```

After changes, run `.\format-code.ps1` to reformat all files.
