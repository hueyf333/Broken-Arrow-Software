cmake_minimum_required(VERSION 3.22)

project(SVT_CL VERSION 1.0.0)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# Add JUCE
add_subdirectory(JUCE)

# Plugin formats to build
set(FORMATS VST3 Standalone)

# Create the plugin target
juce_add_plugin(SVT-CL
    COMPANY_NAME "Broken Arrow Software"
    PLUGIN_MANUFACTURER_CODE BArr
    PLUGIN_CODE Svtc
    FORMATS ${FORMATS}
    PRODUCT_NAME "SVT-CL"
    VST3_CATEGORIES "Fx" "Distortion"
    NEEDS_MIDI_INPUT FALSE
    NEEDS_MIDI_OUTPUT FALSE
    IS_SYNTH FALSE
    IS_MIDI_EFFECT FALSE
    EDITOR_WANTS_KEYBOARD_FOCUS FALSE
    COPY_PLUGIN_AFTER_BUILD TRUE
    MICROPHONE_PERMISSION_ENABLED TRUE
)

# Source files
target_sources(SVT-CL PRIVATE
    SVT-CL/Source/PluginProcessor.cpp
    SVT-CL/Source/PluginEditor.cpp
    SVT-CL/Source/DSP/TubePreamp.cpp
    SVT-CL/Source/DSP/ToneStack.cpp
    SVT-CL/Source/DSP/PowerAmp.cpp
    SVT-CL/Source/DSP/CabSimulator.cpp
    SVT-CL/Source/DSP/SVTProcessor.cpp
)

# Header files (for IDE organization)
target_sources(SVT-CL PRIVATE
    SVT-CL/Source/PluginProcessor.h
    SVT-CL/Source/PluginEditor.h
    SVT-CL/Source/DSP/TubePreamp.h
    SVT-CL/Source/DSP/ToneStack.h
    SVT-CL/Source/DSP/PowerAmp.h
    SVT-CL/Source/DSP/CabSimulator.h
    SVT-CL/Source/DSP/SVTProcessor.h
)

# Include directories
target_include_directories(SVT-CL PRIVATE
    SVT-CL/Source
    SVT-CL/Source/DSP
)

# Compiler definitions
target_compile_definitions(SVT-CL PUBLIC
    JUCE_WEB_BROWSER=0
    JUCE_USE_CURL=0
    JUCE_VST3_CAN_REPLACE_VST2=0
    JUCE_DISPLAY_SPLASH_SCREEN=0
    JUCE_REPORT_APP_USAGE=0
)

# Link JUCE modules
target_link_libraries(SVT-CL PRIVATE
    juce::juce_audio_utils
    juce::juce_dsp
    juce::juce_gui_basics
    juce::juce_audio_plugin_client
PUBLIC
    juce::juce_recommended_config_flags
    juce::juce_recommended_lto_flags
    juce::juce_recommended_warning_flags
)

# Optional: Build DSP test executable
option(BUILD_TESTS "Build DSP tests" ON)

if(BUILD_TESTS)
    add_executable(dsp_test
        test/dsp_test.cpp
        SVT-CL/Source/DSP/TubePreamp.cpp
        SVT-CL/Source/DSP/ToneStack.cpp
        SVT-CL/Source/DSP/PowerAmp.cpp
        SVT-CL/Source/DSP/CabSimulator.cpp
        SVT-CL/Source/DSP/SVTProcessor.cpp
    )
    
    target_include_directories(dsp_test PRIVATE
        SVT-CL/Source
        SVT-CL/Source/DSP
    )
    
    target_compile_definitions(dsp_test PRIVATE
        JUCE_STANDALONE_APPLICATION=1
        JUCE_USE_CURL=0
        JUCE_WEB_BROWSER=0
    )
    
    target_link_libraries(dsp_test PRIVATE
        juce::juce_audio_basics
        juce::juce_dsp
        juce::juce_core
    )
endif()
