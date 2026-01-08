# Build Notes for BASL Designer

## Quick Start (Windows with Qt 6.x and VS2022)

### Prerequisites Checklist
- [ ] Visual Studio 2022 installed with "Desktop development with C++" workload
- [ ] Qt 6.x for MSVC 2019 64-bit (works with VS2022)
- [ ] Qt Visual Studio Tools extension (optional but helpful)

### Qt Installation
1. Download Qt Online Installer from https://www.qt.io/download
2. During installation, select:
   - Qt 6.x for Windows
   - MSVC 2019 64-bit component
   - Qt Creator (optional)
3. Note the installation path (e.g., `C:\Qt\6.6.1\msvc2019_64`)

### Environment Setup

#### Option A: Using Qt VS Tools Extension
1. Install "Qt Visual Studio Tools" from Extensions → Manage Extensions
2. Restart Visual Studio
3. Go to Extensions → Qt VS Tools → Qt Versions
4. Click "+" and add your Qt installation path
5. Set it as default

#### Option B: Manual Configuration
1. Add Qt bin directory to PATH:
   ```
   C:\Qt\6.6.1\msvc2019_64\bin
   ```
2. Or run Qt command prompt before opening Visual Studio

### Building

1. **Clone Repository**
   ```bash
   git clone https://github.com/hueyf333/Broken-Arrow-Software.git
   cd Broken-Arrow-Software
   ```

2. **Open Solution**
   - Double-click `BrokenArrowSoftware.sln`
   - Or open in Visual Studio 2022

3. **Configure Qt (if not using Qt VS Tools)**
   - Right-click BASLDesigner project → Properties
   - Ensure C++ Language Standard is set to C++20
   - Check that Qt paths are configured

4. **Build**
   - Select configuration: Debug or Release
   - Platform: x64
   - Build → Build Solution (Ctrl+Shift+B)
   - First build may take a few minutes for Qt MOC generation

5. **Run**
   - Press F5 to run with debugging
   - Or Ctrl+F5 to run without debugging
   - Or navigate to `bin\x64\Release\BASLDesigner.exe`

### Build Output
- Executables: `bin\x64\Debug\` or `bin\x64\Release\`
- Intermediate: `obj\BASLDesigner\x64\Debug\` or `obj\BASLDesigner\x64\Release\`
- MOC files: Generated automatically by Qt build integration

### Common Build Issues

#### Issue: "Qt is not configured"
**Solution**: 
- Install Qt VS Tools extension
- Or manually add Qt paths in project properties
- Verify QTDIR environment variable

#### Issue: "Cannot find moc.exe"
**Solution**:
- Ensure Qt bin directory is in PATH
- Verify Qt installation is complete
- Check Qt version matches project settings

#### Issue: "LNK2019 unresolved external symbol"
**Solution**:
- Ensure all .cpp files are in project
- Check Qt modules are linked (Core, Gui, Widgets)
- Rebuild entire solution (Clean → Build)

#### Issue: "C++20 features not recognized"
**Solution**:
- Update Visual Studio to latest version
- Verify project properties: C/C++ → Language → C++ Language Standard = C++20

### Runtime Requirements

To run the application on a machine without Qt SDK:

1. **Required DLLs** (copy from Qt bin directory):
   - Qt6Core.dll
   - Qt6Gui.dll
   - Qt6Widgets.dll
   - vcruntime140.dll
   - msvcp140.dll

2. **Qt Plugins** (copy folders to exe directory):
   - platforms/qwindows.dll
   - styles/qwindowsvistastyle.dll

3. **Or use windeployqt**:
   ```bash
   cd bin\x64\Release
   windeployqt BASLDesigner.exe
   ```

### Development Workflow

1. **Make Changes**
   - Edit source files in `BASLDesigner/src` or `BASLDesigner/include`

2. **Build**
   - Press Ctrl+Shift+B

3. **Test**
   - Run application (F5)
   - Use Tools → Run Self Test to verify functionality

4. **Debug**
   - Set breakpoints in source files
   - Press F5 to debug
   - Use Qt debug visualizers for Qt types

### Qt MOC (Meta-Object Compiler)

Qt classes with Q_OBJECT macro require MOC processing:
- `.h` files marked as `<QtMoc Include>` in .vcxproj
- MOC generates `moc_*.cpp` files automatically
- These are compiled and linked automatically

### Adding New Files

#### New Qt Widget Class
1. Add `MyWidget.h` to `include/`
2. Add `MyWidget.cpp` to `src/`
3. Update `BASLDesigner.vcxproj`:
   ```xml
   <QtMoc Include="include\MyWidget.h" />
   <ClCompile Include="src\MyWidget.cpp" />
   ```
4. Rebuild solution

#### New Non-Qt Class
1. Add header to `include/`
2. Add source to `src/`
3. Update project file as:
   ```xml
   <ClInclude Include="include\MyClass.h" />
   <ClCompile Include="src\MyClass.cpp" />
   ```

### Code Style

- C++20 modern features encouraged
- Use Qt containers (QList, QString, etc.) for Qt integration
- Follow existing naming conventions:
  - Classes: PascalCase
  - Members: m_camelCase
  - Methods: camelCase
  - Constants: kConstantName or CONSTANT_NAME

### Testing

1. **Manual Testing**
   - Run application
   - Test each menu item
   - Verify knob preview updates
   - Test export functionality

2. **Self-Test**
   - Tools → Run Self Test
   - All 4 tests should pass
   - Check temp directory for output files

3. **Sample Project**
   - File → Open → Samples/sample_knob.baslproj
   - Verify it loads correctly
   - Try exporting it

### Performance Notes

- **Debug Build**: Slower, includes debug symbols
- **Release Build**: Optimized, recommended for testing performance
- **First Run**: May be slow due to Qt initialization
- **Rendering**: Frame generation is CPU-intensive, expected

### Qt Documentation

- Qt 6 Docs: https://doc.qt.io/qt-6/
- Qt Widgets: https://doc.qt.io/qt-6/qtwidgets-index.html
- QPainter: https://doc.qt.io/qt-6/qpainter.html

### Support

For build issues:
1. Check this document
2. Verify Qt installation
3. Check Visual Studio version
4. Review Qt VS Tools documentation
5. Open issue on GitHub

## Build Configuration Summary

- **Toolset**: MSVC v143 (Visual Studio 2022)
- **Language**: C++20 (`/std:c++20`)
- **Platform**: Windows x64
- **Qt Version**: 6.x (any 6.x.x)
- **Qt Modules**: Core, GUI, Widgets
- **Character Set**: Unicode
- **Runtime Library**: Multi-threaded DLL

## Project Structure

```
BASLDesigner/
├── include/          # Header files (*.h)
├── src/              # Implementation files (*.cpp)
├── resources/        # Qt resources (icons, qrc)
└── BASLDesigner.vcxproj
```

---

**Last Updated**: January 2024
**Tested With**: Visual Studio 2022, Qt 6.6.1
