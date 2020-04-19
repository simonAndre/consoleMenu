// #include "MenuitemUpdaterbase.h"
// #include "../IO/IoHelpers.hpp"
#include "MenuitemUpdater.hpp"
#include "Menubase.h"

namespace CONSOLEMENU_NAMESPACE
{

MenuitemUpdaterbase::MenuitemUpdaterbase(Menubase *menuinstance, const char *menuname, MenuitemHierarchy *parent, menutype type)
    : Menuitem(menuinstance, menuname, parent, type)
{
}
MenuitemUpdaterbase::MenuitemUpdaterbase() : Menuitem()
{
}

/**
 * @brief display the menu-item current value, if variableToUpdate is set (for updaters menu-items)
 * 
 * @param displaycb : display callback
 * @param addbrackets : true to surround with brackets
 */
void MenuitemUpdaterbase::displayCurrentValue() {}

/**
 * @brief take the user input, if _variableToUpdate is set (for updaters menu-items)
 * 
 * @return true 
 * @return false 
 */
bool MenuitemUpdaterbase::takeUserInput() { return false; }

void MenuitemUpdaterbase::setInputTrials(ushort trials)
{
    _inputtrials = trials;
}

/**
 * @brief call the menu function
 * 
 * @return true if call was successfull (return bool from the callback).
 * if not successfull, false to prompt again in the outside loop. 
 */
bool MenuitemUpdaterbase::selectAction()
{
    IoHelpers::IOdisplay(";current value is : ");
    this->displayCurrentValue();
    IoHelpers::IOdisplayLn("");
    this->takeUserInput(); // return value is not handled for now
    return false;          //to stay in the menu
}

void MenuitemUpdaterbase::display(ushort idx_menu)
{
    IoHelpers::IOdisplay(idx_menu);
    IoHelpers::IOdisplay(_menuinstance->getOptions().id_separator);
    IoHelpers::IOdisplay(_mname);
    IoHelpers::IOdisplay(" [=");
    this->displayCurrentValue();
    IoHelpers::IOdisplay("]");
    IoHelpers::IOdisplayLn("");
}

} // namespace CONSOLEMENU_NAMESPACE