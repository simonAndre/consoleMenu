
#include "src/consoleMenu/Menu/Menuitem.h"
#include "src/consoleMenu/Menu/Menubase.h"
#include "src/consoleMenu/IO/IoHelpers.hpp"

namespace CONSOLEMENU_NAMESPACE
{

Menuitem::Menuitem() {}

/**
 * @brief Construct a new Menuitembase object
 * 
 * @param menuname 
 * @param id 
 * @param parentid 
 * @param menuFonction 
 * @param type 
 */
Menuitem::Menuitem(Menubase *menuinstance, const char *menuname, SubMenu *parent, menutype type)
{
    this->_menuinstance = menuinstance;
    this->_mparent = parent;
    this->_mname = std::string(menuname);
    this->_mtype = type;
}

void Menuitem::setMenuInstance(Menubase *mi)
{
    this->_menuinstance = mi;
}

SubMenu *Menuitem::getParent()
{
    return (SubMenu *)this->_mparent;
}
void Menuitem::setParent(SubMenu *parent)
{
    this->_mparent = parent;
}
void Menuitem::setName(const char *name)
{
    this->_mname = std::string(name);
}
const char *Menuitem::getName()
{
    return this->_mname.c_str();
}
void Menuitem::setType(menutype type)
{
    this->_mtype = type;
}
menutype Menuitem::getType()
{
    return this->_mtype;
}
ushort Menuitem::getMenuKey()
{
    return this->_mkey;
}
ushort Menuitem::getId()
{
    return this->_mid;
}
void Menuitem::setId(ushort id)
{
    this->_mid = id;
}
/**
 * @brief display the menuitem 
 * 
 * @param dispcallback function to use to render (UI dependant)
 * @param idx_menu : num to use for menuitem selection
 */
void Menuitem::display(ushort idx_menu)
{
    //use menuname
    IoHelpers::IOdisplay(idx_menu);
    IoHelpers::IOdisplay(this->_menuinstance->getOptions().id_separator);
    IoHelpers::IOdisplay(this->_mname.c_str());
    IoHelpers::IOdisplayLn("");
}

bool Menuitem::selectAction()
{
    throw std::runtime_error("no action implemented for this menutype");
}

} // namespace CONSOLEMENU_NAMESPACE