cmake_minimum_required(VERSION 3.20)
project(SkinningStudio VERSION 1.0.0 LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

# Set output directories
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin)
set(CMAKE_LIBRARY_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/lib)
set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/lib)

# Find packages
find_package(SDL2 CONFIG QUIET)
if(NOT SDL2_FOUND)
    find_package(SDL2 REQUIRED)
endif()

find_package(glad CONFIG QUIET)
if(NOT glad_FOUND)
    message(STATUS "glad not found via find_package, will use external/")
    # Add glad manually if needed
    set(GLAD_SOURCES "${CMAKE_SOURCE_DIR}/external/glad/src/glad.c")
    set(GLAD_INCLUDE "${CMAKE_SOURCE_DIR}/external/glad/include")
endif()

find_package(nlohmann_json CONFIG QUIET)
if(NOT nlohmann_json_FOUND)
    message(STATUS "nlohmann_json not found via find_package, will use external/")
    set(JSON_INCLUDE "${CMAKE_SOURCE_DIR}/external/json/include")
endif()

find_package(imgui CONFIG QUIET)
if(NOT imgui_FOUND)
    message(STATUS "imgui not found via find_package, will use external/")
    set(IMGUI_SOURCES
        "${CMAKE_SOURCE_DIR}/external/imgui/imgui.cpp"
        "${CMAKE_SOURCE_DIR}/external/imgui/imgui_demo.cpp"
        "${CMAKE_SOURCE_DIR}/external/imgui/imgui_draw.cpp"
        "${CMAKE_SOURCE_DIR}/external/imgui/imgui_tables.cpp"
        "${CMAKE_SOURCE_DIR}/external/imgui/imgui_widgets.cpp"
        "${CMAKE_SOURCE_DIR}/external/imgui/backends/imgui_impl_sdl2.cpp"
        "${CMAKE_SOURCE_DIR}/external/imgui/backends/imgui_impl_opengl3.cpp"
    )
    set(IMGUI_INCLUDE "${CMAKE_SOURCE_DIR}/external/imgui" "${CMAKE_SOURCE_DIR}/external/imgui/backends")
endif()

# Include directories
include_directories(${CMAKE_SOURCE_DIR}/include)

# Source files
file(GLOB_RECURSE SOURCES 
    "${CMAKE_SOURCE_DIR}/src/*.cpp"
)

file(GLOB_RECURSE HEADERS 
    "${CMAKE_SOURCE_DIR}/include/*.h"
    "${CMAKE_SOURCE_DIR}/include/*.hpp"
)

# Create executable
add_executable(SkinningStudio ${SOURCES} ${HEADERS})

# Link libraries
target_link_libraries(SkinningStudio PRIVATE
    SDL2::SDL2
    SDL2::SDL2main
    glad::glad
    nlohmann_json::nlohmann_json
    imgui::imgui
)

# Platform-specific settings
if(WIN32)
    # Windows-specific settings
    set_target_properties(SkinningStudio PROPERTIES
        WIN32_EXECUTABLE TRUE
    )
    # Copy SDL2 DLL to output directory on Windows
    add_custom_command(TARGET SkinningStudio POST_BUILD
        COMMAND ${CMAKE_COMMAND} -E copy_if_different
        $<TARGET_FILE:SDL2::SDL2>
        $<TARGET_FILE_DIR:SkinningStudio>
    )
elseif(APPLE)
    # macOS-specific settings
    find_library(COCOA_LIBRARY Cocoa)
    find_library(IOKIT_LIBRARY IOKit)
    find_library(COREVIDEO_LIBRARY CoreVideo)
    target_link_libraries(SkinningStudio PRIVATE
        ${COCOA_LIBRARY}
        ${IOKIT_LIBRARY}
        ${COREVIDEO_LIBRARY}
    )
elseif(UNIX)
    # Linux-specific settings
    target_link_libraries(SkinningStudio PRIVATE
        dl
        pthread
    )
endif()

# Copy assets to build directory
add_custom_command(TARGET SkinningStudio POST_BUILD
    COMMAND ${CMAKE_COMMAND} -E copy_directory
    ${CMAKE_SOURCE_DIR}/assets
    $<TARGET_FILE_DIR:SkinningStudio>/assets
)

# Installation
install(TARGETS SkinningStudio
    RUNTIME DESTINATION bin
)
install(DIRECTORY assets
    DESTINATION bin
)
