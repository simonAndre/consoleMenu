

#include "Menu/Menubase.h"
#include <stdexcept>
#include <cstdio>
#include <vector>

#define CONSOLEMENU_NOTIMPLEMENTED "call to base function not implemented"
namespace CONSOLEMENU_NAMESPACE 
{

static char _consolemenuversion[10]{'\0'};

Menubase::Menubase() {}

Menubase::Menubase(MenuOptions options)
{
    _menuoptions = options;
}
//
//
//

void Menubase::setOptions(MenuOptions options)
{
    _menuoptions = options;
    _menuDefaultTimeout = _menuoptions.expirationTimeSec;
}

MenuOptions Menubase::getOptions()
{
    return _menuoptions;
}

char *Menubase::getVersion()
{
    sprintf(_consolemenuversion, "%i.%i.%i", CONSOLEMENU_VERSION_MAJOR, CONSOLEMENU_VERSION_MINOR, CONSOLEMENU_VERSION_REVISION);
    return _consolemenuversion;
}

SubMenu *Menubase::getRootMenu()
{
    return nullptr;
}

bool Menubase::addChild(SubMenu *parent [[gnu::unused]], Menuitem *child [[gnu::unused]])
{
    throw std::runtime_error(CONSOLEMENU_NOTIMPLEMENTED);
}

/**
 * @brief Get a menuitem the By its menuKey
 * 
 * @param menukey 
 * @return Menuitem 
 */
Menuitem *Menubase::getByKey(ushort menukey [[gnu::unused]]) { throw std::runtime_error(CONSOLEMENU_NOTIMPLEMENTED); }

Menuitem *Menubase::getById(ushort menuid [[gnu::unused]]) { throw std::runtime_error(CONSOLEMENU_NOTIMPLEMENTED); }

ushort Menubase::size() { throw std::runtime_error(CONSOLEMENU_NOTIMPLEMENTED); }

void Menubase::displayMenu(SubMenu *parent [[gnu::unused]]) { throw std::runtime_error(CONSOLEMENU_NOTIMPLEMENTED); }

void Menubase::addMenuKey(ushort menukey [[gnu::unused]], ushort menuid [[gnu::unused]]) { throw std::runtime_error(CONSOLEMENU_NOTIMPLEMENTED); }

void Menubase::launchMenu()
{
    throw std::runtime_error(CONSOLEMENU_NOTIMPLEMENTED);
}

bool Menubase::LoopCheckSerial()
{
    throw std::runtime_error(CONSOLEMENU_NOTIMPLEMENTED);
}

SubMenu *Menubase::addSubMenu(const char *label [[gnu::unused]])
{
    throw std::runtime_error(CONSOLEMENU_NOTIMPLEMENTED);
}

// std::vector<Menuitem> Menubase::getChildsOf(SubMenu *parent [[gnu::unused]])
// {
//     throw std::runtime_error(CONSOLEMENU_NOTIMPLEMENTED);
// }

} // namespace CONSOLEMENU_NAMESPACE
