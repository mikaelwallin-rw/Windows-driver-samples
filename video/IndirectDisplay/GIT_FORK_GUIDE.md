# ?? Fork and Push Changes to Your GitHub

## Step 1: Fork on GitHub (Web)

1. Go to: https://github.com/microsoft/Windows-driver-samples
2. Click **"Fork"** button (top-right)
3. This creates: `https://github.com/YOUR_USERNAME/Windows-driver-samples`

---

## Step 2: Add Your Fork as Remote

```powershell
cd "D:\wkspaces\Windows-driver-samples"

# Add your fork as 'myfork' remote (replace YOUR_USERNAME)
git remote add myfork https://github.com/YOUR_USERNAME/Windows-driver-samples.git

# Verify remotes
git remote -v
```

**Expected output:**
```
origin    https://github.com/microsoft/Windows-driver-samples.git (fetch)
origin    https://github.com/microsoft/Windows-driver-samples.git (push)
myfork    https://github.com/YOUR_USERNAME/Windows-driver-samples.git (fetch)
myfork    https://github.com/YOUR_USERNAME/Windows-driver-samples.git (push)
```

---

## Step 3: Create a New Branch for Your Changes

```powershell
# Create and switch to a new branch
git checkout -b idd-virtual-display-enhancements

# Or if you prefer a different name:
# git checkout -b my-custom-branch-name
```

**Why create a branch?**
- Keeps your changes separate from the main branch
- Makes it easier to create Pull Requests later
- Allows you to pull updates from Microsoft's repo without conflicts

---

## Step 4: Stage Your Changes

```powershell
# Check current status
git status

# Stage the scripts and documentation you created
git add RunDriver.ps1
git add UpdateDriver.ps1
git add CheckDisplays.ps1
git add README_SCRIPTS.md
git add MONITOR_CONFIGURATION_GUIDE.md
git add VALIDATION_SUMMARY.md
git add RELOCATION_SUMMARY.md
git add QUICK_REFERENCE.md

# Stage modified driver files
git add IddSampleDriver/Driver.cpp
git add IddSampleDriver/Driver.h

# Or stage everything at once:
# git add .
```

---

## Step 5: Commit Your Changes

```powershell
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
- Monitor 4: 1920x1080 @ 144Hz"
```

---

## Step 6: Push to Your Fork

```powershell
# Push your branch to your fork
git push -u myfork idd-virtual-display-enhancements
```

**First time pushing?** Git might ask for credentials:
- **Username:** Your GitHub username
- **Password:** Use a **Personal Access Token** (not your password)
  - Create one at: https://github.com/settings/tokens
  - Required scopes: `repo` (Full control of private repositories)

---

## Step 7: Create Pull Request (Optional)

If you want to contribute back to Microsoft's repository:

1. Go to your fork: `https://github.com/YOUR_USERNAME/Windows-driver-samples`
2. Click **"Compare & pull request"** button
3. Fill in description of your changes
4. Click **"Create pull request"**

**OR** just keep it in your fork for personal use!

---

## ?? Quick Command Summary

```powershell
# Navigate to repository
cd "D:\wkspaces\Windows-driver-samples"

# Add your fork (replace YOUR_USERNAME!)
git remote add myfork https://github.com/YOUR_USERNAME/Windows-driver-samples.git

# Create and switch to new branch
git checkout -b idd-virtual-display-enhancements

# Stage all changes
git add .

# Commit with message
git commit -m "Add virtual display driver enhancements"

# Push to your fork
git push -u myfork idd-virtual-display-enhancements
```

---

## ?? Future Workflow

### Keep Your Fork Updated with Microsoft's Changes

```powershell
# Fetch latest from Microsoft
git fetch origin

# Switch to main branch
git checkout main

# Merge Microsoft's updates
git merge origin/main

# Push updates to your fork
git push myfork main

# Switch back to your feature branch
git checkout idd-virtual-display-enhancements

# Rebase your changes on top of latest main (optional)
git rebase main
```

### Make More Changes Later

```powershell
# Make your code changes...

# Stage and commit
git add .
git commit -m "Additional improvements"

# Push to your fork
git push myfork idd-virtual-display-enhancements
```

---

## ??? Authentication Options

### Option 1: HTTPS with Personal Access Token (Recommended)

```powershell
# When prompted for password, use your Personal Access Token
# Create at: https://github.com/settings/tokens
```

### Option 2: SSH (More Secure, No Password Needed)

```powershell
# Change remote to SSH
git remote set-url myfork git@github.com:YOUR_USERNAME/Windows-driver-samples.git

# Requires SSH key setup first
# Guide: https://docs.github.com/en/authentication/connecting-to-github-with-ssh
```

---

## ?? Files You're About to Commit

**New Files:**
- ? `RunDriver.ps1` - Launch virtual monitors
- ? `UpdateDriver.ps1` - Update driver
- ? `CheckDisplays.ps1` - Display diagnostics
- ? `README_SCRIPTS.md` - Script documentation
- ? `MONITOR_CONFIGURATION_GUIDE.md` - Configuration guide
- ? `VALIDATION_SUMMARY.md` - Implementation summary
- ? `RELOCATION_SUMMARY.md` - Relocation notes
- ? `QUICK_REFERENCE.md` - Quick reference

**Modified Files:**
- ? `IddSampleDriver/Driver.cpp` - 4 monitors, custom resolutions
- ? `IddSampleDriver/Driver.h` - Header updates

---

## ? Troubleshooting

**Q: "fatal: remote myfork already exists"**
```powershell
# Remove and re-add with correct URL
git remote remove myfork
git remote add myfork https://github.com/YOUR_USERNAME/Windows-driver-samples.git
```

**Q: "Authentication failed"**
- Use Personal Access Token instead of password
- Or set up SSH keys

**Q: "You are not currently on a branch"**
```powershell
git checkout main
git checkout -b idd-virtual-display-enhancements
```

---

**Your changes are now backed up on your GitHub account!** ??
