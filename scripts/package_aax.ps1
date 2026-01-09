# AAX Plugin Packaging Script
# Creates .aaxplugin bundle structure

param(
    [string]$Configuration,
    [string]$OutputDir,
    [string]$TargetName
)

Write-Host "Packaging AAX Plugin..."
Write-Host "Configuration: $Configuration"
Write-Host "Output Directory: $OutputDir"
Write-Host "Target Name: $TargetName"

# Create bundle directory structure
$bundlePath = Join-Path $OutputDir "$TargetName.aaxplugin"
$contentsPath = Join-Path $bundlePath "Contents"
$win64Path = Join-Path $contentsPath "x64"

Write-Host "Creating bundle structure at: $bundlePath"

# Create directories
New-Item -ItemType Directory -Force -Path $bundlePath | Out-Null
New-Item -ItemType Directory -Force -Path $contentsPath | Out-Null
New-Item -ItemType Directory -Force -Path $win64Path | Out-Null

# Copy DLL to proper location
$sourceDll = Join-Path $OutputDir "$TargetName.aaxplugin"
$destDll = Join-Path $win64Path "$TargetName.aaxplugin"

if (Test-Path $sourceDll) {
    Write-Host "Copying plugin DLL..."
    Copy-Item $sourceDll $destDll -Force
} else {
    Write-Error "Source DLL not found: $sourceDll"
    exit 1
}

# Create PkgInfo file
$pkgInfoPath = Join-Path $contentsPath "PkgInfo"
Set-Content -Path $pkgInfoPath -Value "TDMwPTul" -NoNewline

# Create Info.plist
$infoPlistPath = Join-Path $contentsPath "Info.plist"
$infoPlistContent = @"
<?xml version="1.0" encoding="UTF-8"?>
<!DOCTYPE plist PUBLIC "-//Apple//DTD PLIST 1.0//EN" "http://www.apple.com/DTDs/PropertyList-1.0.dtd">
<plist version="1.0">
<dict>
    <key>CFBundleDevelopmentRegion</key>
    <string>English</string>
    <key>CFBundleExecutable</key>
    <string>$TargetName</string>
    <key>CFBundleIdentifier</key>
    <string>com.brokenarrow.patchwrap</string>
    <key>CFBundleInfoDictionaryVersion</key>
    <string>6.0</string>
    <key>CFBundleName</key>
    <string>Patchwrap</string>
    <key>CFBundlePackageType</key>
    <string>TDMw</string>
    <key>CFBundleShortVersionString</key>
    <string>1.0.0</string>
    <key>CFBundleSignature</key>
    <string>PTul</string>
    <key>CFBundleVersion</key>
    <string>1.0.0</string>
    <key>CSResourcesFileMapped</key>
    <true/>
</dict>
</plist>
"@

Set-Content -Path $infoPlistPath -Value $infoPlistContent

Write-Host "AAX Plugin bundle created successfully!"
Write-Host "Bundle location: $bundlePath"
