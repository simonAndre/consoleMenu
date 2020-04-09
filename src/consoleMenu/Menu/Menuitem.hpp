#pragma once
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
    Menuitem() {}

    /**
 * @brief Construct a new Menuitembase object
 * 
 * @param menuname 
 * @param id 
 * @param parentid 
 * @param menuFonction 
 * @param type 
 */
    Menuitem(Menubase *menuinstance, const char *menuname, MenuitemHierarchy *parent, menutype type)
    {
        this->_menuinstance = menuinstance;
        this->_mparent = parent;
        this->_mname = std::string(menuname);
        this->_mtype = type;
    }

    void setMenuInstance(Menubase *mi)
    {
        this->_menuinstance = mi;
    }

    MenuitemHierarchy *getParent()
    {
        return (MenuitemHierarchy *)this->_mparent;
    }
    void setParent(MenuitemHierarchy *parent)
    {
        this->_mparent = parent;
    }
    void setName(const char *name)
    {
        this->_mname = std::string(name);
    }
    const char *getName()
    {
        return this->_mname.c_str();
    }
    void setType(menutype type)
    {
        this->_mtype = type;
    }
    menutype getType()
    {
        return this->_mtype;
    }
    virtual ushort getMenuKey()
    {
        return this->_mkey;
    }
    ushort getId()
    {
        return this->_mid;
    }
    void setId(ushort id)
    {
        this->_mid = id;
    }
    /**
 * @brief display the menuitem 
 * 
 * @param dispcallback function to use to render (UI dependant)
 * @param idx_menu : num to use for menuitem selection
 */
    virtual void display(ushort idx_menu)
    {
        //use menuname
        IoHelpers::IOdisplay(idx_menu);
        IoHelpers::IOdisplay(this->_menuinstance->getOptions().id_separator);
        IoHelpers::IOdisplay(this->_mname.c_str());
        IoHelpers::IOdisplayLn("");
    }

    virtual bool selectAction()
    {
        throw std::runtime_error("no action implemented for this menutype");
    }
};

} // namespace CONSOLEMENU_NAMESPACE