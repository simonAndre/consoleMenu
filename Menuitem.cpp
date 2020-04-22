
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

const char *Menuitem::getLabel()
{
    return this->_mname.c_str();
}
void Menuitem::setMenuKey(ushort menukey)
{
    this->_menuinstance->addMenuKey(menukey, this->getId());
    this->_mkey = menukey;
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
    IoHelpers::IOdisplay(this->getLabel());
    IoHelpers::IOdisplayLn("");
}

/**
 * @brief call the menu functions. Stop the callback call on the first failing callback (returning false) 
 * 
 * @return true if call was successfull for all callbacks (return true from each callbacks).
 * if not successfull, false to prompt again in the outside loop. 
 */
bool Menuitem::selectAction()
{
    IoHelpers::IOdisplayLn("");
    std::vector<fp_callback1>::const_iterator it_form1;
    std::vector<fp_callback3>::const_iterator it_form3;
    for (it_form1 = _callbacksForm1.begin(); it_form1 != _callbacksForm1.end(); ++it_form1)
    {
        if(!(*it_form1)())      // call the callback form1
            return false;
    }
    for (it_form3 = _callbacksForm3.begin(); it_form3 != _callbacksForm3.end(); ++it_form3)
    {
        if(!(*it_form3)(this->getMenuKey(),this->getLabel())) // call the callback form3
            return false;
    }
    return true;
}

void Menuitem::addCallback(fp_callback1 newcallback)
{
    this->_callbacksForm1.push_back(newcallback);
}

void Menuitem::addCallback(fp_callback3 newcallback)
{
    this->_callbacksForm3.push_back(newcallback);
}

} // namespace CONSOLEMENU_NAMESPACE