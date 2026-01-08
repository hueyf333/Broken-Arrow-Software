# Build Instructions for Windows

This document provides step-by-step instructions for building Broken Arrow Skin Lab on Windows.

## Prerequisites

### Required Software

1. **Windows 10/11**
   - Version 1809 (Build 17763) or later
   - Windows 11 recommended for best experience

2. **Visual Studio 2022** (or later)
   - Download from: https://visualstudio.microsoft.com/
   - Community Edition is sufficient

3. **Required Visual Studio Workloads**
   - .NET Desktop Development
   - Universal Windows Platform development
   - Windows App SDK C# Templates

### Installing Visual Studio with Required Workloads

1. Run Visual Studio Installer
2. Select "Modify" on your Visual Studio 2022 installation
3. In the "Workloads" tab, check:
   - ✓ .NET Desktop Development
   - ✓ Universal Windows Platform development
4. In the "Individual Components" tab, ensure these are selected:
   - ✓ Windows App SDK C# Templates
   - ✓ .NET 8.0 Runtime
5. Click "Modify" to install

### Installing .NET SDK and Windows App SDK

**Option 1: Using Visual Studio Installer (Recommended)**
- The workloads above include necessary SDKs

**Option 2: Manual Installation**

1. Install .NET 8.0 SDK:
   ```powershell
   winget install Microsoft.DotNet.SDK.8
   ```

2. Install Windows App SDK:
   ```powershell
   dotnet workload install microsoft-windowsapp-sdk
   ```

## Building the Solution

### Method 1: Using Visual Studio (Recommended)

1. **Clone the Repository**
   ```powershell
   git clone https://github.com/hueyf333/Broken-Arrow-Software.git
   cd Broken-Arrow-Software
   ```

2. **Open the Solution**
   - Double-click `BrokenArrowSkinLab.sln`
   - Or in Visual Studio: File → Open → Project/Solution → Select `BrokenArrowSkinLab.sln`

3. **Restore NuGet Packages**
   - Visual Studio will automatically prompt to restore packages
   - Or manually: Right-click solution → Restore NuGet Packages
   - Wait for restoration to complete (check status bar)

4. **Select Build Configuration**
   - Configuration: Debug or Release
   - Platform: x64 (recommended) or x86 or ARM64

5. **Build the Solution**
   - Press `F6` or `Ctrl+Shift+B`
   - Or: Build → Build Solution
   - Check Output window for build status

6. **Run the Application**
   - Press `F5` to run with debugging
   - Or `Ctrl+F5` to run without debugging
   - Or: Debug → Start Debugging / Start Without Debugging

### Method 2: Using Command Line

1. **Open Developer Command Prompt for VS 2022**
   - Start Menu → Visual Studio 2022 → Developer Command Prompt

2. **Navigate to Repository**
   ```powershell
   cd path\to\Broken-Arrow-Software
   ```

3. **Restore Packages**
   ```powershell
   dotnet restore BrokenArrowSkinLab.sln
   ```

4. **Build the Solution**
   ```powershell
   dotnet build BrokenArrowSkinLab.sln -c Release
   ```

5. **Run the Application**
   ```powershell
   dotnet run --project BrokenArrowSkinLab\BrokenArrowSkinLab.csproj
   ```

### Method 3: Using MSBuild

```powershell
# Restore
msbuild BrokenArrowSkinLab.sln /t:Restore

# Build
msbuild BrokenArrowSkinLab.sln /p:Configuration=Release /p:Platform=x64

# Run
.\BrokenArrowSkinLab\bin\x64\Release\net8.0-windows10.0.19041.0\BrokenArrowSkinLab.exe
```

## Running Tests

### Using Visual Studio

1. Open Test Explorer: Test → Test Explorer (or `Ctrl+E, T`)
2. Click "Run All Tests" button
3. View results in Test Explorer window

### Using Command Line

```powershell
# Run all tests
dotnet test BrokenArrowSkinLab.Tests\BrokenArrowSkinLab.Tests.csproj

# Run with detailed output
dotnet test BrokenArrowSkinLab.Tests\BrokenArrowSkinLab.Tests.csproj --verbosity normal

# Run specific test
dotnet test --filter "FullyQualifiedName~LayerTests"
```

## Troubleshooting

### Issue: "SDK not found" or "Workload not installed"

**Solution:**
```powershell
dotnet workload install microsoft-windowsapp-sdk
dotnet workload restore
```

### Issue: NuGet package restore fails

**Solution:**
```powershell
# Clear NuGet cache
dotnet nuget locals all --clear

# Restore packages
dotnet restore BrokenArrowSkinLab.sln --force
```

### Issue: "Project targets .NET 8.0 but SDK not found"

**Solution:**
1. Download and install .NET 8.0 SDK from https://dotnet.microsoft.com/download
2. Restart Visual Studio
3. Run: `dotnet --list-sdks` to verify installation

### Issue: "Windows App SDK not found"

**Solution:**
1. Install via Visual Studio Installer:
   - Modify VS 2022 → Individual Components
   - Search for "Windows App SDK"
   - Install Windows App SDK C# Templates

2. Or via command line:
   ```powershell
   dotnet workload install microsoft-windowsapp-sdk
   ```

### Issue: Build succeeds but app won't run

**Solution:**
1. Check Windows version: `winver` (must be 1809 or later)
2. Ensure all NuGet packages restored successfully
3. Try cleaning and rebuilding:
   ```powershell
   dotnet clean
   dotnet build
   ```

### Issue: Missing asset files

**Solution:**
The repository includes placeholder PNG assets. If they're missing:
1. Check `BrokenArrowSkinLab\Assets\` folder
2. Ensure all PNG files are present (see Assets/README.md)
3. If missing, they can be regenerated (requires Python with Pillow)

## Build Output Locations

### Debug Build (x64)
```
BrokenArrowSkinLab\bin\x64\Debug\net8.0-windows10.0.19041.0\
```

### Release Build (x64)
```
BrokenArrowSkinLab\bin\x64\Release\net8.0-windows10.0.19041.0\
```

The main executable is `BrokenArrowSkinLab.exe` in the above directories.

## Creating a Deployment Package

### MSIX Package (Recommended for Distribution)

1. Right-click `BrokenArrowSkinLab` project → Publish → Create App Packages
2. Select distribution method
3. Configure package settings
4. Build package

### Standalone Deployment

```powershell
dotnet publish BrokenArrowSkinLab\BrokenArrowSkinLab.csproj `
  -c Release `
  -r win-x64 `
  --self-contained true `
  -p:PublishSingleFile=true `
  -p:IncludeNativeLibrariesForSelfExtract=true
```

Output location:
```
BrokenArrowSkinLab\bin\Release\net8.0-windows10.0.19041.0\win-x64\publish\
```

## Performance Build (Optimized)

For best performance:

```powershell
dotnet build BrokenArrowSkinLab.sln `
  -c Release `
  -p:Platform=x64 `
  -p:PublishTrimmed=true `
  -p:PublishReadyToRun=true
```

## Continuous Integration

For automated builds, use:

```yaml
# Example GitHub Actions workflow
- name: Setup .NET
  uses: actions/setup-dotnet@v3
  with:
    dotnet-version: '8.0.x'

- name: Install Windows SDK
  uses: microsoft/setup-msbuild@v1

- name: Restore dependencies
  run: dotnet restore

- name: Build
  run: dotnet build --configuration Release --no-restore

- name: Test
  run: dotnet test --no-restore --verbosity normal
```

## Next Steps

After successful build:
1. Run the application
2. Try creating a new project
3. Add layers and experiment with properties
4. Export a sample image
5. Explore the templates in Assets/Templates/

## Getting Help

- Check README.md for usage instructions
- Review DEVELOPER.md for architecture details
- Open an issue on GitHub for specific problems

---

**Note**: This project is Windows-only due to WinUI 3 platform requirements.
