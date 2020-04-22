
#pragma once

#include "specialTypes.h"
namespace CONSOLEMENU_NAMESPACE
{

static ushort _menuDefaultTimeout [[gnu::unused]]{0};

// forward declarations
class Menuitem;
class SubMenu;

class Menubase
{
protected:
    MenuOptions _menuoptions;

public:
    Menubase();
    Menubase(MenuOptions options);
    virtual void setOptions(MenuOptions options);
    virtual MenuOptions getOptions();

    static char *getVersion();

    virtual SubMenu *getRootMenu();

    virtual bool addChild(SubMenu *parent, Menuitem *child);
    /**
 * @brief Get a menuitem the By its menuKey
 * 
 * @param menukey 
 * @return Menuitem 
 */
    virtual Menuitem *getByKey(ushort menukey);

    virtual Menuitem *getById(ushort menuid);

    virtual ushort size();

    virtual void displayMenu(SubMenu *parent);

    virtual void launchMenu();

    virtual void LoopCheckSerial();

    virtual void addMenuKey(ushort menukey, ushort menuid);
};
} // namespace CONSOLEMENU_NAMESPACE
