#ifndef MENUHANDLER_H
#define MENUHANDLER_H

#include <string>
#include <map>
#include <functional>
#include <memory>
#include <vector>

// Forward declaration of XPLMMenuID
typedef void *XPLMMenuID;

class MenuItem
{
public:
    // Constructor and Destructor
    MenuItem(const std::string &title);
    ~MenuItem();

    // Member functions
    void addSubItem(const std::string &title, std::function<void()> action);
    std::unique_ptr<MenuItem> addSubMenu(const std::string &title);
    void addSeparator();

private:
    // Private constructor for submenu creation
    MenuItem(const std::string &title, XPLMMenuID parent_menu, int item_idx);

    // Static menu handler callback (AviTab pattern)
    static void menuHandler(void *menuRef, void *itemRef);

    // Member variables
    XPLMMenuID m_parent_menu = nullptr;
    XPLMMenuID m_menu_id = nullptr;
    int m_item_id = -1;
    std::vector<std::function<void()>> m_callbacks;
};

#endif // MENUHANDLER_H