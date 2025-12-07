# XPlaneUtilities API Documentation

## Overview

XPlaneUtilities is a modern C++ library that provides essential utilities for X-Plane plugin development. It offers simplified interfaces for common tasks while maintaining high performance and modern C++ standards.

## Core Components

### XPlaneLog

Advanced logging system built on spdlog with X-Plane integration.

#### Features
- Multiple output sinks (console, file, X-Plane debug)
- Thread-safe logging
- Modern formatting with fmt library
- Shared logger access for other libraries
- Automatic log file management

#### API Reference

```cpp
class XPlaneLog {
public:
    static void init(const std::string& plugin_name);
    static void shutdown();
    static void trace(const std::string& message);
    static void debug(const std::string& message);
    static void info(const std::string& message);
    static void warn(const std::string& message);
    static void error(const std::string& message);
    static void critical(const std::string& message);
};

// Compile-time logging macros (strip debug/trace in release builds)
XPLANE_LOG_TRACE(fmt, ...)   // Removed in NDEBUG builds
XPLANE_LOG_DEBUG(fmt, ...)   // Removed in NDEBUG builds
XPLANE_LOG_INFO(fmt, ...)
XPLANE_LOG_WARN(fmt, ...)
XPLANE_LOG_ERROR(fmt, ...)
```

### MenuHandler

Simplified X-Plane menu creation and management.

#### Features
- Modern C++ interface for X-Plane menus
- Lambda callback support
- Automatic resource cleanup
- Move semantics support
- Menu item enable/disable

#### API Reference

```cpp
class MenuItem {
public:
    explicit MenuItem(const std::string& title);
    ~MenuItem();
    
    void addSubItem(const std::string& title, std::function<void()> action);
    std::unique_ptr<MenuItem> addSubMenu(const std::string& title);
    void addSeparator();
};
```

**Note:** MenuItem is non-copyable and non-movable. Keep the object alive for the menu's lifetime.

## Usage Examples

### Basic Logging

```cpp
#include <XPlaneUtilities/XPlaneUtilities.h>

// Initialize in your plugin
XPlaneUtilities::Initialize("MyPlugin");

// Use throughout your code
XPlaneLog::info("Plugin initialized");
XPlaneLog::error("Error occurred: {}", error_message);

// Or use compile-time macros (debug/trace stripped in release)
XPLANE_LOG_DEBUG("Debug info: {}", value);
XPLANE_LOG_INFO("Info message");

// Other libraries can get the shared logger
if (auto logger = spdlog::get("xplane")) {
    logger->info("Message from library");
}
```

### Menu Creation

```cpp
#include <XPlaneUtilities/MenuHandler.h>

// Create main menu
MenuItem mainMenu("My Plugin");

// Add items with callbacks
mainMenu.addSubItem("Settings", []() {
    // Handle settings
});

mainMenu.addSeparator();

mainMenu.addSubItem("About", []() {
    // Show about dialog
});

// Create submenu
auto settingsMenu = mainMenu.addSubMenu("Advanced");
settingsMenu->addSubItem("Option 1", []() { /* ... */ });
settingsMenu->addSubItem("Option 2", []() { /* ... */ });
```

## Integration Guide

### CMake Integration

```cmake
# Using FetchContent
include(FetchContent)
FetchContent_Declare(
    XPlaneUtilities
    GIT_REPOSITORY https://github.com/1090MHz/XPlaneUtilities.git
    GIT_TAG        main
)
FetchContent_MakeAvailable(XPlaneUtilities)

target_link_libraries(your_plugin PRIVATE XPlaneUtilities::XPlaneUtilities)
```

### vcpkg Integration

```bash
vcpkg install xplane-utilities
```

Then in your CMakeLists.txt:
```cmake
find_package(XPlaneUtilities REQUIRED)
1. **Initialize Early**: Call `XPlaneUtilities::Initialize()` in your plugin's startup function
2. **Shutdown Properly**: Call `XPlaneLog::shutdown()` in your plugin cleanup
3. **Use Shared Logger**: Other libraries should use `spdlog::get("xplane")` for consistent logging
4. **Menu Lifetime**: Keep MenuItem objects alive for the lifetime of your menus
5. **Use Macros for Performance**: Use `XPLANE_LOG_*` macros to eliminate debug strings in release builds
6. **Thread Safety**: All logging functions are thread-safe
1. **Initialize Early**: Call `XPlaneUtilities::Initialize()` in your plugin's startup function
2. **Use Shared Logger**: Other libraries should use `spdlog::get("xplane")` for consistent logging
3. **Menu Lifetime**: Keep MenuItem objects alive for the lifetime of your menus
4. **Error Handling**: Check `MenuItem::isValid()` after creation in critical code
5. **Thread Safety**: All logging functions are thread-safe

## Requirements

- C++17 compatible compiler
- CMake 3.20+
- spdlog library
- fmt library
- X-Plane SDK (for X-Plane specific features)

## Platform Support

- Windows (MSVC, MinGW)
- macOS (Clang)
- Linux (GCC, Clang)

## License

MIT License - see LICENSE file for details.
