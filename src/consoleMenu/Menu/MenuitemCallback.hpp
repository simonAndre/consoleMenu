#pragma once
// #include "Menuitem.hpp"
// #include "specialTypes.hpp"
// #include "../IO/IoHelpers.hpp"
// #include "Menuitem.hpp"
// #include "MenuitemCallback.hpp"
// #include "MenuitemUpdater.hpp"
// #include "MenuitemHierarchy.hpp"
// #include "Menu.hpp"
namespace CONSOLEMENU_NAMESPACE
{

class MenuitemCallback : public Menuitem
{
private:
protected:
    fp_callback1 _mFonction_form1{nullptr};
    fp_callback3 _mFonction_form3{nullptr};
    fp_namingcallback *_mNamingFonction{nullptr};

public:
    /**
 * @brief Construct a new MenuitemCallback object
 * 
 * @param menuname 
 * @param id 
 * @param parentid 
 * @param menuFonction 
 * @param type 
 */
    MenuitemCallback(Menubase *menuinstance, const char *menuname, MenuitemHierarchy *parent, menutype type, ushort menukey)
        : Menuitem(menuinstance, menuname, parent, type)
    {
        this->_mkey = menukey;
    }

    MenuitemCallback() : Menuitem()
    {
    }

    void SetCallback(fp_callback1 menuFonction)
    {
        _mFonction_form1 = menuFonction;
    }
    void SetCallback(fp_callback3 menuFonction)
    {
        _mFonction_form3 = menuFonction;
    }
    void SetNamingCallback(fp_namingcallback namingFonction)
    {
        _mNamingFonction = namingFonction;
    }
    void setMenuKey(ushort key)
    {
        this->_mkey = key;
    }
    virtual void display(ushort idx_menu) override
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
    virtual bool selectAction() override
    {
        IoHelpers::IOdisplayLn("");
        if (_mFonction_form3)
            return this->_mFonction_form3(this->_mkey, this->_mname.c_str());
        if (_mFonction_form1)
            return this->_mFonction_form1();
        return false;
    }
};
} // namespace CONSOLEMENU_NAMESPACE