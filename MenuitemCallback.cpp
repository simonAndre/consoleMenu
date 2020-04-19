
#include "src/consoleMenu/Menu/MenuitemCallback.h"
#include "src/consoleMenu/IO/IoHelpers.hpp"
#include "src/consoleMenu/Menu/Menubase.h"

namespace CONSOLEMENU_NAMESPACE
{

MenuitemCallback::MenuitemCallback(Menubase *menuinstance, const char *menuname, MenuitemHierarchy *parent, menutype type, ushort menukey)
    : Menuitem(menuinstance, menuname, parent, type)
{
    this->_mkey = menukey;
}

MenuitemCallback::MenuitemCallback() : Menuitem()
{
}

void MenuitemCallback::SetCallback(fp_callback1 menuFonction)
{
    _mFonction_form1 = menuFonction;
}
void MenuitemCallback::SetCallback(fp_callback3 menuFonction)
{
    _mFonction_form3 = menuFonction;
}
void MenuitemCallback::SetNamingCallback(fp_namingcallback namingFonction)
{
    _mNamingFonction = namingFonction;
}
void MenuitemCallback::setMenuKey(ushort key)
{
    this->_mkey = key;
}
void MenuitemCallback::display(ushort idx_menu)
{
    if (!_mNamingFonction)
        //call base function
        Menuitem::display(idx_menu);
    else
    {
        IoHelpers::IOdisplay(idx_menu);
        IoHelpers::IOdisplay(_menuinstance->getOptions().id_separator);
        IoHelpers::IOdisplay(_mNamingFonction(this->_mkey));
        IoHelpers::IOdisplayLn("");
    }
}

/**
 * @brief call the menu function
 * 
 * @return true if call was successfull (return bool from the callback).
 * if not successfull, false to prompt again in the outside loop. 
 */
bool MenuitemCallback::selectAction()
{
    IoHelpers::IOdisplayLn("");
    if (_mFonction_form3)
        return this->_mFonction_form3(this->_mkey, this->_mname.c_str());
    if (_mFonction_form1)
        return this->_mFonction_form1();
    return false;
}
} // namespace CONSOLEMENU_NAMESPACE