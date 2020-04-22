#include "src/consoleMenu/Menu/SubMenu.h"
#include "src/consoleMenu/Menu/MenuitemCallback.h"
#include "src/consoleMenu/Menu/Menuitem.h"
#include "src/consoleMenu/Menu/Menubase.h"

namespace CONSOLEMENU_NAMESPACE
{
SubMenu::SubMenu(Menubase *menuinstance, const char *label) : Menuitem()
{
    this->_menuinstance = menuinstance;
    this->_mname = std::string(label);
    this->_mtype = menutype::hierarchymenu;
    this->_mid = 0;
}

/**
 * @brief add a submenu item hierarchy
 * 
 * @param label : label displayed
 * @return new SubMenu* created 
 */
SubMenu *SubMenu::addSubMenu(const char *label)
{
    SubMenu *mih = new SubMenu(this->_menuinstance, label);
    if (this->_menuinstance->addChild(this, mih))
        return mih;
    return nullptr;
}

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
MenuitemCallback* SubMenu::addMenuitemCallback(const char *label, ushort menukey, fp_callback3 onselectFunc)
{
    MenuitemCallback *mic = new MenuitemCallback(this->_menuinstance, label, this, menutype::externalFunction, menukey);
    mic->addCallback(onselectFunc);
    if( this->_menuinstance->addChild(this, mic))
        return mic;
    return nullptr;
}

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
MenuitemCallback* SubMenu::addMenuitemCallback(fp_namingcallback namingFunc, ushort menukey, fp_callback3 onselectFunc)
{
    MenuitemCallback *mic = new MenuitemCallback(this->_menuinstance, "", this, menutype::externalFunction, menukey);
    mic->addCallback(onselectFunc);
    mic->SetNamingCallback(namingFunc);
    if(this->_menuinstance->addChild(this, mic))
        return mic;
    return nullptr;
}

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
MenuitemCallback* SubMenu::addMenuitemCallback(const char *label, fp_callback1 onselectFunc)
{
    MenuitemCallback *mic = new MenuitemCallback(this->_menuinstance, label, this, menutype::externalFunction, (ushort)CONSOLEMENU_NOMENUKEY);
    mic->addCallback(onselectFunc);
    if(this->_menuinstance->addChild(this, mic))
        return mic;
    return nullptr;
}

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
MenuitemCallback* SubMenu::addMenuitemCallback(fp_namingcallback namingFunc, fp_callback1 onselectFunc)
{
    MenuitemCallback *mic = new MenuitemCallback(this->_menuinstance, "", this, menutype::externalFunction, CONSOLEMENU_NOMENUKEY);
    mic->addCallback(onselectFunc);
    mic->SetNamingCallback(namingFunc);
    if(this->_menuinstance->addChild(this, mic))
        return mic;
    return nullptr;
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
MenuitemUpdaterbase *SubMenu::addMenuitemUpdater(const char *label, char *variableToUpdate, size_t stringsize)
{
    MenuitemUpdater<char *> *miu = new MenuitemUpdater<char *>(this->_menuinstance, label, this, menutype::variableUpdater_s);
    miu->setVarToUpdate(variableToUpdate);
    miu->setInputTrials(this->_menuinstance->getOptions().badInputRepeats);
    miu->setStringToUpdateSize(stringsize);
    if( this->_menuinstance->addChild(this, miu))
        return miu;
    return nullptr;
}

} // namespace CONSOLEMENU_NAMESPACE