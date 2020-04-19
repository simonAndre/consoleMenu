#include "MenuitemBack.h"

namespace CONSOLEMENU_NAMESPACE
{

MenuitemBack::MenuitemBack(Menubase *menuinstance) : Menuitem()
{
    _menuinstance = menuinstance;
    _mname = CONSOLEMENU_MENU_BACK;
    _mtype = menutype::back;
}
void MenuitemBack::setLastParent(MenuitemHierarchy *lastparent)
{
    _mparent = lastparent;
}

} // namespace CONSOLEMENU_NAMESPACE