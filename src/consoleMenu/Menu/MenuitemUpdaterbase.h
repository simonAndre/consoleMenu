#pragma once
#include "Menuitem.h"

namespace CONSOLEMENU_NAMESPACE
{

class MenuitemUpdaterbase : public Menuitem
{
protected:
    ushort _inputtrials;

public:
    MenuitemUpdaterbase(Menubase *menuinstance, const char *menuname, SubMenu *parent);
    MenuitemUpdaterbase();

    /**
 * @brief display the menu-item current value, if variableToUpdate is set (for updaters menu-items)
 * 
 * @param displaycb : display callback
 * @param addbrackets : true to surround with brackets
 */
    virtual void displayCurrentValue();

    /**
 * @brief take the user input, if _variableToUpdate is set (for updaters menu-items)
 * 
 * @return true 
 * @return false 
 */
    virtual bool takeUserInput();

    void setInputTrials(ushort trials);

    /**
 * @brief call the menu function
 * 
 * @return true if call was successfull (return bool from the callback).
 * if not successfull, false to prompt again in the outside loop. 
 */
    virtual SelectActionResult selectAction() override;

    virtual void display(ushort idx_menu);
};

} // namespace CONSOLEMENU_NAMESPACE