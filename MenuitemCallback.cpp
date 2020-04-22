
#include "src/consoleMenu/Menu/MenuitemCallback.h"
#include "src/consoleMenu/IO/IoHelpers.hpp"
#include "src/consoleMenu/Menu/Menubase.h"

namespace CONSOLEMENU_NAMESPACE
{

MenuitemCallback::MenuitemCallback(Menubase *menuinstance, const char *menuname, SubMenu *parent, menutype type, ushort menukey)
    : Menuitem(menuinstance, menuname, parent, type)
{
    this->_mkey = menukey;
}

MenuitemCallback::MenuitemCallback() : Menuitem()
{
}

void MenuitemCallback::SetNamingCallback(fp_namingcallback namingFonction)
{
    _mNamingFonction = namingFonction;
}
void MenuitemCallback::setMenuKey(ushort key)
{
    this->_mkey = key;
}
const char *MenuitemCallback::getLabel()
{
    if (this->_mNamingFonction)
        return this->_mNamingFonction(this->_mkey);
    else
        return Menuitem::getLabel();
}

} // namespace CONSOLEMENU_NAMESPACE