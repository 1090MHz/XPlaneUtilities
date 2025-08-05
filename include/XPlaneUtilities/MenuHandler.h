#pragma once

/**
 * @file MenuHandler.h
 * @brief Simplified X-Plane menu creation and management
 */

#include <string>
#include <map>
#include <functional>

// Forward declaration of XPLMMenuID
typedef void *XPLMMenuID;

/**
 * @brief Simplified menu item management for X-Plane plugins
 * 
 * MenuHandler provides a modern C++ interface for creating and managing
 * X-Plane plugin menus with automatic cleanup and lambda support.
 * 
 * @example
 * ```cpp
 * // Create a main menu
 * MenuItem mainMenu("My Plugin");
 * 
 * // Add menu items with lambda callbacks
 * mainMenu.addSubItem("Settings", []() {
 *     // Handle settings click
 * });
 * 
 * mainMenu.addSubItem("About", []() {
 *     // Show about dialog
 * });
 * ```
 */
class MenuItem
{
public:
    /**
     * @brief Create a new menu item
     * @param title The display title for the menu
     * 
     * Creates a new menu in X-Plane's plugin menu system.
     * The menu is automatically cleaned up when the MenuItem is destroyed.
     */
    explicit MenuItem(const std::string &title);

    /**
     * @brief Destructor - automatically cleans up X-Plane menu resources
     */
    ~MenuItem();

    // Disable copy construction and assignment
    MenuItem(const MenuItem&) = delete;
    MenuItem& operator=(const MenuItem&) = delete;

    // Enable move construction and assignment
    MenuItem(MenuItem&& other) noexcept;
    MenuItem& operator=(MenuItem&& other) noexcept;

    /**
     * @brief Add a sub-menu item with a callback
     * @param title The display title for the sub-item
     * @param action Callback function to execute when item is selected
     * 
     * Adds a clickable item to this menu. The action callback will be
     * invoked when the user selects this menu item.
     */
    void addSubItem(const std::string &title, std::function<void()> action);

    /**
     * @brief Add a separator line to the menu
     * 
     * Adds a visual separator between menu items for better organization.
     */
    void addSeparator();

    /**
     * @brief Enable or disable a menu item
     * @param itemIndex Index of the item to modify (0-based)
     * @param enabled True to enable, false to disable
     */
    void setItemEnabled(int itemIndex, bool enabled);

    /**
     * @brief Check if this menu item is valid
     * @return True if the menu was created successfully
     */
    bool isValid() const { return m_menu_id != nullptr; }

private:
    /**
     * @brief Static callback handler for X-Plane menu system
     * @param menu_ref Menu reference from X-Plane
     * @param item_ref Item reference from X-Plane
     */
    static void menuCallback(void* menu_ref, void* item_ref);

    /// X-Plane menu ID
    XPLMMenuID m_menu_id = nullptr;
    
    /// Our plugin's menu item ID in the parent menu
    int m_item_id = -1;
    
    /// Map of menu item indices to their callback functions
    std::map<int, std::function<void()>> m_actions;
    
    /// Next available item index
    int m_next_item_id = 0;
};
