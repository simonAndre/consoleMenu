#pragma once

#include "Menubase.h"
#include "MenuitemUpdater.hpp"
#include "MenuitemCallback.h"
// #include <vector>
// #include "Menu.hpp"

namespace CONSOLEMENU_NAMESPACE
{

// forward declarations
// class Menuitem;

class SubMenu : public Menuitem
{
    private:
        std::vector<ushort> _childsid;

    public:
        std::vector<fp_callback1> callbacksForChilds;
        SubMenu(Menubase *menuinstance);
        SubMenu(Menubase *menuinstance, const char *label);

        SubMenu *SetParent(SubMenu *parent);
        SubMenu *SetLabel(const char *name);
        virtual Menuitem* SetId(ushort id) override;

        /**
 * @brief add a submenu item hierarchy
 * 
 * @param label : label displayed
 * @return new SubMenu* created 
 */
        SubMenu *addSubMenu(const char *label);

        /**
     * @brief add a submenu item calling a given function - static labelling
     * 
     * @param label : label displayed
     * @param menukey : key to use for this menu (the unicity of this key is under your concern, this menu-item con't be created if it's not unique).
     * @param onselectFunc : pointer to the function to call for this menu. on the form @fp_callback3.
     * If the return value of the function is true : exit the menu after execution of this function, 
     * else stay in the current menu and wait for another action
     * @return MenuitemCallback* to chain with other configuration for this MenuItem
     */
        MenuitemCallback *addMenuitemCallback(const char *label, ushort menukey, fp_callback3 onselectFunc);

        /**
     * @brief add a submenu item calling a given function - dynamic labelling
     * 
     * @param namingFunc : callback to a function provinding dynamically the menuitem name (possibly given the menukey)
     * @param menukey : key to use for this menu (the unicity of this key is under your concern, this menu-item con't be created if it's not unique).
     * @param onselectFunc : pointer to the function to call for this menu. on the form @fp_callback3.
     * If the return value of the function is true : exit the menu after execution of this function, 
     * else stay in the current menu and wait for another action
     * @return MenuitemCallback* to chain with other configuration for this MenuItem
       */
        MenuitemCallback *addMenuitemCallback(fp_namingcallback namingFunc, ushort menukey, fp_callback3 onselectFunc);

        /**
     * @brief add a submenu item calling a given simple function (no params) - static labelling
     * 
     * @param label : label displayed
     * @param onselectFunc : pointer to the function to call for this menu. on the form @fp_callback1 (no params).
     * If the return value of the function is true : exit the menu after execution of this function, 
     * else stay in the current menu and wait for another action
     * @return MenuitemCallback* to chain with other configuration for this MenuItem
     */
        MenuitemCallback *addMenuitemCallback(const char *label, fp_callback1 onselectFunc);

        /**
     * @brief add a submenu item calling a given simple function (no params) - dynamic labelling
     * 
     * @param namingFunc : callback to a function provinding dynamically the menuitem name (possibly given the menukey)
     * @param onselectFunc : pointer to the function to call for this menu. on the form @fp_callback1 (no params).
     * If the return value of the function is true : exit the menu after execution of this function, 
     * else stay in the current menu and wait for another action
     * @return MenuitemUpdaterbase* to chain with other configuration for this MenuItem
     */
        MenuitemCallback *addMenuitemCallback(fp_namingcallback namingFunc, fp_callback1 onselectFunc);

        /**
 * @brief add a menu item designed for one goal : update a value in a given pointer to a variable.
     * 
 * 
 * @tparam T 
 * @param label 
 * @param variableToUpdate : pointer to the variable to be updated 
 * @param stringsize : size max of the c-string (buffer of char) 
 * @return MenuitemUpdaterbase* to chain with other configuration for this MenuItem
 */
        template <typename T>
        MenuitemUpdaterbase *addMenuitemUpdater(const char *label, T *variableToUpdate)
        {
            MenuitemUpdater<T> *miu = new MenuitemUpdater<T>(this->_menuinstance, label, this, menutype::variableUpdater_i);
            miu->setVarToUpdate(variableToUpdate);
            miu->setInputTrials(this->_menuinstance->getOptions().badInputRepeats);
            if (this->_menuinstance->addChild(this, miu))
                return miu;
            return nullptr;
    }

    /**
     * @brief add a menu item designed for one goal : update a string value in a given pointer to a variable.
     * 
     * @param label 
     * @param variableToUpdate : pointer to the variable to be updated
     * @param stringsize : size max of the c-string (buffer of char)
     * @return MenuitemUpdaterbase* to chain with other configuration for this MenuItem
     */
    MenuitemUpdaterbase *addMenuitemUpdater(const char *label, char *variableToUpdate, size_t stringsize);

    size_t getChildCount();

    Menuitem **getChilds();


    /**
 * @brief Add a callback function for all child item added to this submenu (aditive feature : don't override preexisting defined actions)
 * if callbacks are defined on specific childs on a child level, this common callback will be chained afterward (lower priority of execution)
 * 
 * @param newcallback prototype form = [bool function();]
 * @return SubMenu* to chain with other configuration for this MenuItem
 */
    SubMenu *addCallbackToChilds(fp_callback1 newcallback);
};

} // namespace CONSOLEMENU_NAMESPACE