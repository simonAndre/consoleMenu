#pragma once
#include "MenuitemUpdaterbase.h"
#include "../IO/IoHelpers.hpp"

namespace CONSOLEMENU_NAMESPACE
{

template <typename T>
class MenuitemUpdater : public MenuitemUpdaterbase
{
private:
    T *_variableToUpdate{nullptr};

public:
    MenuitemUpdater(Menubase *menuinstance, const char *menuname, SubMenu *parent)
        : MenuitemUpdaterbase(menuinstance, menuname, parent)
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
    // virtual bool takeUserInput() override; //see implementation below
    virtual bool takeUserInput() override
    {
        if (this->_variableToUpdate)
        {
            return IoHelpers::TakeUserInput("enter new value>", (T *)this->_variableToUpdate, this->_inputtrials);
        }
        return false;
    }

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
    MenuitemUpdater(Menubase *menuinstance, const char *menuname, SubMenu *parent)
        : MenuitemUpdaterbase(menuinstance, menuname, parent)    {}
    
    MenuitemUpdater() : MenuitemUpdaterbase(){}

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

template <>
class MenuitemUpdater<bool> : public MenuitemUpdaterbase
{
private:
    bool *_variableToUpdate{nullptr};

public:
    MenuitemUpdater(Menubase *menuinstance, const char *menuname, SubMenu *parent)
        : MenuitemUpdaterbase(menuinstance, menuname, parent)
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
            IoHelpers::IOdisplay(*this->_variableToUpdate);
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
            if (CONSOLEMENU_UPDATERMENU_BOOLMODESWITCH == 1)
            {
                *this->_variableToUpdate = !*this->_variableToUpdate;
                return true;
            }
            else
            {
                return IoHelpers::TakeUserInput("enter new value (0/1)>", (bool *)this->_variableToUpdate, this->_inputtrials);
            }
        }
        return false;
    }

    void setVarToUpdate(bool *varptr)
    {
        _variableToUpdate = varptr;
    }
};

} // namespace CONSOLEMENU_NAMESPACE