#include <XPlaneUtilities/MenuHandler.h>
#include <XPLMDisplay.h>
#include <XPLMMenus.h>
#include <stdexcept>

MenuItem::MenuItem(const std::string &title)
{
    m_parent_menu = XPLMFindPluginsMenu();
    m_item_id = XPLMAppendMenuItem(m_parent_menu, title.c_str(), nullptr, 0);
    
    if (m_item_id < 0) {
        throw std::runtime_error("Couldn't create menu item: " + title);
    }
    
    m_menu_id = XPLMCreateMenu(title.c_str(), m_parent_menu, m_item_id, menuHandler, this);
    
    if (!m_menu_id) {
        XPLMRemoveMenuItem(m_parent_menu, m_item_id);
        throw std::runtime_error("Couldn't create menu: " + title);
    }
}

MenuItem::MenuItem(const std::string &title, XPLMMenuID parent_menu, int item_idx)
{
    m_parent_menu = parent_menu;
    m_item_id = XPLMAppendMenuItem(parent_menu, title.c_str(), nullptr, 0);
    
    if (m_item_id < 0) {
        throw std::runtime_error("Couldn't create submenu item: " + title);
    }
    
    m_menu_id = XPLMCreateMenu(title.c_str(), parent_menu, m_item_id, menuHandler, this);
    
    if (!m_menu_id) {
        XPLMRemoveMenuItem(parent_menu, m_item_id);
        throw std::runtime_error("Couldn't create submenu: " + title);
    }
}

MenuItem::~MenuItem()
{
    if (m_menu_id) {
        XPLMDestroyMenu(m_menu_id);
        m_menu_id = nullptr;
    }
    
    if (m_item_id >= 0 && m_parent_menu) {
        XPLMRemoveMenuItem(m_parent_menu, m_item_id);
        m_item_id = -1;
    }
}

void MenuItem::addSubItem(const std::string &title, std::function<void()> action)
{
    m_callbacks.push_back(action);
    intptr_t idx = m_callbacks.size() - 1;
    XPLMAppendMenuItem(m_menu_id, title.c_str(), reinterpret_cast<void *>(idx), 0);
}

std::unique_ptr<MenuItem> MenuItem::addSubMenu(const std::string &title)
{
    auto submenu = std::unique_ptr<MenuItem>(new MenuItem(title, m_menu_id, m_item_id));
    return submenu;
}

void MenuItem::addSeparator()
{
    XPLMAppendMenuSeparator(m_menu_id);
}

void MenuItem::menuHandler(void *menuRef, void *itemRef)
{
    MenuItem *menu = static_cast<MenuItem *>(menuRef);
    auto idx = reinterpret_cast<intptr_t>(itemRef);
    
    if (idx >= 0 && idx < static_cast<intptr_t>(menu->m_callbacks.size())) {
        std::function<void()> callback = menu->m_callbacks[idx];
        if (callback) {
            callback();
        }
    }
}