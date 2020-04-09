
#pragma once

#include <string.h>
#include "specialTypes.hpp"

namespace CONSOLEMENU_NAMESPACE
{
//forward declaration
class Menuitem;
class MenuitemBack;
class MenuitemHierarchy;

static char _consolemenuversion[10];

class Menubase
{
protected:
    MenuOptions _menuoptions;

public:
    Menubase() {}
    Menubase(MenuOptions options)
    {
        _menuoptions = options;
    }
    virtual void setOptions(MenuOptions options)
    {
        _menuoptions = options;
    }
    virtual MenuOptions getOptions()
    {
        return _menuoptions;
    }

    virtual char *getVersion()
    {
        sprintf(_consolemenuversion, "%i.%i.%i", CONSOLEMENU_VERSION_MAJOR, CONSOLEMENU_VERSION_MINOR, CONSOLEMENU_VERSION_REVISION);
        return _consolemenuversion;
    }

    virtual MenuitemHierarchy *getRootMenu()
    {
        return NULL;
    }

    virtual bool addChild(MenuitemHierarchy *parent, Menuitem *child) {}
    /**
 * @brief Get a menuitem the By its menuKey
 * 
 * @param menukey 
 * @return Menuitem 
 */
    virtual Menuitem *getByKey(ushort menukey) {}

    virtual Menuitem *getById(ushort menuid) {}

    virtual size_t size() {}

    virtual void displayMenu(MenuitemHierarchy *parent) {}

    virtual void launchMenu()
    {
    }
};
} // namespace CONSOLEMENU_NAMESPACE
