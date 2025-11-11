# Format all C++ files in the project using clang-format
# Usage: .\format-code.ps1 [-Check]

param(
    [switch]$Check
)

$projectRoot = $PSScriptRoot
$files = Get-ChildItem -Path $projectRoot -Recurse -Include *.cpp, *.h, *.hpp -Exclude *.tmh | 
Where-Object { 
    $_.FullName -notmatch '\\(x64|ARM64|Debug|Release|vcpkg_installed|\.vs)\\' 
}

Write-Host "Found $($files.Count) C++ files to process" -ForegroundColor Cyan

$clangFormat = Get-Command clang-format -ErrorAction SilentlyContinue

if (-not $clangFormat) {
    Write-Host "ERROR: clang-format not found in PATH" -ForegroundColor Red
    Write-Host "Please install LLVM or add clang-format to your PATH" -ForegroundColor Yellow
    Write-Host "Download from: https://releases.llvm.org/" -ForegroundColor Yellow
    exit 1
}

Write-Host "Using clang-format: $($clangFormat.Source)" -ForegroundColor Green

$needsFormatting = @()

foreach ($file in $files) {
    $relativePath = $file.FullName.Substring($projectRoot.Length + 1)
    
    if ($Check) {
        # Check if file needs formatting
        $output = & clang-format --dry-run --Werror $file.FullName 2>&1
        if ($LASTEXITCODE -ne 0) {
            $needsFormatting += $relativePath
            Write-Host "  [NEEDS FORMAT] $relativePath" -ForegroundColor Yellow
        }
        else {
            Write-Host "  [OK] $relativePath" -ForegroundColor Green
        }
    }
    else {
        # Format the file
        Write-Host "  Formatting: $relativePath"
        & clang-format -i $file.FullName
        
        if ($LASTEXITCODE -eq 0) {
            Write-Host "    [OK]" -ForegroundColor Green
        }
        else {
            Write-Host "    [FAILED]" -ForegroundColor Red
        }
    }
}

if ($Check) {
    if ($needsFormatting.Count -gt 0) {
        Write-Host "`n$($needsFormatting.Count) file(s) need formatting:" -ForegroundColor Yellow
        $needsFormatting | ForEach-Object { Write-Host "  - $_" }
        Write-Host "`nRun '.\format-code.ps1' to format all files" -ForegroundColor Cyan
        exit 1
    }
    else {
        Write-Host "`nAll files are properly formatted!" -ForegroundColor Green
        exit 0
    }
}
else {
    Write-Host "`nFormatting complete!" -ForegroundColor Green
}
