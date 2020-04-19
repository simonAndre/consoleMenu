#pragma once

#include "MenuitemHierarchy.h"

namespace CONSOLEMENU_NAMESPACE
{

class MenuitemBack : public Menuitem
{
private:
public:
    MenuitemBack(Menubase *menuinstance);
    void setLastParent(MenuitemHierarchy *lastparent);
};

} // namespace CONSOLEMENU_NAMESPACE