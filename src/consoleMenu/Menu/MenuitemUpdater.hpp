#pragma once
// #include "Menuitem.hpp"
// #include "specialTypes.hpp"
// #include "../IO/IoHelpers.hpp"
// #include "Menuitem.hpp"
// #include "MenuitemCallback.hpp"
// #include "MenuitemUpdater.hpp"
// #include "MenuitemHierarchy.hpp"
// #include "Menu.hpp"
// #include "../Configuration.hpp"

namespace CONSOLEMENU_NAMESPACE
{

class MenuitemUpdaterbase : public Menuitem
{
protected:
    ushort _inputtrials;

public:
    MenuitemUpdaterbase(Menubase *menuinstance, const char *menuname, MenuitemHierarchy *parent, menutype type)
        : Menuitem(menuinstance, menuname, parent, type)
    {
    }
    MenuitemUpdaterbase() : Menuitem()
    {
    }

    /**
 * @brief display the menu-item current value, if variableToUpdate is set (for updaters menu-items)
 * 
 * @param displaycb : display callback
 * @param addbrackets : true to surround with brackets
 */
    virtual void displayCurrentValue() {}

    /**
 * @brief take the user input, if _variableToUpdate is set (for updaters menu-items)
 * 
 * @return true 
 * @return false 
 */
    virtual bool takeUserInput() { return false; }

    void setInputTrials(ushort trials)
    {
        _inputtrials = trials;
    }

    /**
 * @brief call the menu function
 * 
 * @return true if call was successfull (return bool from the callback).
 * if not successfull, false to prompt again in the outside loop. 
 */
    virtual bool selectAction() override
    {
        IoHelpers::IOdisplay(";current value is : ");
        this->displayCurrentValue();
        IoHelpers::IOdisplayLn("");
        this->takeUserInput(); // return value is not handled for now
        return false;          //to stay in the menu
    }

    virtual void display(ushort idx_menu)
    {
        IoHelpers::IOdisplay(idx_menu);
        IoHelpers::IOdisplay(_menuinstance->getOptions().id_separator);
        IoHelpers::IOdisplay(_mname);
        IoHelpers::IOdisplay(" [=");
        this->displayCurrentValue();
        IoHelpers::IOdisplay("]");
        IoHelpers::IOdisplayLn("");
    }
};

template <typename T>
class MenuitemUpdater : public MenuitemUpdaterbase
{
private:
    T *_variableToUpdate{nullptr};

public:
    MenuitemUpdater(Menubase *menuinstance, const char *menuname, MenuitemHierarchy *parent, menutype type)
        : MenuitemUpdaterbase(menuinstance, menuname, parent, type)
    {
    }
    MenuitemUpdater() : MenuitemUpdaterbase()
    {
    }

    /**
 * @brief display the menu-item current value, if variableToUpdate is set (for updaters menu-items)
 * 
 * @param displaycb : display callback
 * @param addbrackets : true to surround with brackets
 */
    virtual void displayCurrentValue() override
    {
        if (this->_variableToUpdate)
        {
            IoHelpers::IOdisplay(*(T *)this->_variableToUpdate);
        }
    }

    /**
 * @brief take the user input, if _variableToUpdate is set (for updaters menu-items)
 * 
 * @return true 
 * @return false 
 */
    virtual bool takeUserInput() override; //see implementation below

    void setVarToUpdate(T *varptr)
    {
        _variableToUpdate = varptr;
    }
};

template <>
class MenuitemUpdater<char *> : public MenuitemUpdaterbase
{
private:
    char *_variableToUpdate{nullptr};
    size_t _stringToUpdateSize;

public:
    MenuitemUpdater(Menubase *menuinstance, const char *menuname, MenuitemHierarchy *parent, menutype type)
        : MenuitemUpdaterbase(menuinstance, menuname, parent, type)
    {
    }
    MenuitemUpdater() : MenuitemUpdaterbase()
    {
    }

    void setStringToUpdateSize(size_t size)
    {
        _stringToUpdateSize = size;
    }

    /**
 * @brief display the menu-item current value, if variableToUpdate is set (for updaters menu-items)
 * 
 * @param displaycb : display callback
 * @param addbrackets : true to surround with brackets
 */
    virtual void displayCurrentValue() override
    {
        if (this->_variableToUpdate)
        {
            IoHelpers::IOdisplay(this->_variableToUpdate);
        }
    }

    /**
 * @brief take the user input, if _variableToUpdate is set (for updaters menu-items)
 * 
 * @return true 
 * @return false 
 */
    virtual bool takeUserInput() override
    {
        if (this->_variableToUpdate)
        {
            return IoHelpers::TakeUserInput("enter new value>", (char *)this->_variableToUpdate, this->_stringToUpdateSize, this->_inputtrials);
        }
        return false;
    }

    void setVarToUpdate(char *varptr)
    {
        _variableToUpdate = varptr;
    }
};

template <typename T>
bool MenuitemUpdater<T>::takeUserInput()
{
    if (this->_variableToUpdate)
    {
        return IoHelpers::TakeUserInput("enter new value>", (T *)this->_variableToUpdate, this->_inputtrials);
    }
    return false;
}

template <>
bool MenuitemUpdater<bool>::takeUserInput()
{
    if (this->_variableToUpdate)
    {
        if (CONSOLEMENU_UPDATERMENU_BOOLMODESWITCH == 1)
        {
            *(bool *)this->_variableToUpdate = !*(bool *)this->_variableToUpdate;
            return true;
        }
        else
        {
            return IoHelpers::TakeUserInput("enter new value (0/1)>", (bool *)this->_variableToUpdate, this->_inputtrials);
        }
    }
    return false;
}
} // namespace CONSOLEMENU_NAMESPACE