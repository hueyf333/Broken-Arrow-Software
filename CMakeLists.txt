cmake_minimum_required(VERSION 3.15)

project(SUPERBASSMAN VERSION 1.0.0)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# Add JUCE
add_subdirectory(JUCE)

# Define the plugin
juce_add_plugin(SuperBassman
    COMPANY_NAME "Broken Arrow Software"
    IS_SYNTH FALSE
    NEEDS_MIDI_INPUT FALSE
    NEEDS_MIDI_OUTPUT FALSE
    IS_MIDI_EFFECT FALSE
    EDITOR_WANTS_KEYBOARD_FOCUS FALSE
    COPY_PLUGIN_AFTER_BUILD TRUE
    PLUGIN_MANUFACTURER_CODE BrAr
    PLUGIN_CODE SBsm
    FORMATS VST3 Standalone
    PRODUCT_NAME "SuperBassman"
    BUNDLE_ID com.brokenarrow.SuperBassman
    COMPANY_WEBSITE "https://github.com/hueyf333/Broken-Arrow-Software"
    COMPANY_EMAIL ""
    PLUGIN_NAME "SuperBassman"
    DESCRIPTION "Fender Super Bassman Bass Head Emulation"
)

# Add source files
target_sources(SuperBassman
    PRIVATE
        Source/PluginProcessor.cpp
        Source/PluginEditor.cpp
        Source/UI/CustomLookAndFeel.cpp
        Source/UI/LevelMeter.cpp
)

# Include directories
target_include_directories(SuperBassman
    PRIVATE
        Source
        Source/DSP
        Source/UI
)

# Preprocessor definitions
target_compile_definitions(SuperBassman
    PUBLIC
        JUCE_WEB_BROWSER=0
        JUCE_USE_CURL=0
        JUCE_VST3_CAN_REPLACE_VST2=0
        JUCE_DISPLAY_SPLASH_SCREEN=0
        JUCE_REPORT_APP_USAGE=0
)

# Link JUCE modules
target_link_libraries(SuperBassman
    PRIVATE
        juce::juce_audio_basics
        juce::juce_audio_devices
        juce::juce_audio_formats
        juce::juce_audio_plugin_client
        juce::juce_audio_processors
        juce::juce_audio_utils
        juce::juce_core
        juce::juce_data_structures
        juce::juce_dsp
        juce::juce_events
        juce::juce_graphics
        juce::juce_gui_basics
        juce::juce_gui_extra
    PUBLIC
        juce::juce_recommended_config_flags
        juce::juce_recommended_lto_flags
        juce::juce_recommended_warning_flags
)

# Set C++20 features
target_compile_features(SuperBassman PRIVATE cxx_std_20)
