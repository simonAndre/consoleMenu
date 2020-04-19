#pragma once
#include <string>
#include <string.h>
#include <stdexcept>
#include "../commontypes.h"
#include "../Configuration.h"
#include "specialTypes.h"

namespace CONSOLEMENU_NAMESPACE
{

//forward declaration
class Menubase;
class MenuitemHierarchy;

class Menuitem
{
private:
protected:
    Menubase *_menuinstance{nullptr};
    ushort _mid = 0;
    MenuitemHierarchy *_mparent{nullptr}; // if defined, it must be a MenuitemHierarchy
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
    Menuitem(Menubase *menuinstance, const char *menuname, MenuitemHierarchy *parent, menutype type);

    void setMenuInstance(Menubase *mi);

    MenuitemHierarchy *getParent();
    void setParent(MenuitemHierarchy *parent);
    void setName(const char *name);
    const char *getName();
    void setType(menutype type);
    menutype getType();
    virtual ushort getMenuKey();
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
};

} // namespace CONSOLEMENU_NAMESPACE