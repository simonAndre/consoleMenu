#pragma once
#include <string>
#include <string.h>
#include <stdexcept>
#include "../commontypes.h"
#include "specialTypes.h"
#include <vector>

namespace CONSOLEMENU_NAMESPACE
{

//forward declaration
class Menubase;
class SubMenu;

class Menuitem
{
private:
    std::vector<fp_callback1> _callbacksForm1;
    std::vector<fp_callback3> _callbacksForm3;

protected:
    Menubase *_menuinstance{nullptr};
    ushort _mid = 0;
    SubMenu *_mparent{nullptr};           // if defined, it must be a SubMenu
    ushort _mkey = CONSOLEMENU_NOMENUKEY; //optional
    std::string _mname;
    menutype _mtype;

public:
    Menuitem();

    /**
 * @brief Construct a new Menuitembase object
 * 
 * @param menuname 
 * @param id 
 * @param parentid 
 * @param menuFonction 
 * @param type 
 */
    Menuitem(Menubase *menuinstance, const char *menuname, SubMenu *parent, menutype type);

    void setMenuInstance(Menubase *mi);

    SubMenu *getParent();
    void setParent(SubMenu *parent);
    void setName(const char *name);
    virtual const char *getLabel();
    void setType(menutype type);
    menutype getType();
    virtual ushort getMenuKey();
    /**
     * @brief add a Menu Key to this menu-item (throw runtimeExeption if this menukey alreaky exists)
     * 
     * @param menukey 
     */
    void setMenuKey(ushort menukey);
    ushort getId();
    void setId(ushort id);
    /**
 * @brief display the menuitem 
 * 
 * @param dispcallback function to use to render (UI dependant)
 * @param idx_menu : num to use for menuitem selection
 */
    virtual void display(ushort idx_menu);

    virtual bool selectAction();

    /**
 * @brief Add a callback function to this menuitem (aditive feature : don't override preexisting defined actions)
 * 
 * @param newcallback prototype form = [bool function();]
 * @return true 
 * @return false 
 */
    void addCallback(fp_callback1 newcallback);

    /**
 * @brief Add a callback function to this menuitem (aditive feature : don't override preexisting defined actions)
 * 
 * @param newcallback prototype form = [bool function(ushort menukey,const char* itemlabel);]
 * @return true 
 * @return false 
 */
    void addCallback(fp_callback3 newcallback);
};

} // namespace CONSOLEMENU_NAMESPACE