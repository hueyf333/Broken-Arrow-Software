# IR Resources Directory

This directory is intended for user-loaded impulse response (IR) files.

## Supported Formats
- WAV files (mono or stereo)
- Sample rates: 44.1 kHz, 48 kHz, 88.2 kHz, 96 kHz (will be resampled as needed)
- Bit depth: 16-bit, 24-bit, 32-bit float

## Factory IRs
The plugin includes 3 built-in factory impulse responses:
1. **4x12 Vintage** - Darker, midrange-focused cabinet simulation
2. **4x12 Modern** - Brighter, tighter, more articulate
3. **2x12 Combo** - Open sound with less low-end

These are synthetically generated based on typical cabinet frequency responses.

## Loading Custom IRs
To use custom impulse responses:
1. Place your WAV files in this directory
2. Future versions will include a "Load IR" button in the plugin UI
3. For now, IRs can be loaded programmatically via the CabinetSimulator::loadUserIR() method

## Recommended IR Sources
- **3 Sigma Audio** - https://www.3sigmaaudio.com
- **Celestion Impulse Responses** - https://celestion.com
- **Ownhammer** - https://www.ownhammer.com
- **God's Cab** - Free IR collection
- **Lancaster Audio** - https://lancasteraudio.com

## IR Best Practices
- Keep IR length under 500ms for best CPU performance
- Mono IRs will be duplicated to both channels
- Stereo IRs use separate convolution per channel
- Normalize your IRs to prevent level jumps
- Test IRs at various input levels to ensure no artifacts

## Technical Details
The cabinet simulator uses JUCE's partitioned FFT convolution engine:
- Automatic latency compensation
- Efficient CPU usage via partitioned processing
- Automatic trimming of silence
- Normalization option to maintain consistent levels
