cmake_minimum_required(VERSION 3.22)

project(AmpegSVT VERSION 1.0.0)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# Add JUCE framework
add_subdirectory(JUCE)

# Enable all formats for development
juce_add_plugin(AmpegSVT
    COMPANY_NAME "Broken Arrow Software"
    IS_SYNTH FALSE
    NEEDS_MIDI_INPUT FALSE
    NEEDS_MIDI_OUTPUT FALSE
    IS_MIDI_EFFECT FALSE
    EDITOR_WANTS_KEYBOARD_FOCUS FALSE
    COPY_PLUGIN_AFTER_BUILD TRUE
    PLUGIN_MANUFACTURER_CODE Brka
    PLUGIN_CODE Asvt
    FORMATS VST3 Standalone
    PRODUCT_NAME "Ampeg SVT Bass Amp")

# Source files
target_sources(AmpegSVT
    PRIVATE
        Source/PluginProcessor.cpp
        Source/PluginEditor.cpp
        Source/DSP/InputStage.cpp
        Source/DSP/TubePreamp.cpp
        Source/DSP/ToneStack.cpp
        Source/DSP/PowerAmp.cpp
        Source/DSP/CabSimulator.cpp
        Source/DSP/Oversampler.cpp
        Source/Components/CustomKnob.cpp
        Source/Components/CustomSwitch.cpp
        Source/Components/LevelMeter.cpp)

# Header files (for IDE organization)
target_sources(AmpegSVT
    PRIVATE
        Source/PluginProcessor.h
        Source/PluginEditor.h
        Source/DSP/InputStage.h
        Source/DSP/TubePreamp.h
        Source/DSP/ToneStack.h
        Source/DSP/PowerAmp.h
        Source/DSP/CabSimulator.h
        Source/DSP/Oversampler.h
        Source/Components/CustomKnob.h
        Source/Components/CustomSwitch.h
        Source/Components/LevelMeter.h)

# JUCE modules
target_compile_definitions(AmpegSVT
    PUBLIC
        JUCE_WEB_BROWSER=0
        JUCE_USE_CURL=0
        JUCE_VST3_CAN_REPLACE_VST2=0)

target_link_libraries(AmpegSVT
    PRIVATE
        juce::juce_audio_utils
        juce::juce_audio_formats
        juce::juce_dsp
    PUBLIC
        juce::juce_recommended_config_flags
        juce::juce_recommended_lto_flags
        juce::juce_recommended_warning_flags)

# Copy assets to build directory
add_custom_command(TARGET AmpegSVT POST_BUILD
    COMMAND ${CMAKE_COMMAND} -E copy_directory
    ${CMAKE_SOURCE_DIR}/Assets
    $<TARGET_FILE_DIR:AmpegSVT>/Assets)

add_custom_command(TARGET AmpegSVT POST_BUILD
    COMMAND ${CMAKE_COMMAND} -E copy_directory
    ${CMAKE_SOURCE_DIR}/Presets
    $<TARGET_FILE_DIR:AmpegSVT>/Presets)
