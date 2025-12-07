# XPlaneUtilities

Modern C++ utility library for X-Plane plugin development.

## Features

- **XPlaneLog**: Advanced logging system with spdlog integration and X-Plane specific sinks
- **MenuHandler**: Simplified X-Plane menu creation and management
- **Modern C++17**: Built with modern C++ standards optimized for X-Plane compatibility
- **Cross-platform**: Works on Windows, macOS, and Linux
- **CMake integration**: Easy to integrate into existing projects

## Quick Start

### Using CMake FetchContent

```cmake
include(FetchContent)
FetchContent_Declare(
    XPlaneUtilities
    GIT_REPOSITORY https://github.com/1090MHz/XPlaneUtilities.git
    GIT_TAG        main
)
FetchContent_MakeAvailable(XPlaneUtilities)

target_link_libraries(your_plugin PRIVATE XPlaneUtilities::XPlaneUtilities)
```

### Using vcpkg

The library includes a `vcpkg.json` manifest for automatic dependency management:

```bash
# Manifest mode (automatic)
cmake -B build -DCMAKE_TOOLCHAIN_FILE=[vcpkg root]/scripts/buildsystems/vcpkg.cmake

# Or classic mode
vcpkg install xplane-utilities
```

## Usage

### Logging

```cpp
#include <XPlaneUtilities/XPlaneUtilities.h>

// Initialize in your plugin
XPlaneUtilities::Initialize("MyPlugin");

// Use throughout your code
XPlaneLog::info("Plugin initialized");
XPlaneLog::error("Something went wrong: {}", error_message);

// Or use compile-time macros (debug/trace stripped in release)
XPLANE_LOG_DEBUG("Debug info: {}", value);
XPLANE_LOG_INFO("Info message");

// Shutdown on plugin cleanup
XPlaneLog::shutdown();
```

### Menu Handling

```cpp
#include <XPlaneUtilities/MenuHandler.h>

// Create a menu
MenuItem mainMenu("My Plugin");
mainMenu.addSubItem("Settings", []() {
    // Handle settings click
});
```

### Shared Logging with Other Libraries

XPlaneUtilities registers a shared logger that other libraries can use:

```cpp
// In other libraries/modules
if (auto logger = spdlog::get("xplane")) {
    logger->info("Message from library");
}
```

## Building

### Requirements

- CMake 3.20+
- C++17 compatible compiler
- X-Plane SDK (optional, for X-Plane specific features)
- vcpkg (recommended for dependencies)

### Build Steps

```bash
git clone https://github.com/1090MHz/XPlaneUtilities.git
cd XPlaneUtilities
mkdir build && cd build
cmake .. -DCMAKE_TOOLCHAIN_FILE=[vcpkg root]/scripts/buildsystems/vcpkg.cmake \
         -DXPLANE_SDK_PATH=/path/to/XPSDK410 \
         -DXPLANE_UTILITIES_BUILD_TESTS=OFF \
         -DXPLANE_UTILITIES_BUILD_EXAMPLES=OFF
cmake --build . --config Release
```

### Options

- `XPLANE_UTILITIES_BUILD_TESTS`: Build unit tests (default: ON)
- `XPLANE_UTILITIES_BUILD_EXAMPLES`: Build examples (default: ON)
- `XPLANE_UTILITIES_INSTALL`: Enable install targets (default: OFF)
- `XPLANE_SDK_PATH`: Path to X-Plane SDK (required)

### As a Git Submodule (Recommended)

This is the primary usage pattern - no installation needed:

```bash
git submodule add https://github.com/1090MHz/XPlaneUtilities.git
```

```cmake
add_subdirectory(XPlaneUtilities)
target_link_libraries(your_plugin PRIVATE XPlaneUtilities)
```

## License

MIT License - see LICENSE file for details.

## Contributing

1. Fork the repository
2. Create a feature branch
3. Make your changes
4. Add tests for new functionality
5. Submit a pull request

## Changelog

### Version 1.0.0
- Initial release
- XPlaneLog with spdlog integration
- MenuHandler for simplified menu creation
- Modern CMake configuration
- vcpkg support
