#pragma once

/**
 * @file XPlaneUtilities.h
 * @brief Main convenience header for XPlaneUtilities library
 * 
 * Include this header to get access to all XPlaneUtilities functionality.
 * Alternatively, include specific headers for individual components.
 */

#include "XPlaneLog.h"
#include "MenuHandler.h"

namespace XPlaneUtilities {
    /**
     * @brief Library version information
     */
    constexpr int VERSION_MAJOR = 1;
    constexpr int VERSION_MINOR = 0;
    constexpr int VERSION_PATCH = 0;
    constexpr const char* VERSION_STRING = "1.0.0";
    
    /**
     * @brief Initialize the XPlaneUtilities library
     * @param plugin_name Name of the plugin using this library
     * 
     * This function initializes logging and other core systems.
     * Call this once during your plugin initialization.
     */
    inline void Initialize(const std::string& plugin_name) {
        XPlaneLog::init(plugin_name);
    }
}
