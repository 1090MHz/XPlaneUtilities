cmake_minimum_required(VERSION 3.20)
project(XPlaneUtilities 
    VERSION 1.0.0
    DESCRIPTION "Modern C++ utility library for X-Plane plugin development"
    LANGUAGES CXX
)

# Modern C++ standards - using C++17 for better X-Plane compatibility
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

# Build options
option(XPLANE_UTILITIES_BUILD_TESTS "Build tests" ON)
option(XPLANE_UTILITIES_BUILD_EXAMPLES "Build examples" ON)

# Find dependencies
# Note: When built as part of ImGuiSimBrief, spdlog and fmt are already available
# When built standalone, add them as subdirectories from parent
if(TARGET spdlog::spdlog)
    # spdlog is already available (built as submodule)
    message(STATUS "Using spdlog from parent project")
else()
    # Try to find spdlog in parent directory
    if(EXISTS "${CMAKE_CURRENT_SOURCE_DIR}/../spdlog/CMakeLists.txt")
        message(STATUS "Adding spdlog from parent directory")
        add_subdirectory("${CMAKE_CURRENT_SOURCE_DIR}/../spdlog" "${CMAKE_CURRENT_BINARY_DIR}/spdlog" EXCLUDE_FROM_ALL)
    else()
        find_package(spdlog REQUIRED)
    endif()
endif()

if(TARGET fmt::fmt)
    # fmt is already available (built as submodule)
    message(STATUS "Using fmt from parent project")
else()
    # Try to find fmt in parent directory
    if(EXISTS "${CMAKE_CURRENT_SOURCE_DIR}/../fmt/CMakeLists.txt")
        message(STATUS "Adding fmt from parent directory")
        add_subdirectory("${CMAKE_CURRENT_SOURCE_DIR}/../fmt" "${CMAKE_CURRENT_BINARY_DIR}/fmt" EXCLUDE_FROM_ALL)
    else()
        find_package(fmt REQUIRED)
    endif()
endif()

# X-Plane SDK path (required for this library)
if(NOT DEFINED XPLANE_SDK_PATH)
    set(XPLANE_SDK_PATH "${CMAKE_CURRENT_SOURCE_DIR}/../XPSDK" CACHE PATH "Path to X-Plane SDK")
endif()

# Require X-Plane SDK
if(NOT EXISTS "${XPLANE_SDK_PATH}/CHeaders")
    message(FATAL_ERROR "X-Plane SDK not found at: ${XPLANE_SDK_PATH}. Please set XPLANE_SDK_PATH to the correct location.")
endif()

message(STATUS "X-Plane SDK found at: ${XPLANE_SDK_PATH}")

# Library sources
set(XPLANE_UTILITIES_SOURCES
    src/XPlaneLog.cpp
    src/MenuHandler.cpp
)

# Library headers
set(XPLANE_UTILITIES_HEADERS
    include/XPlaneUtilities/XPlaneLog.h
    include/XPlaneUtilities/MenuHandler.h
    include/XPlaneUtilities/XPlaneUtilities.h
)

# Create library target
add_library(XPlaneUtilities ${XPLANE_UTILITIES_SOURCES} ${XPLANE_UTILITIES_HEADERS})

# Set target properties
set_target_properties(XPlaneUtilities PROPERTIES
    VERSION ${PROJECT_VERSION}
    SOVERSION ${PROJECT_VERSION_MAJOR}
    OUTPUT_NAME "xplane-utilities"
)

# Include directories
target_include_directories(XPlaneUtilities
    PUBLIC 
        $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/include>
        $<INSTALL_INTERFACE:include>
)

# Link dependencies
target_link_libraries(XPlaneUtilities
    PRIVATE
        spdlog::spdlog
        fmt::fmt
)

# X-Plane SDK integration (required)
target_include_directories(XPlaneUtilities
    PRIVATE
        "${XPLANE_SDK_PATH}/CHeaders/XPLM"
        "${XPLANE_SDK_PATH}/CHeaders/Widgets"
)

# X-Plane SDK platform definitions
if(WIN32)
    target_compile_definitions(XPlaneUtilities PRIVATE IBM=1 XPLM200=1 XPLM210=1 XPLM300=1 XPLM301=1 XPLM303=1 XPLM400=1)
elseif(APPLE)
    target_compile_definitions(XPlaneUtilities PRIVATE APL=1 XPLM200=1 XPLM210=1 XPLM300=1 XPLM301=1 XPLM303=1 XPLM400=1)
elseif(UNIX)
    target_compile_definitions(XPlaneUtilities PRIVATE LIN=1 XPLM200=1 XPLM210=1 XPLM300=1 XPLM301=1 XPLM303=1 XPLM400=1)
endif()

# Compiler-specific options
if(MSVC)
    target_compile_options(XPlaneUtilities PRIVATE /W4)
else()
    target_compile_options(XPlaneUtilities PRIVATE -Wall -Wextra -Wpedantic)
endif()

# Tests
if(XPLANE_UTILITIES_BUILD_TESTS)
    enable_testing()
    add_subdirectory(tests)
endif()

# Examples
if(XPLANE_UTILITIES_BUILD_EXAMPLES)
    add_subdirectory(examples)
endif()

# Optional: Install configuration (disabled by default for submodule usage)
option(XPLANE_UTILITIES_INSTALL "Generate install target" OFF)

if(XPLANE_UTILITIES_INSTALL)
    # Install configuration
    include(GNUInstallDirs)
    include(CMakePackageConfigHelpers)

    # Install targets
    install(TARGETS XPlaneUtilities
        EXPORT XPlaneUtilitiesTargets
        LIBRARY DESTINATION ${CMAKE_INSTALL_LIBDIR}
        ARCHIVE DESTINATION ${CMAKE_INSTALL_LIBDIR}
        RUNTIME DESTINATION ${CMAKE_INSTALL_BINDIR}
        INCLUDES DESTINATION ${CMAKE_INSTALL_INCLUDEDIR}
    )

    # Install headers
    install(DIRECTORY include/
        DESTINATION ${CMAKE_INSTALL_INCLUDEDIR}
        FILES_MATCHING PATTERN "*.h"
    )

    # Export targets
    install(EXPORT XPlaneUtilitiesTargets
        FILE XPlaneUtilitiesTargets.cmake
        NAMESPACE XPlaneUtilities::
        DESTINATION ${CMAKE_INSTALL_LIBDIR}/cmake/XPlaneUtilities
    )

    # Create package config
    configure_package_config_file(
        "${CMAKE_CURRENT_SOURCE_DIR}/cmake/XPlaneUtilitiesConfig.cmake.in"
        "${CMAKE_CURRENT_BINARY_DIR}/XPlaneUtilitiesConfig.cmake"
        INSTALL_DESTINATION ${CMAKE_INSTALL_LIBDIR}/cmake/XPlaneUtilities
    )

    write_basic_package_version_file(
        "${CMAKE_CURRENT_BINARY_DIR}/XPlaneUtilitiesConfigVersion.cmake"
        VERSION ${PROJECT_VERSION}
        COMPATIBILITY SameMajorVersion
    )

    install(FILES
        "${CMAKE_CURRENT_BINARY_DIR}/XPlaneUtilitiesConfig.cmake"
        "${CMAKE_CURRENT_BINARY_DIR}/XPlaneUtilitiesConfigVersion.cmake"
        DESTINATION ${CMAKE_INSTALL_LIBDIR}/cmake/XPlaneUtilities
    )
endif()
