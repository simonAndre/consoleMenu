

#include "src/consoleMenu/Menu/Menubase.h"
#include <stdexcept>
#include <cstdio>
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

MenuitemHierarchy *Menubase::getRootMenu()
{
    return nullptr;
}

bool Menubase::addChild(MenuitemHierarchy *parent [[gnu::unused]], Menuitem *child [[gnu::unused]])
{
    throw std::runtime_error("call to base function not implemented");
}

/**
 * @brief Get a menuitem the By its menuKey
 * 
 * @param menukey 
 * @return Menuitem 
 */
Menuitem *Menubase::getByKey(ushort menukey [[gnu::unused]]) { throw std::runtime_error("call to base function not implemented"); }

Menuitem *Menubase::getById(ushort menuid [[gnu::unused]]) { throw std::runtime_error("call to base function not implemented"); }

ushort Menubase::size() { throw std::runtime_error("call to base function not implemented"); }

void Menubase::displayMenu(MenuitemHierarchy *parent [[gnu::unused]]) { throw std::runtime_error("call to base function not implemented"); }

void Menubase::launchMenu()
{
    throw std::runtime_error("call to base function not implemented");
}

void Menubase::LoopCheckSerial()
{
    throw std::runtime_error("call to base function not implemented");
}
} // namespace CONSOLEMENU_NAMESPACE
