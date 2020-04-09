#pragma once

namespace CONSOLEMENU_NAMESPACE
{

class MenuitemBack : public Menuitem
{
private:
public:
    MenuitemBack(Menubase *menuinstance) : Menuitem()
    {
        _menuinstance = menuinstance;
        _mname = CONSOLEMENU_MENU_BACK;
        _mtype = menutype::back;
    }
    void setLastParent(MenuitemHierarchy *lastparent)
    {
        _mparent = lastparent;
    }
};

} // namespace CONSOLEMENU_NAMESPACE