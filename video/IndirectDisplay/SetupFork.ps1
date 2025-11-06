# 🚀 Quick Setup Script - Fork and Push Changes
# 
# BEFORE RUNNING: 
# 1. Fork the repository on GitHub: https://github.com/microsoft/Windows-driver-samples
# 2. Replace mikaelwallin-rw below with your actual GitHub username
# 3. Run this script in PowerShell as Administrator

# ============================================================
# CONFIGURATION - EDIT THIS!
# ============================================================
$GitHubUsername = "mikaelwallin-rw"  # ← CHANGE THIS!
$BranchName = "idd-virtual-display-enhancements"

# ============================================================
# DO NOT EDIT BELOW THIS LINE
# ============================================================

Write-Host "=== Setting up Git Fork and Push ===" -ForegroundColor Cyan
Write-Host ""

# Validate username
if ($GitHubUsername -eq "YOUR_USERNAME") {
    Write-Host "ERROR: Please edit this script and set your GitHub username!" -ForegroundColor Red
    Write-Host "Open SetupFork.ps1 and change line 9:" -ForegroundColor Yellow
    Write-Host '  $GitHubUsername = "your-actual-username"' -ForegroundColor Yellow
    Write-Host ""
    pause
    exit 1
}

# Navigate to repository root
$repoPath = "D:\wkspaces\Windows-driver-samples"
if (-not (Test-Path $repoPath)) {
    Write-Host "ERROR: Repository not found at: $repoPath" -ForegroundColor Red
    pause
    exit 1
}

Set-Location $repoPath
Write-Host "✓ Repository location: $repoPath" -ForegroundColor Green
Write-Host ""

# Check if myfork remote already exists
$existingRemote = git remote | Where-Object { $_ -eq "myfork" }
if ($existingRemote) {
    Write-Host "⚠ Remote 'myfork' already exists. Removing and re-adding..." -ForegroundColor Yellow
    git remote remove myfork
}

# Add your fork as remote
Write-Host "Adding your fork as remote 'myfork'..." -ForegroundColor Cyan
$forkUrl = "https://github.com/$GitHubUsername/Windows-driver-samples.git"
git remote add myfork $forkUrl

# Verify remotes
Write-Host ""
Write-Host "Current remotes:" -ForegroundColor Cyan
git remote -v
Write-Host ""

# Create new branch
Write-Host "Creating branch: $BranchName" -ForegroundColor Cyan
$currentBranch = git branch --show-current
if ($currentBranch -eq $BranchName) {
Write-Host "✓ Already on branch: $BranchName" -ForegroundColor Green
} else {
 git checkout -b $BranchName
    if ($LASTEXITCODE -eq 0) {
        Write-Host "✓ Created and switched to branch: $BranchName" -ForegroundColor Green
    } else {
        # Branch might already exist
        git checkout $BranchName
        if ($LASTEXITCODE -eq 0) {
        Write-Host "✓ Switched to existing branch: $BranchName" -ForegroundColor Green
        }
    }
}
Write-Host ""

# Show current status
Write-Host "Current status:" -ForegroundColor Cyan
git status --short
Write-Host ""

# Stage all changes
Write-Host "Staging all changes..." -ForegroundColor Cyan
git add video/IndirectDisplay/

# Show what will be committed
Write-Host ""
Write-Host "Files to be committed:" -ForegroundColor Yellow
git diff --cached --name-only
Write-Host ""

# Commit
Write-Host "Creating commit..." -ForegroundColor Cyan
git commit -m "Add virtual display driver enhancements

- Configure 4 virtual monitors with custom resolutions
- Add PowerShell management scripts (RunDriver, UpdateDriver, CheckDisplays)
- Add comprehensive documentation and configuration guides
- Fix obsolete wudfwdm.h header issue
- Relocate scripts to permanent location

Monitors configured:
- Monitor 1: 2560x1440 @ 144Hz
- Monitor 2: 3840x2160 @ 60Hz (4K)
- Monitor 3: 1920x1080 @ 60Hz
- Monitor 4: 1920x1080 @ 144Hz

Scripts added:
- RunDriver.ps1 - Launch virtual monitors
- UpdateDriver.ps1 - Update driver after rebuild
- CheckDisplays.ps1 - Display diagnostics

Documentation added:
- README_SCRIPTS.md - Script usage guide
- MONITOR_CONFIGURATION_GUIDE.md - Configuration reference
- QUICK_REFERENCE.md - Quick command reference
- GIT_FORK_GUIDE.md - Git workflow guide"

if ($LASTEXITCODE -eq 0) {
    Write-Host "✓ Commit created successfully" -ForegroundColor Green
} else {
    Write-Host "⚠ Commit may have failed or nothing to commit" -ForegroundColor Yellow
}
Write-Host ""

# Ask before pushing
Write-Host "Ready to push to your fork!" -ForegroundColor Cyan
Write-Host "This will push to: $forkUrl" -ForegroundColor Yellow
Write-Host ""
$response = Read-Host "Do you want to push now? (y/n)"

if ($response -eq 'y' -or $response -eq 'Y') {
    Write-Host ""
    Write-Host "Pushing to your fork..." -ForegroundColor Cyan
    git push -u myfork $BranchName
    
    if ($LASTEXITCODE -eq 0) {
        Write-Host ""
        Write-Host "✓ Successfully pushed to your fork!" -ForegroundColor Green
        Write-Host ""
        Write-Host "Next steps:" -ForegroundColor Cyan
  Write-Host "1. Visit: https://github.com/$GitHubUsername/Windows-driver-samples" -ForegroundColor Yellow
        Write-Host "2. You should see a 'Compare & pull request' button" -ForegroundColor Yellow
        Write-Host "3. (Optional) Create a pull request to contribute back to Microsoft" -ForegroundColor Yellow
   Write-Host ""
      Write-Host "Your changes are now backed up on GitHub! 🎉" -ForegroundColor Green
    } else {
    Write-Host ""
        Write-Host "⚠ Push failed. This might be due to:" -ForegroundColor Yellow
  Write-Host "  - Authentication required (use Personal Access Token, not password)" -ForegroundColor Yellow
   Write-Host "  - Fork doesn't exist yet on GitHub" -ForegroundColor Yellow
   Write-Host "  - Network issues" -ForegroundColor Yellow
        Write-Host ""
        Write-Host "To push manually:" -ForegroundColor Cyan
        Write-Host "  git push -u myfork $BranchName" -ForegroundColor White
    }
} else {
    Write-Host ""
    Write-Host "Skipped push. To push later, run:" -ForegroundColor Cyan
    Write-Host "  git push -u myfork $BranchName" -ForegroundColor White
}

Write-Host ""
Write-Host "Setup complete!" -ForegroundColor Green
Write-Host ""
