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
# Priority order:
# 1. Already available as target (from parent project)
# 2. vcpkg (if CMAKE_TOOLCHAIN_FILE is set)
# 3. Parent directory submodules (for embedding in other projects)
# 4. System-installed packages

# Check if using vcpkg
if(DEFINED CMAKE_TOOLCHAIN_FILE AND CMAKE_TOOLCHAIN_FILE MATCHES "vcpkg")
    set(USING_VCPKG TRUE)
    message(STATUS "Using vcpkg for dependencies")
else()
    set(USING_VCPKG FALSE)
endif()

# fmt must be added before spdlog so spdlog can resolve fmt::fmt correctly.
if(TARGET fmt::fmt)
    message(STATUS "Using fmt from parent project")
elseif(USING_VCPKG)
    find_package(fmt REQUIRED)
    message(STATUS "Using fmt from vcpkg")
elseif(EXISTS "${CMAKE_CURRENT_SOURCE_DIR}/../fmt/CMakeLists.txt")
    message(STATUS "Adding fmt from parent directory")
    add_subdirectory("${CMAKE_CURRENT_SOURCE_DIR}/../fmt" "${CMAKE_CURRENT_BINARY_DIR}/fmt" EXCLUDE_FROM_ALL)
else()
    find_package(fmt REQUIRED)
    message(STATUS "Using system-installed fmt")
endif()

# spdlog — added after fmt so it can consume fmt::fmt
if(TARGET spdlog::spdlog)
    message(STATUS "Using spdlog from parent project")
elseif(USING_VCPKG)
    find_package(spdlog REQUIRED)
    message(STATUS "Using spdlog from vcpkg")
elseif(EXISTS "${CMAKE_CURRENT_SOURCE_DIR}/../spdlog/CMakeLists.txt")
    message(STATUS "Adding spdlog from parent directory")
    # Tell spdlog to use external fmt in header-only mode before adding it.
    # This causes spdlog's CMake to link fmt::fmt-header-only and define
    # SPDLOG_FMT_EXTERNAL on the spdlog target (transitively applied to consumers).
    set(SPDLOG_FMT_EXTERNAL_HO ON CACHE BOOL "Use external fmt header-only library instead of bundled" FORCE)
    add_subdirectory("${CMAKE_CURRENT_SOURCE_DIR}/../spdlog" "${CMAKE_CURRENT_BINARY_DIR}/spdlog" EXCLUDE_FROM_ALL)
else()
    find_package(spdlog REQUIRED)
    message(STATUS "Using system-installed spdlog")
endif()

# X-Plane SDK path (required for this library)
if(NOT DEFINED XPLANE_SDK_PATH)
    set(XPLANE_SDK_PATH "${CMAKE_CURRENT_SOURCE_DIR}/../third_party/libs/XPSDK430" CACHE PATH "Path to X-Plane SDK")
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
    src/DataRefAccess.cpp
    src/DataRefImport.cpp
    src/DataRefExport.cpp
    src/FlightDataProvider.cpp
)

# Library headers
set(XPLANE_UTILITIES_HEADERS
    include/XPlaneUtilities/XPlaneLog.h
    include/XPlaneUtilities/MenuHandler.h
    include/XPlaneUtilities/DataRefAccess.h
    include/XPlaneUtilities/DataRefImport.h
    include/XPlaneUtilities/DataRefExport.h
    include/XPlaneUtilities/FlightDataProvider.h
    include/XPlaneUtilities/XPlaneUtilities.h
)

# Create library target
add_library(XPlaneUtilities ${XPLANE_UTILITIES_SOURCES} ${XPLANE_UTILITIES_HEADERS})

# Set target properties
set_target_properties(XPlaneUtilities PROPERTIES
    VERSION ${PROJECT_VERSION}
    SOVERSION ${PROJECT_VERSION_MAJOR}
    OUTPUT_NAME "xplane-utilities"
    POSITION_INDEPENDENT_CODE ON
)

# Include directories
target_include_directories(XPlaneUtilities
    PUBLIC 
        $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/include>
        $<INSTALL_INTERFACE:include>
)

# Link dependencies
# Header-only targets avoid compiled-lib ODR violations when multiple
# translation units (e.g. plugin.cpp) include spdlog/fmt.
# PUBLIC propagates both the targets and SPDLOG_FMT_EXTERNAL to dependent targets
# so they never accidentally pull in the compiled variants.
target_link_libraries(XPlaneUtilities
    PUBLIC
        spdlog::spdlog_header_only
        fmt::fmt-header-only
)

# Ensure spdlog consistently uses the external fmt provided above.
# SPDLOG_FMT_EXTERNAL tells spdlog to use an external fmt library
# instead of its bundled copy. We set it explicitly so behavior remains
# consistent even if dependencies come from a parent project or package manager.
target_compile_definitions(XPlaneUtilities
    PUBLIC
        SPDLOG_FMT_EXTERNAL
)

# X-Plane SDK integration (required)
target_include_directories(XPlaneUtilities
    PRIVATE
        "${XPLANE_SDK_PATH}/CHeaders/XPLM"
        "${XPLANE_SDK_PATH}/CHeaders/Widgets"
)

# X-Plane SDK platform definitions
if(WIN32)
    target_compile_definitions(XPlaneUtilities PRIVATE IBM=1 XPLM200=1 XPLM210=1 XPLM300=1 XPLM301=1 XPLM302=1 XPLM303=1 XPLM400=1 XPLM411=1 XPLM420=1 XPLM430=1)
elseif(APPLE)
    target_compile_definitions(XPlaneUtilities PRIVATE APL=1 XPLM200=1 XPLM210=1 XPLM300=1 XPLM301=1 XPLM302=1 XPLM303=1 XPLM400=1 XPLM411=1 XPLM420=1 XPLM430=1)
elseif(UNIX)
    target_compile_definitions(XPlaneUtilities PRIVATE LIN=1 XPLM200=1 XPLM210=1 XPLM300=1 XPLM301=1 XPLM302=1 XPLM303=1 XPLM400=1 XPLM411=1 XPLM420=1 XPLM430=1)
endif()

# Compiler-specific options
if(MSVC)
    target_compile_options(XPlaneUtilities PRIVATE
        /W4
        # Treat angle-bracket includes (fmt, spdlog, etc.) as external headers
        # so MSVC suppresses warnings from third-party code (e.g. C4127 from fmt).
        /external:anglebrackets
        /external:W0
    )
else()
    target_compile_options(XPlaneUtilities PRIVATE -Wall -Wextra -Wpedantic)
endif()

# Tests
# if(XPLANE_UTILITIES_BUILD_TESTS)
#     enable_testing()
#     add_subdirectory(tests)
# endif()

# Examples
# if(XPLANE_UTILITIES_BUILD_EXAMPLES)
#     add_subdirectory(examples)
# endif()

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
