cmake_minimum_required(VERSION 3.22)

project(BrokenArrowSkinLab VERSION 1.0.0)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# Add JUCE
add_subdirectory(JUCE)

# Create the application
juce_add_gui_app(BrokenArrowSkinLab
    PRODUCT_NAME "BrokenArrowSkinLab"
    COMPANY_NAME "Broken Arrow Software"
    BUNDLE_ID "com.brokenarrowsoftware.skinlab"
)

# Source files
target_sources(BrokenArrowSkinLab PRIVATE
    BrokenArrowSkinLab/Source/Main.cpp
    BrokenArrowSkinLab/Source/MainComponent.cpp
)

# Required JUCE modules
target_link_libraries(BrokenArrowSkinLab PRIVATE
    juce::juce_gui_basics
    juce::juce_gui_extra
    juce::juce_graphics
    juce::juce_data_structures
    juce::juce_events
    juce::juce_core
)

# Compiler definitions
target_compile_definitions(BrokenArrowSkinLab PRIVATE
    JUCE_WEB_BROWSER=0
    JUCE_USE_CURL=0
    JUCE_APPLICATION_NAME_STRING="$<TARGET_PROPERTY:BrokenArrowSkinLab,JUCE_PRODUCT_NAME>"
    JUCE_APPLICATION_VERSION_STRING="$<TARGET_PROPERTY:BrokenArrowSkinLab,JUCE_VERSION>"
)
