#pragma once

#include "Menubase.h"
#include "MenuitemUpdater.hpp"

// #include "MenuitemHierarchy.hpp"
// #include "Menu.hpp"

namespace CONSOLEMENU_NAMESPACE
{

// forward declarations
// class Menuitem;

class MenuitemHierarchy : public Menuitem
{
public:
    MenuitemHierarchy(Menubase *menuinstance, const char *label);

    bool addSubItem(Menuitem *subitem);

    /**
 * @brief add a submenu item hierarchy
 * 
 * @param label : label displayed
 * @return new MenuitemHierarchy* created 
 */
    MenuitemHierarchy *addMenuitemHierarchy(const char *label);

    /**
     * @brief add a submenu item calling a given function - static labelling
     * 
     * @param label : label displayed
     * @param menukey : key to use for this menu (the unicity of this key is under your concern, this menu-item con't be created if it's not unique).
     * @param onselectFunc : pointer to the function to call for this menu. on the form @fp_callback3.
     * If the return value of the function is true : exit the menu after execution of this function, 
     * else stay in the current menu and wait for another action
     * @return true : new menu-item correctly added to the collection.
     * @return false : an issue occured (typically, check the unicity of menukey)
     */
    bool addMenuitemCallback(const char *label, ushort menukey, fp_callback3 onselectFunc);

    /**
     * @brief add a submenu item calling a given function - dynamic labelling
     * 
     * @param namingFunc : callback to a function provinding dynamically the menuitem name (possibly given the menukey)
     * @param menukey : key to use for this menu (the unicity of this key is under your concern, this menu-item con't be created if it's not unique).
     * @param onselectFunc : pointer to the function to call for this menu. on the form @fp_callback3.
     * If the return value of the function is true : exit the menu after execution of this function, 
     * else stay in the current menu and wait for another action
     * @return true : new menu-item correctly added to the collection.
     * @return false : an issue occured (typically, check the unicity of menukey)
       */
    bool addMenuitemCallback(fp_namingcallback namingFunc, ushort menukey, fp_callback3 onselectFunc);

    /**
     * @brief add a submenu item calling a given simple function (no params) - static labelling
     * 
     * @param label : label displayed
     * @param onselectFunc : pointer to the function to call for this menu. on the form @fp_callback1 (no params).
     * If the return value of the function is true : exit the menu after execution of this function, 
     * else stay in the current menu and wait for another action
     * @return true : new menu-item correctly added to the collection.
     * @return false : an issue occured (typically, check the unicity of menukey)
     */
    bool addMenuitemCallback(const char *label, fp_callback1 onselectFunc);

    /**
     * @brief add a submenu item calling a given simple function (no params) - dynamic labelling
     * 
     * @param namingFunc : callback to a function provinding dynamically the menuitem name (possibly given the menukey)
     * @param onselectFunc : pointer to the function to call for this menu. on the form @fp_callback1 (no params).
     * If the return value of the function is true : exit the menu after execution of this function, 
     * else stay in the current menu and wait for another action
     * @return true : new menu-item correctly added to the collection.
     * @return false : an issue occured (typically, check the unicity of menukey)
     */
    bool addMenuitemCallback(fp_namingcallback namingFunc, fp_callback1 onselectFunc);

    /**
 * @brief add a menu item designed for one goal : update an string value in a given pointer to a variable.
     * 
 * 
 * @tparam T 
 * @param label 
 * @param variableToUpdate : pointer to the variable to be updated 
 * @param stringsize : size max of the c-string (buffer of char) 
 * @return true 
 * @return false 
 */
    template <typename T>
    bool addMenuitemUpdater(const char *label, T *variableToUpdate)
    {
        MenuitemUpdater<T> *miu = new MenuitemUpdater<T>(this->_menuinstance, label, this, menutype::variableUpdater_i);
        miu->setVarToUpdate(variableToUpdate);
        miu->setInputTrials(this->_menuinstance->getOptions().badInputRepeats);
        return this->_menuinstance->addChild(this, miu);
    }

    /**
     * @brief add a menu item designed for one goal : update an string value in a given pointer to a variable.
     * 
     * @param label 
     * @param variableToUpdate : pointer to the variable to be updated
     * @param stringsize : size max of the c-string (buffer of char)
     * @return true 
     * @return false 
     */
    bool addMenuitemUpdater(const char *label, char *variableToUpdate, size_t stringsize);
};

} // namespace CONSOLEMENU_NAMESPACE