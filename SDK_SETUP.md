# Patchwrap SDK Configuration

This file documents the SDK paths required for building Patchwrap.

## AAX SDK

The AAX SDK must be obtained from Avid and set up manually.

**Download**: https://developer.avid.com/
**Required Version**: AAX SDK 2.4.0 or later
**License**: Avid Developer License (free registration required)

### Setup Instructions

1. Register at https://developer.avid.com/
2. Download the AAX SDK
3. Extract to a local directory (e.g., `C:\SDKs\AAX_SDK`)
4. Set the environment variable `AAX_SDK_PATH` to point to the SDK root

**Environment Variable**:
```
AAX_SDK_PATH = C:\SDKs\AAX_SDK
```

### Expected AAX SDK Structure

```
AAX_SDK/
├── Interfaces/
│   ├── AAX.h
│   ├── AAX_IEffectParameters.h
│   ├── AAX_IController.h
│   └── ACF/
├── Libs/
│   ├── Debug/
│   │   └── AAXLibrary_libcpp.lib
│   └── Release/
│       └── AAXLibrary_libcpp.lib
└── Documentation/
```

### Project References

The AAX SDK is referenced in the `Patchwrap_AAX.vcxproj` file:

- Include directories: `$(AAX_SDK_PATH)\Interfaces` and `$(AAX_SDK_PATH)\Interfaces\ACF`
- Library directories: `$(AAX_SDK_PATH)\Libs\Debug` or `$(AAX_SDK_PATH)\Libs\Release`
- Linked library: `AAXLibrary_libcpp.lib`

## VST3 SDK

The VST3 SDK is open source and can be automatically fetched.

**Repository**: https://github.com/steinbergmedia/vst3sdk
**License**: Dual license (GPLv3 or Steinberg VST3 License)
**Version**: Latest stable (3.7.x or later)

### Setup Instructions

**Option 1: Automatic (Recommended)**
```powershell
cd scripts
.\fetch_vst3sdk.ps1
```

**Option 2: Git Submodule**
```bash
git submodule update --init --recursive
```

**Option 3: Manual Clone**
```bash
cd third_party
git clone --recursive https://github.com/steinbergmedia/vst3sdk.git
```

### Expected VST3 SDK Structure

```
third_party/vst3sdk/
├── base/
├── public.sdk/
│   ├── source/
│   └── vst/
├── pluginterfaces/
│   └── vst/
└── CMakeLists.txt
```

### Project References

The VST3 SDK is referenced in both project files:

- `Patchwrap_Core.vcxproj`: `$(SolutionDir)third_party\vst3sdk`
- `Patchwrap_AAX.vcxproj`: `$(SolutionDir)third_party\vst3sdk`

Key headers used:
- `pluginterfaces/vst/ivstaudioprocessor.h`
- `pluginterfaces/vst/ivsteditcontroller.h`
- `pluginterfaces/vst/ivstcomponent.h`
- `public.sdk/source/vst/hosting/module.h`

## Build Configuration

### Include Paths

**Patchwrap_Core**:
```
$(ProjectDir)include
$(SolutionDir)third_party\vst3sdk
```

**Patchwrap_AAX**:
```
$(ProjectDir)include
$(SolutionDir)Patchwrap_Core\include
$(AAX_SDK_PATH)\Interfaces
$(AAX_SDK_PATH)\Interfaces\ACF
$(SolutionDir)third_party\vst3sdk
```

### Library Paths

**Patchwrap_AAX**:
```
$(AAX_SDK_PATH)\Libs\Debug         (Debug configuration)
$(AAX_SDK_PATH)\Libs\Release       (Release configuration)
```

### Linked Libraries

**Patchwrap_AAX**:
```
AAXLibrary_libcpp.lib              (from AAX SDK)
Patchwrap_Core.lib                 (from our build)
```

## Compiler Settings

### Platform
- **x64 only** (AAX requires 64-bit)

### C++ Standard
- **C++17** (`/std:c++17`)

### Runtime Library
- **Debug**: Multi-threaded Debug (`/MTd`)
- **Release**: Multi-threaded (`/MT`)

This matches AAX SDK requirements.

### Preprocessor Defines

**Debug**:
```
_DEBUG
DEVELOPMENT=1
```

**Release**:
```
NDEBUG
RELEASE=1
```

## Output Structure

```
Build/
├── Debug/
│   ├── Patchwrap_Core.lib
│   ├── Patchwrap_AAX/              (intermediate)
│   └── Patchwrap.aaxplugin/        (final bundle)
│       └── Contents/
│           ├── x64/
│           │   └── Patchwrap.aaxplugin
│           ├── Info.plist
│           └── PkgInfo
└── Release/
    └── (same structure as Debug)
```

## Post-Build Packaging

The `scripts\package_aax.ps1` PowerShell script is executed after building Patchwrap_AAX:

1. Creates `.aaxplugin` bundle directory structure
2. Copies the built DLL to `Contents/x64/`
3. Generates `Info.plist` with bundle metadata
4. Creates `PkgInfo` with bundle type

## Troubleshooting

### AAX SDK Not Found

**Error**: 
```
Cannot open include file: 'AAX.h': No such file or directory
```

**Solution**:
1. Verify AAX SDK is downloaded and extracted
2. Check `AAX_SDK_PATH` environment variable is set
3. Restart Visual Studio after setting environment variable
4. Verify path in environment variable points to SDK root (not Interfaces subfolder)

### VST3 SDK Not Found

**Error**:
```
Cannot open include file: 'pluginterfaces/vst/ivstaudioprocessor.h'
```

**Solution**:
1. Run `scripts\fetch_vst3sdk.ps1`
2. Or manually clone VST3 SDK to `third_party\vst3sdk`
3. Ensure `--recursive` flag was used to clone submodules

### Linker Errors

**Error**:
```
unresolved external symbol ... AAXLibrary_libcpp.lib
```

**Solution**:
1. Verify AAX SDK libraries exist in `$(AAX_SDK_PATH)\Libs\Debug` and `Release`
2. Check platform is set to x64 (not Win32)
3. Verify runtime library setting matches AAX SDK (Multi-threaded)

## Additional Notes

### AAX Signing

Production AAX plugins must be signed by Avid using PACE anti-piracy.

- Development: Can use unsigned plugins locally
- Distribution: Must sign with Avid developer certificate

Contact Avid for PACE signing information.

### VST3 Hosting License

When using VST3 SDK for hosting (not creating plugins), you can choose:
- **GPLv3**: Open source license
- **Steinberg License**: Proprietary agreement with Steinberg

This project is compatible with both. Choose based on your distribution needs.

### Pro Tools Compatibility

- **Minimum**: Pro Tools 11 (first version with AAX)
- **Recommended**: Pro Tools 2018.1 or later
- **Platform**: Windows x64 only (macOS not yet supported)
