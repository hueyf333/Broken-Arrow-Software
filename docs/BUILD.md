# Build Guide - Broken Arrow Hair Studio

## Prerequisites

### Required Software

1. **Visual Studio 2022**
   - Download from: https://visualstudio.microsoft.com/downloads/
   - Install with:
     - Desktop development with C++
     - Windows 11 SDK (10.0.22621.0 or later)
     - C++ CMake tools for Windows (optional)
     - C++ Clang tools for Windows (optional for static analysis)

2. **Windows App SDK 1.4+**
   - Installed via NuGet (automatic when opening solution)
   - Or download manually from: https://docs.microsoft.com/windows/apps/windows-app-sdk/

3. **DirectX 12**
   - Included with Windows 11 SDK
   - Ensure your GPU supports DirectX 12

### Optional SDKs for Full Export Functionality

4. **Autodesk FBX SDK 2020.3.4+**
   - Download from: https://www.autodesk.com/developer-network/platform-technologies/fbx-sdk-2020-3-4
   - Install to default location: `C:\Program Files\Autodesk\FBX\FBX SDK\2020.3.4`
   - After installation, add to your User or System Environment Variables:
     - `FBX_SDK_ROOT` = `C:\Program Files\Autodesk\FBX\FBX SDK\2020.3.4`
   - Note: Without FBX SDK, you can still use OBJ and GLTF export

5. **Alembic SDK 1.8+**
   - Option A: Build from source: https://github.com/alembic/alembic
   - Option B: Use vcpkg:
     ```bash
     vcpkg install alembic:x64-windows
     ```
   - Note: Without Alembic, animation caching won't be available

6. **tinygltf (Header-only, included)**
   - Already included in `src/third_party/tinygltf`
   - No separate installation needed

## Build Steps

### Quick Start (Standard Build without External SDKs)

1. **Clone the Repository**
   ```bash
   git clone https://github.com/hueyf333/Broken-Arrow-Software.git
   cd Broken-Arrow-Software
   ```

2. **Open the Solution**
   - Launch Visual Studio 2022
   - Open `BrokenArrowHairStudio.sln`

3. **Restore NuGet Packages**
   - Visual Studio will prompt to restore packages
   - Or manually: Right-click solution → Restore NuGet Packages
   - Wait for Microsoft.WindowsAppSDK to download

4. **Select Build Configuration**
   - Platform: **x64** (REQUIRED - x86 not supported)
   - Configuration: **Debug** or **Release**

5. **Build Solution**
   - Menu: Build → Build Solution (Ctrl+Shift+B)
   - Or: Right-click solution → Build Solution
   - First build may take 5-10 minutes

6. **Run the Application**
   - Set `BrokenArrowHairStudio` as startup project (right-click → Set as Startup Project)
   - Press F5 (Debug) or Ctrl+F5 (Release)

### Advanced Build with External SDKs

If you've installed FBX SDK or Alembic:

1. **Configure FBX SDK Path** (if not using default)
   - Create `FBXSettings.props` in solution root:
     ```xml
     <?xml version="1.0" encoding="utf-8"?>
     <Project ToolsVersion="4.0" xmlns="http://schemas.microsoft.com/developer/msbuild/2003">
       <PropertyGroup>
         <FBX_SDK_ROOT>C:\Path\To\Your\FBX_SDK</FBX_SDK_ROOT>
       </PropertyGroup>
     </Project>
     ```
   - Import in projects that need it

2. **Configure Alembic** (if using vcpkg)
   - Visual Studio should auto-detect vcpkg packages
   - Or add manually in project properties:
     - C/C++ → Additional Include Directories: `$(VCPKG_ROOT)\installed\x64-windows\include`
     - Linker → Additional Library Directories: `$(VCPKG_ROOT)\installed\x64-windows\lib`

3. **Build with Full Features**
   - Same build steps as above
   - FBX and Alembic exporters will be enabled automatically

## Build Configurations

### Debug Configuration
- Optimizations: Disabled
- Runtime Library: Multi-threaded Debug DLL (/MDd)
- Debug Information: Full (/Zi)
- Use for: Development, debugging, testing

### Release Configuration
- Optimizations: Maximum Speed (/O2)
- Runtime Library: Multi-threaded DLL (/MD)
- Whole Program Optimization: Yes
- Use for: Production, final builds, performance testing

## Project Structure

```
BrokenArrowHairStudio.sln
├── src/math/           → BrokenArrowMath.vcxproj (Static Library)
├── src/io/             → BrokenArrowIO.vcxproj (Static Library)
├── src/hair/           → BrokenArrowHair.vcxproj (Static Library)
├── src/viewport/       → BrokenArrowViewport.vcxproj (Static Library)
└── src/app/            → BrokenArrowHairStudio.vcxproj (Application)
```

**Build Order (automatic):**
1. BrokenArrowMath (no dependencies)
2. BrokenArrowIO (depends on Math)
3. BrokenArrowHair (depends on Math, IO)
4. BrokenArrowViewport (depends on Math, Hair, IO)
5. BrokenArrowHairStudio (depends on all above)

## Output

### Debug Build
- Location: `bin\x64\Debug\`
- Files:
  - `BrokenArrowHairStudio.exe` - Main executable
  - `BrokenArrowMath.lib` - Math library
  - `BrokenArrowIO.lib` - I/O library
  - `BrokenArrowHair.lib` - Hair system
  - `BrokenArrowViewport.lib` - Viewport renderer
  - `*.pdb` - Debug symbols

### Release Build
- Location: `bin\x64\Release\`
- Files:
  - `BrokenArrowHairStudio.exe` - Optimized executable
  - `*.lib` - Libraries
  - `*.pdb` - Release debug symbols (optional)

## Troubleshooting

### Build Errors

**Error: "Windows App SDK not found"**
- Solution: Install via NuGet Package Manager
- Tools → NuGet Package Manager → Manage NuGet Packages for Solution
- Browse for "Microsoft.WindowsAppSDK" version 1.4+

**Error: "Cannot open include file: 'd3d12.h'"**
- Solution: Install Windows 11 SDK
- Visual Studio Installer → Modify → Individual Components
- Check "Windows 11 SDK (10.0.22621.0)"

**Error: "LNK1104: cannot open file 'd3d12.lib'"**
- Solution: Ensure Windows SDK libraries are in linker path
- Project Properties → Linker → General → Additional Library Directories
- Should include: `$(WindowsSdkDir)Lib\$(WindowsSDKVersion)\um\x64`

**Error: "C1083: Cannot open include file: 'fbxsdk.h'"**
- This is expected if FBX SDK is not installed
- FBX export features will be disabled
- OBJ and GLTF export still work

**Error: Platform mismatch (trying to build x86)**
- Solution: Always select **x64** platform
- Configuration Manager → Active Solution Platform → x64

### Runtime Errors

**Error: "The application was unable to start correctly (0xc000007b)"**
- Cause: Missing Visual C++ Redistributables
- Solution: Install from https://aka.ms/vs/17/release/vc_redist.x64.exe

**Error: DirectX 12 device creation failed**
- Cause: GPU doesn't support DX12 or drivers out of date
- Solution 1: Update graphics drivers
- Solution 2: Use a DX12-compatible GPU

**Error: "MSVCP140.dll was not found"**
- Cause: Visual C++ Runtime missing
- Solution: Install Visual C++ 2022 Redistributable

## Performance Tips

### Faster Builds
1. Use Release configuration for faster compilation
2. Enable parallel compilation:
   - Project Properties → C/C++ → General
   - Multi-processor Compilation: Yes (/MP)
3. Use SSD for source and build output
4. Disable unnecessary build features:
   - Tools → Options → Projects and Solutions → Build and Run
   - Reduce "Maximum number of parallel project builds"

### Incremental Builds
- After first build, subsequent builds are much faster
- Clean only when necessary (Build → Clean Solution)
- Use "Build" instead of "Rebuild" for incremental updates

## Next Steps

After successful build:
1. Read the main [README.md](../README.md) for usage instructions
2. Try the tutorial: [Tutorial.md](Tutorial.md)
3. Import a sample scalp mesh from `assets/meshes/`
4. Create your first hair description!

## Support

For build issues:
- Check existing GitHub Issues
- Create new issue with:
  - Visual Studio version
  - Build configuration
  - Full error message
  - Build output log

---

**Happy Building!**

*Last Updated: January 2024*
