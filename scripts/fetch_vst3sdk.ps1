# VST3 SDK Fetch Script
# Downloads and sets up VST3 SDK for building

param(
    [string]$SdkPath = "..\third_party\vst3sdk"
)

Write-Host "VST3 SDK Setup"
Write-Host "=============="

$thirdPartyDir = Join-Path $PSScriptRoot "..\third_party"
$vst3SdkDir = Join-Path $thirdPartyDir "vst3sdk"

# Create third_party directory if it doesn't exist
if (-not (Test-Path $thirdPartyDir)) {
    Write-Host "Creating third_party directory..."
    New-Item -ItemType Directory -Path $thirdPartyDir | Out-Null
}

# Check if VST3 SDK already exists
if (Test-Path $vst3SdkDir) {
    Write-Host "VST3 SDK already exists at: $vst3SdkDir"
    Write-Host "To re-download, delete the directory and run this script again."
    exit 0
}

Write-Host ""
Write-Host "VST3 SDK Download"
Write-Host "-----------------"
Write-Host "The VST3 SDK needs to be downloaded from Steinberg."
Write-Host ""
Write-Host "Option 1: Git Clone (Recommended)"
Write-Host "  Run: git clone --recursive https://github.com/steinbergmedia/vst3sdk.git $vst3SdkDir"
Write-Host ""
Write-Host "Option 2: Manual Download"
Write-Host "  1. Go to: https://github.com/steinbergmedia/vst3sdk"
Write-Host "  2. Download the repository"
Write-Host "  3. Extract to: $vst3SdkDir"
Write-Host ""

# Attempt to clone if git is available
$gitAvailable = $null -ne (Get-Command git -ErrorAction SilentlyContinue)

if ($gitAvailable) {
    Write-Host "Git is available. Attempting to clone VST3 SDK..."
    Write-Host ""
    
    try {
        git clone --recursive https://github.com/steinbergmedia/vst3sdk.git $vst3SdkDir
        
        if (Test-Path $vst3SdkDir) {
            Write-Host ""
            Write-Host "VST3 SDK cloned successfully!"
            Write-Host "Location: $vst3SdkDir"
            exit 0
        }
    }
    catch {
        Write-Host "Failed to clone VST3 SDK: $_"
        Write-Host "Please download manually using the instructions above."
        exit 1
    }
}
else {
    Write-Host "Git is not available. Please download VST3 SDK manually."
    exit 1
}
