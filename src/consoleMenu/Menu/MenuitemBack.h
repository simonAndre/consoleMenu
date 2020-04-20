#pragma once

#include "SubMenu.h"

namespace CONSOLEMENU_NAMESPACE
{

class MenuitemBack : public Menuitem
{
private:
public:
    MenuitemBack(Menubase *menuinstance);
    void setLastParent(SubMenu *lastparent);
};

} // namespace CONSOLEMENU_NAMESPACE