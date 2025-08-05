#include "XPlaneUtilities/MenuHandler.h"

// X-Plane SDK Headers
#include <XPLMDisplay.h>
#include <XPLMMenus.h>

#include <functional>
#include <map>
#include <memory>
#include <string>
#include <utility> // For std::move

MenuItem::MenuItem(const std::string &title)
{
    m_item_id = XPLMAppendMenuItem(XPLMFindPluginsMenu(), title.c_str(), nullptr, 1);
    m_menu_id = XPLMCreateMenu(title.c_str(), XPLMFindPluginsMenu(), m_item_id, menuCallback, this);
}

MenuItem::~MenuItem()
{
    if (m_menu_id != nullptr)
    {
        XPLMClearAllMenuItems(m_menu_id);
        XPLMDestroyMenu(m_menu_id);
    }
    if (m_item_id != -1)
    {
        XPLMRemoveMenuItem(XPLMFindPluginsMenu(), m_item_id);
    }
}

MenuItem::MenuItem(MenuItem&& other) noexcept
    : m_menu_id(other.m_menu_id)
    , m_item_id(other.m_item_id)
    , m_actions(std::move(other.m_actions))
    , m_next_item_id(other.m_next_item_id)
{
    // Reset other object to prevent double cleanup
    other.m_menu_id = nullptr;
    other.m_item_id = -1;
    other.m_next_item_id = 0;
    
    // Update the callback reference to point to this object
    if (m_menu_id != nullptr)
    {
        // Note: X-Plane doesn't provide a way to change the callback reference
        // This is a limitation of the X-Plane API
    }
}

MenuItem& MenuItem::operator=(MenuItem&& other) noexcept
{
    if (this != &other)
    {
        // Clean up current resources
        this->~MenuItem();
        
        // Move resources from other
        m_menu_id = other.m_menu_id;
        m_item_id = other.m_item_id;
        m_actions = std::move(other.m_actions);
        m_next_item_id = other.m_next_item_id;
        
        // Reset other object
        other.m_menu_id = nullptr;
        other.m_item_id = -1;
        other.m_next_item_id = 0;
        
        // Update the callback reference to point to this object
        if (m_menu_id != nullptr)
        {
            // Note: X-Plane doesn't provide a way to change the callback reference
            // This is a limitation of the X-Plane API
        }
    }
    return *this;
}

void MenuItem::addSubItem(const std::string &title, std::function<void()> action)
{
    if (m_menu_id != nullptr)
    {
        int item_id = m_next_item_id++;
        m_actions[item_id] = std::move(action);
        XPLMAppendMenuItem(m_menu_id, title.c_str(), reinterpret_cast<void *>(static_cast<intptr_t>(item_id)), 1);
    }
}

void MenuItem::addSeparator()
{
    if (m_menu_id != nullptr)
    {
        XPLMAppendMenuSeparator(m_menu_id);
    }
}

void MenuItem::setItemEnabled(int itemIndex, bool enabled)
{
    if (m_menu_id != nullptr && itemIndex >= 0)
    {
        XPLMEnableMenuItem(m_menu_id, itemIndex, enabled ? 1 : 0);
    }
}

void MenuItem::menuCallback(void *menu_ref, void *item_ref)
{
    MenuItem *menu = static_cast<MenuItem *>(menu_ref);
    if (menu != nullptr)
    {
        intptr_t item_id = reinterpret_cast<intptr_t>(item_ref);
        auto it = menu->m_actions.find(static_cast<int>(item_id));
        if (it != menu->m_actions.end() && it->second)
        {
            it->second();
        }
    }
}
