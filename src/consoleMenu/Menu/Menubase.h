
#pragma once

#include "specialTypes.h"
namespace CONSOLEMENU_NAMESPACE
{

// forward declarations
class Menuitem;
class MenuitemHierarchy;

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

    virtual MenuitemHierarchy *getRootMenu();

    virtual bool addChild(MenuitemHierarchy *parent, Menuitem *child);
    /**
 * @brief Get a menuitem the By its menuKey
 * 
 * @param menukey 
 * @return Menuitem 
 */
    virtual Menuitem *getByKey(ushort menukey);

    virtual Menuitem *getById(ushort menuid);

    virtual ushort size();

    virtual void displayMenu(MenuitemHierarchy *parent);

    virtual void launchMenu();

    virtual void LoopCheckSerial();
};
} // namespace CONSOLEMENU_NAMESPACE
