# SkinningStudio Troubleshooting Guide

## Common Build Issues

### Error: Cannot open include file: 'nlohmann/json.hpp'

This error occurs when vcpkg hasn't properly installed the nlohmann-json dependency. Here are solutions in order of likelihood:

#### Solution 1: Restart Visual Studio
The simplest solution - close and reopen Visual Studio. This triggers vcpkg to restore dependencies from vcpkg.json.

1. Close Visual Studio completely
2. Reopen SkinningStudio.sln
3. Build > Rebuild Solution

#### Solution 2: Verify vcpkg Integration
Ensure vcpkg is properly integrated with Visual Studio:

1. Open a command prompt/PowerShell
2. Navigate to your vcpkg installation directory (e.g., `C:\vcpkg`)
3. Run: `vcpkg integrate install`
4. You should see: "Applied user-wide integration for this vcpkg root"
5. Restart Visual Studio
6. Rebuild the solution

#### Solution 3: Clean and Rebuild
Force a clean rebuild of the solution:

1. In Visual Studio: Build > Clean Solution
2. Delete the `vcpkg_installed` folder if it exists in the solution directory
3. Build > Rebuild Solution
4. Wait for vcpkg to download and install dependencies (first build may take 5-10 minutes)

#### Solution 4: Manual vcpkg Installation
If automatic installation fails, manually install the dependencies:

```bash
# Navigate to your vcpkg directory
cd C:\vcpkg  # or wherever you installed vcpkg

# Install dependencies for x64-windows
vcpkg install nlohmann-json:x64-windows
vcpkg install sdl2:x64-windows
vcpkg install glew:x64-windows
vcpkg install imgui[core,docking-experimental,sdl2-binding,opengl3-binding]:x64-windows
```

Then rebuild the solution in Visual Studio.

#### Solution 5: Check vcpkg Installation
Verify vcpkg is installed and accessible:

1. Open PowerShell/Command Prompt
2. Run: `vcpkg version`
3. If "command not found", you need to install vcpkg:

```bash
# Clone vcpkg
git clone https://github.com/Microsoft/vcpkg.git C:\vcpkg
cd C:\vcpkg

# Bootstrap vcpkg
.\bootstrap-vcpkg.bat

# Integrate with Visual Studio
.\vcpkg integrate install
```

#### Solution 6: Verify Project Configuration
Ensure the project files have proper vcpkg settings:

1. Open SkinningStudio.vcxproj in a text editor
2. Look for these lines in the `<PropertyGroup Label="Globals">` section:
   ```xml
   <VcpkgEnabled>true</VcpkgEnabled>
   <VcpkgUseManifestMode>true</VcpkgUseManifestMode>
   ```
3. If missing, the project needs to be reconfigured

#### Solution 7: Check vcpkg.json Location
The vcpkg.json manifest file MUST be in the same directory as the .sln file:

```
Broken-Arrow-Software/
├── SkinningStudio.sln          ← Solution file
├── vcpkg.json                  ← Must be here!
├── SkinningStudio/
└── RuntimeSample/
```

If vcpkg.json is in the wrong location, move it to the solution directory.

### Error: SDL2.h not found

Similar to the nlohmann/json error - follow the same solutions above. The issue is vcpkg dependencies not being installed.

### Error: imgui.h not found

Same root cause - vcpkg dependencies. Solutions:

1. Verify imgui features in vcpkg.json:
   ```json
   {
     "name": "imgui",
     "features": ["docking-experimental", "sdl2-binding", "opengl3-binding"]
   }
   ```
2. Follow Solution 1-4 above

### Build Warnings: "vcpkg integration is not enabled"

If you see this warning in the build output:

1. Run `vcpkg integrate install` from your vcpkg directory
2. Restart Visual Studio
3. The warning should disappear

### Slow First Build

First build with vcpkg manifest mode can take 5-15 minutes as it:
- Downloads source code for all dependencies
- Builds each library from source
- This is normal and only happens once

Subsequent builds will be much faster.

## Runtime Issues

### Error: SDL2.dll not found

vcpkg should automatically copy DLLs, but if it doesn't:

1. Locate SDL2.dll in `vcpkg_installed\x64-windows\bin\`
2. Copy it to your output directory (`bin\Debug\` or `bin\Release\`)

### Application crashes on startup

1. Verify your graphics drivers are up to date
2. Check that your GPU supports OpenGL 3.3 or higher
3. Run in Debug mode to see detailed error messages

### Docking not working

Verify ImGui was built with docking support:
- Check vcpkg.json has `"docking-experimental"` feature
- Clean and rebuild to ensure correct ImGui version

## Advanced Troubleshooting

### Check vcpkg Install Status

To see what vcpkg has installed:

```bash
vcpkg list
```

You should see:
- nlohmann-json:x64-windows
- sdl2:x64-windows
- glew:x64-windows
- imgui[core,docking-experimental,opengl3-binding,sdl2-binding]:x64-windows

### Enable vcpkg Debug Output

To see detailed vcpkg operations:

1. Tools > Options > Projects and Solutions > Build and Run
2. Set "MSBuild project build output verbosity" to "Detailed"
3. Rebuild - you'll see vcpkg operations in the Output window

### Reset vcpkg Cache

If nothing else works, reset vcpkg:

```bash
# Delete installed packages
rmdir /s vcpkg_installed

# In vcpkg directory, clear cache
vcpkg remove --outdated
```

Then rebuild the solution.

## Getting More Help

If you've tried all solutions and still have issues:

1. Check the build output (View > Output) for specific error messages
2. Verify your Visual Studio version (2022 or later required)
3. Ensure you have the "Desktop development with C++" workload installed
4. Check that you have sufficient disk space (vcpkg needs ~2GB)

### System Requirements

- Visual Studio 2022 or later (v143 toolset)
- Windows 10 or later
- ~2GB free disk space for dependencies
- Graphics card supporting OpenGL 3.3+
- Administrator rights (for vcpkg integrate install)
