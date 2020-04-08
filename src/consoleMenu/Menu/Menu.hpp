
#pragma once

// #include "Menuitem.hpp"
#include <exception>
#include <iterator> // std::iterator, std::input_iterator_tag
#include <map>
#include <string>
#include <string.h>

#define CONSOLEMENU_PARENTID_FORSPECIALMENU 9999
#define CONSOLEMENU_RECOMPUTEPARENT 9999
#define CONSOLEMENU_NOMENUKEY 9999

namespace CONSOLEMENU_NAMESPACE
{
static char _consolemenuversion[10];

class Menu
{

public:
    /**
 * @brief Construct a new console Menu object with default options
 * 
 */

    Menu(fp_IOdisplay displayCallback, fp_IOinput inputCallback)
    {
        _displayCallback = displayCallback;
        _inputCallback = inputCallback;
        internalInit();
    }

    Menu(fp_IOdisplay displayCallback, fp_IOinputId inputId)
    {
        _displayCallback = displayCallback;
        _inputId = inputId;
        internalInit();
    }

    /**
     * @brief Construct a new Menu object
     * use default callbacks for IO display and input
     */
    Menu()
    {
        _displayCallback = IoHelpers::IOdisplay;
        _inputId = Menu::waitforInputIntDefaultCallback;
        internalInit();
    }

    static char *getVersion()
    {
        sprintf(_consolemenuversion, "%i.%i.%i", CONSOLEMENU_VERSION_MAJOR, CONSOLEMENU_VERSION_MINOR, CONSOLEMENU_VERSION_REVISION);
        return _consolemenuversion;
    }

    /**
 * @brief Construct a new console Menu object with options specified
 * 
 * @param displayCallback function displaying the menu to the user from the complete char array
 * @param inputCallback function waiting the user input
 * @param options initialization options
 */
    Menu(fp_IOdisplay displayCallback, fp_IOinput inputCallback, MenuOptions options)
    {
        _displayCallback = displayCallback;
        _inputCallback = inputCallback;
        _menuoptions = options;
        // insert special menu entry for internal use:
        insertSpecialsMI();
    }

    /**
     * @brief add an item heading to a submenu, 
     * part of the menu setting to be done before calling displayMenu
     * 
     * @param menuname 
     * @param parentid : if this menuitem is under a submenu : id of the parent item.
     * @param menukey : key to use for this menu (the unicity of this key is under your concern, an exception will be thrown if it's not unique).
     * @return ushort : menuid created, to reference on the submenu items to setup underneath
     */
    ushort addHierarchyMenuitem(const char *menuname, ushort parentid, ushort menukey)
    {
        Menuitem newmenuitem = this->addMenuitem_internal(menuname, parentid, menutype::hierarchymenu, menukey);
        return newmenuitem.mid;
    }
    /**
     * @brief add an item heading to a submenu, 
     * part of the menu setting to be done before calling displayMenu
     * 
     * @param menuname 
     * @param parentid : if this menuitem is under a submenu : id of the parent item.
     * @return ushort : menuid created, to reference on the submenu items to setup underneath
     */
    ushort addHierarchyMenuitem(const char *menuname, ushort parentid)
    {
        return addHierarchyMenuitem(menuname, parentid, CONSOLEMENU_NOMENUKEY);
    }
    /**
     * @brief add a menu item calling a given function.
     * part of the menu setting to be done before calling displayMenu
     * 
     * @param menuname 
     * @param menuFonction : pointer to the function to call for this menu. NULL for a hierarchy menu. if if the return value of the callback is true : exit the menu after execution of this function, else stay in the current menu and wait for another action
     * @param parentid : if this menuitem is under a submenu : id of the parent item.
     * @return ushort : menuid created
     */
    ushort addCallbackMenuitem(const char *menuname, fp_callback1 menuFonction, ushort parentid)
    {
        Menuitem newmenuitem = this->addMenuitem_internal(menuname, parentid, menutype::externalFunction, CONSOLEMENU_NOMENUKEY);
        _menuCollection.at(newmenuitem.mid).SetCallback(menuFonction);
        return newmenuitem.mid;
    }

    /**
     * @brief add a menu item calling a given function.
     * part of the menu setting to be done before calling displayMenu
     * 
     * @param menuname 
     * @param menuFonction : pointer to the function to call for this menu. if the return value of the callback is true : exit the menu after execution of this function, else stay in the current menu and wait for another action
     * @param parentid : if this menuitem is under a submenu : id of the parent item.
     * @return ushort : menuid created
     */
    ushort addCallbackMenuitem(const char *menuname, fp_callback2 menuFonction, ushort parentid)
    {
        Menuitem newmenuitem = this->addMenuitem_internal(menuname, parentid, menutype::externalFunction, CONSOLEMENU_NOMENUKEY);
        _menuCollection.at(newmenuitem.mid).SetCallback(menuFonction);
        return newmenuitem.mid;
    }
    /**
     * @brief add a menu item calling a given function. This overload let you use a menu key to uniquely identify your menu and treat it specially in your callback. 
     * It can be a mean to group several callbacks into on function.
     * part of the menu setting to be done before calling displayMenu
     * 
     * @param menuname 
     * @param menuFonction : pointer to the function to call for this menu. NULL for a hierarchy menu. if if the return value of the callback is true : exit the menu after execution of this function, else stay in the current menu and wait for another action
     * @param parentid : if this menuitem is under a submenu : id of the parent item.
     * @param menukey : key to use for this menu (the unicity of this key is under your concern, an exception will be thrown if it's not unique).
     * this key is passed back to the callback
     * @return ushort : menuid created
     */
    ushort addCallbackMenuitem(const char *menuname, fp_callback3 menuFonction, ushort parentid, ushort menukey)
    {
        Menuitem newmenuitem = this->addMenuitem_internal(menuname, parentid, menutype::externalFunction, menukey);
        _menuCollection.at(newmenuitem.mid).SetCallback(menuFonction);
        newmenuitem.SetCallback(menuFonction);
        return newmenuitem.mid;
    }

    /**
     * @brief add a menu item calling a given function. This overload let you use a menu key to uniquely identify your menu and treat it specially in your callback. 
     * It can be a mean to group several callbacks into on function.
     * part of the menu setting to be done before calling displayMenu
     * 
     * @param menunamefunction : callback to a function provinding dynamically the menuitem name (possibly given the menukey)
     * @param menuFonction : pointer to the function to call for this menu. NULL for a hierarchy menu. if if the return value of the callback is true : exit the menu after execution of this function, else stay in the current menu and wait for another action
     * @param parentid : if this menuitem is under a submenu : id of the parent item.
     * @param menukey : key to use for this menu (the unicity of this key is under your concern, an exception will be thrown if it's not unique).
     * @return ushort : menuid created
     */
    ushort addDynamicCallbackMenuitem(fp_namingcallback menunamefunction, fp_callback3 menuFonction, ushort parentid, ushort menukey)
    {
        Menuitem newmenuitem = this->addMenuitem_internal("", parentid, menutype::externalFunction, menukey);
        _menuCollection.at(newmenuitem.mid).SetCallback(menuFonction);
        _menuCollection.at(newmenuitem.mid).SetNamingCallback(menunamefunction);
        return newmenuitem.mid;
    }

    /**
 * @brief add a menu item designed for one goal : update an string value in a given pointer to a variable.
 * 
 * @param menuname
 * @param parentid 
 * @param variableToUpdate : pointer to the variable to be updated
 * @param stringsize : size max of the c-string (buffer of char)
 * @param trials : number of given trials before issuing a failure and exits the assignement loop.
 * @return ushort : menuid created
 */
    ushort addUpdaterMenuitem(const char *menuname, ushort parentid, char *variableToUpdate, size_t stringsize)
    {
        Menuitem newmenuitem = this->addMenuitem_internal(menuname, parentid, menutype::variableUpdater_s, CONSOLEMENU_NOMENUKEY);
        _menuCollection.at(newmenuitem.mid).SetVarToUpdate(variableToUpdate);
        _menuCollection.at(newmenuitem.mid).SetInputTrials(_menuoptions.badInputRepeats);
        _menuCollection.at(newmenuitem.mid).SetStringToUpdateSize(stringsize);
        return newmenuitem.mid;
    }

    /**
 * @brief add a menu item designed for one goal : update an int value in a given pointer to a variable.
 * 
 * @param menuname
 * @param parentid 
 * @param variableToUpdate : pointer to the variable to be updated
 * @param trials : number of given trials before issuing a failure and exits the assignement loop.
 * @return ushort : menuid created
 */
    ushort addUpdaterMenuitem(const char *menuname, ushort parentid, int *variableToUpdate)
    {
        Menuitem newmenuitem = this->addMenuitem_internal(menuname, parentid, menutype::variableUpdater_i, CONSOLEMENU_NOMENUKEY);
        _menuCollection.at(newmenuitem.mid).SetVarToUpdate(variableToUpdate);
        _menuCollection.at(newmenuitem.mid).SetInputTrials(_menuoptions.badInputRepeats);
        return newmenuitem.mid;
    }

    /**
 * @brief add a menu item designed for one goal : update an int value in a given pointer to a variable.
 * 
 * @param menuname
 * @param parentid 
 * @param variableToUpdate : pointer to the variable to be updated
 * @param trials : number of given trials before issuing a failure and exits the assignement loop.
 * @return ushort : menuid created
 */
    ushort addUpdaterMenuitem(const char *menuname, ushort parentid, ushort *variableToUpdate)
    {
        Menuitem newmenuitem = this->addMenuitem_internal(menuname, parentid, menutype::variableUpdater_us, CONSOLEMENU_NOMENUKEY);
        _menuCollection.at(newmenuitem.mid).SetVarToUpdate(variableToUpdate);
        _menuCollection.at(newmenuitem.mid).SetInputTrials(_menuoptions.badInputRepeats);
        return newmenuitem.mid;
    }

    /**
 * @brief add a menu item designed for one goal : update an int value in a given pointer to a variable.
 * 
 * @param menuname
 * @param parentid 
 * @param variableToUpdate : pointer to the variable to be updated
 * @param trials : number of given trials before issuing a failure and exits the assignement loop.
 * @return ushort : menuid created
 */
    ushort addUpdaterMenuitem(const char *menuname, ushort parentid, unsigned char *variableToUpdate)
    {
        Menuitem newmenuitem = this->addMenuitem_internal(menuname, parentid, menutype::variableUpdater_uc, CONSOLEMENU_NOMENUKEY);
        _menuCollection.at(newmenuitem.mid).SetVarToUpdate(variableToUpdate);
        _menuCollection.at(newmenuitem.mid).SetInputTrials(_menuoptions.badInputRepeats);
        return newmenuitem.mid;
    }
    /**
 * @brief add a menu item designed for one goal : update an int value in a given pointer to a variable.
 * 
 * @param menuname
 * @param parentid 
 * @param variableToUpdate : pointer to the variable to be updated
 * @param trials : number of given trials before issuing a failure and exits the assignement loop.
 * @return ushort : menuid created
 */
    ushort addUpdaterMenuitem(const char *menuname, ushort parentid, double *variableToUpdate)
    {
        Menuitem newmenuitem = this->addMenuitem_internal(menuname, parentid, menutype::variableUpdater_d, CONSOLEMENU_NOMENUKEY);
        _menuCollection.at(newmenuitem.mid).SetVarToUpdate(variableToUpdate);
        _menuCollection.at(newmenuitem.mid).SetInputTrials(_menuoptions.badInputRepeats);
        return newmenuitem.mid;
    }

    /**
 * @brief add a menu item designed for one goal : update a bool value in a given pointer to a variable.
 * depending on the value of CONSOLEMENU_UPDATERMENU_BOOLMODESWITCH, in switch mode : a single call to this menu-item is enough to switch the value
 * if CONSOLEMENU_UPDATERMENU_BOOLMODESWITCH=0 the user is prompted for the new bool value (0/1 or y/n)
 * 
 * @param menuname
 * @param parentid 
 * @param variableToUpdate : pointer to the variable to be updated
 * @return ushort : menuid created
 */
    ushort addUpdaterMenuitem(const char *menuname, ushort parentid, bool *variableToUpdate)
    {
        Menuitem newmenuitem = this->addMenuitem_internal(menuname, parentid, menutype::variableUpdater_b, CONSOLEMENU_NOMENUKEY);
        _menuCollection.at(newmenuitem.mid).SetVarToUpdate(variableToUpdate);
        _menuCollection.at(newmenuitem.mid).SetInputTrials(1);
        return newmenuitem.mid;
    }
    /**
     * @brief Set  Options for meuns
     * 
     * @param options 
     */
    void setOptions(MenuOptions options)
    {
        _menuoptions = options;
    }

    /**
     * @brief  get the string (as a char array) to display the menu for the current hierarchy
     * 
     */
    void displayMenu()
    {
        displayMenu(0, 0);
    }

    /**
     * @brief to call in the main loop : listen to an input from the serial UI and react displaying the root menu
     * this method can be implemnted in your code, in this case, call Menu::displayMenu() when you want to display the root menu.
     * currently only implemented for arduino
     */
    void LoopCheckSerial()
    {
#if CONSOLEMENU_EMBEDDED_MODE
        if (!this->_isserialmenuative && Serial.available() > 0)
        {
            Serial.flush();
            // enter serial menu:
            this->_isserialmenuative = true;
            displayMenu();
            /**
         * while displaying console menu, the process loop on the user input, it is a sync process TODO : make it async
         */
            // Serial.println("menu exited");
            this->_isserialmenuative = false;
        }
#endif
    }

    /**
 * @brief Get a menuitem the By its menuKey
 * 
 * @param menukey 
 * @return Menuitem 
 */
    Menuitem getByKey(ushort menukey)
    {
        // std::map<ushort, Menuitem>::iterator it = this->_menuCollection.begin();
        // do
        // {
        //     if (it->second.mkey == menukey)
        //         return it->second;
        // } while (it != this->_menuCollection.end());
        ushort menuid = this->_menukeys.at(menukey);
        return this->_menuCollection.at(menuid);
    }

private:
    std::map<ushort, Menuitem> _menuCollection;
    std::map<ushort, ushort> _menukeys; // dictionary of menykeys:menuid
    fp_IOdisplay _displayCallback;
    fp_IOinput _inputCallback;
    fp_IOinputId _inputId;
    MenuOptions _menuoptions;
    bool _isMenuCollectionComplete = false;

#if CONSOLEMENU_EMBEDDED_MODE
    bool _isserialmenuative = false;
#endif

    void
    internalInit()
    {
        // insert special menu entry for internal use:
        insertSpecialsMI();
    }

    Menuitem addMenuitem_internal(const char *menuname, ushort parentid, menutype mtype, ushort menukey)
    {
        if (parentid > 0)
        {
            if (_menuCollection.find(parentid) == _menuCollection.end())
            {
                char err[50];
                sprintf(err, "Parent Linking error : no menu item found in the menuitem collection with id %i", parentid);
                throw std::runtime_error(err);
            }
        }
        ushort itemid = _menuCollection.size() + 1;
        Menuitem newmenuitem(menuname, itemid, parentid, mtype);
        newmenuitem.mkey = menukey;
        if (menukey != CONSOLEMENU_NOMENUKEY)
        {
            if (_menukeys.find(menukey) != _menukeys.end())
            {
                char err[50];
                sprintf(err, "a menuitem was already declared in the menuitem collection with ther key {}", menukey);
                throw std::runtime_error(err);
            }
            _menukeys[menukey] = itemid;
        }
        _menuCollection.insert(std::pair<ushort, Menuitem>(
            itemid,
            newmenuitem));

        return _menuCollection.at(itemid);
    }

    /**
     * @brief insert common specials menuitems displayed before the regular MI.
     * 
     */
    void insertSpecialsMI()
    {
        _menuCollection.insert(std::pair<ushort, Menuitem>(
            1,
            Menuitem("< back", 1, CONSOLEMENU_PARENTID_FORSPECIALMENU, menutype::back)));
    }

    /**
     * @brief append common specials menuitems displayed after the regular MI.
     * 
     */
    void appendSpecialsMI()
    {
        ushort itemid = _menuCollection.size();
        itemid++;
        _menuCollection.insert(std::pair<ushort, Menuitem>(
            itemid,
            Menuitem("> exit", itemid, CONSOLEMENU_PARENTID_FORSPECIALMENU, menutype::exit)));

        _isMenuCollectionComplete = true;
    }

    /**
 * @brief default implementation for fp_IOinput
 * 
 * @return ushort 
 */
    static ushort waitforInputIntDefaultCallback()
    {
        ushort i;
        if (IoHelpers::TakeUserInput("", &i, 1))
        {
            return i;
        }
        return 0;
    }

    void _internalAppendDisplay()
    {
    }

    /**
     * @brief compute the menu rendering given the context
     * for internal recursive calls
     * 
     * @param hierarchyId 
     * @param lasthierachyid 
     */
    void displayMenu(ushort hierarchyId, ushort lasthierachyid)
    {

        if (!_isMenuCollectionComplete)
            appendSpecialsMI();

        bool done = false;

        do
        {
            IoHelpers::IOdisplay("---------------------\n");
            ushort ix = 0;
            std::map<ushort, Menuitem>::iterator it;
            std::map<ushort, ushort> menuitems;

            //add the regular menus items for this hierarchy
            for (it = _menuCollection.begin(); it != _menuCollection.end(); ++it)
            {
                if (it->second.mparentid == hierarchyId ||
                    (it->second.mtype == menutype::back &&                                             // condition for back Menu
                     _menuoptions.addBack && hierarchyId > 0) ||                                       // condition for back Menu
                    (it->second.mtype == menutype::exit &&                                             // condition for exit Menu
                     (_menuoptions.addExitForEachLevel || (_menuoptions.addBack && hierarchyId == 0))) // condition for exit Menu
                )
                {
                    ix++;

                    std::string menuname;
                    if (it->second.mNamingFonction != NULL)
                    {
                        menuname = it->second.mNamingFonction(it->second.mkey);
                        it->second.mname = menuname;
                    }
                    else
                    {
                        menuname = it->second.mname;
                    }

                    IoHelpers::IOdisplay(ix);
                    IoHelpers::IOdisplay(_menuoptions.id_separator);
                    IoHelpers::IOdisplay(menuname.c_str());
                    if (it->second.isUpdaterMenuItem())
                    {
                        IoHelpers::IOdisplay(" [=");
                        it->second.displayCurrentValue();
                        IoHelpers::IOdisplay("]");
                    }
                    IoHelpers::IOdisplayLn("");

                    menuitems.insert(std::pair<ushort, ushort>(ix, it->second.mid));
                }
                if (lasthierachyid == CONSOLEMENU_RECOMPUTEPARENT && it->second.mtype == menutype::hierarchymenu && it->first == hierarchyId)
                    lasthierachyid = it->second.mparentid;
            }

            ushort menuitemid;
            IoHelpers::IOdisplay("please choose an action: >");

            try
            {
                ushort inputi = USHRT_MAX;
                //redisplay!! TODO
                if (_inputId != NULL)
                {
                    inputi = _inputId();
                }
                else if (_inputCallback != NULL)
                {
                    const char *input = _inputCallback();
#if CONSOLEMENU_EMBEDDED_MODE
                    inputi = (ushort)atoi(input);
#else
                    inputi = std::stoi(input);
#endif
                }
#if CONSOLEMENU_EMBEDDED_MODE
                // on a computer with screen, the input is already displayed
                char strbuff[3];
                sprintf(strbuff, "%i", inputi);
                _displayCallback(strbuff);
#endif
                menuitemid = menuitems.at(inputi);
                std::map<ushort, Menuitem>::iterator it2 = _menuCollection.find(menuitemid);
                if (it2 != _menuCollection.end())
                {
                    Menuitem mi = it2->second;

                    switch (mi.mtype)
                    {
                    case menutype::hierarchymenu:
                        _displayCallback("\n");
                        //recursive call to display the underlying menu
                        displayMenu(it2->first, hierarchyId);
                        return;
                    case menutype::back:
                        _displayCallback("\n");
                        displayMenu(lasthierachyid, CONSOLEMENU_RECOMPUTEPARENT);
                        return;
                    case menutype::exit:
                        _displayCallback(">exited\n");
                        return;
                    case menutype::externalFunction:
                        _displayCallback("\n");
                        done = callMenuCallbackFunction(mi);
                        // if (!done && mi.mNamingFonction != NULL)
                        // {
                        //     //recursive call to display again the menu and reload the menu display callback
                        //     displayMenu(hierarchyId, lasthierachyid);
                        //     return;
                        // }
                        // if (done)
                        // _displayCallback(">exited\n");
                        break;
                    default:
                        if (mi.isUpdaterMenuItem())
                        {
                            done = variableUpdater(mi);
                            // if (!done && mi.mNamingFonction != NULL)
                            // {
                            //     //recursive call to display again the menu and reload the menu display callback
                            //     displayMenu(hierarchyId, lasthierachyid);
                            //     return;
                            // }
                            // if (done)
                            //     _displayCallback(">exited\n");
                        }
                        else
                            throw std::runtime_error("this menutype is not implemented");
                        break;
                    }
                }
            }
            catch (const std::exception &e)
            {
                continue;
            }
        } while (!done);
        _displayCallback(">exited\n");
    }

    bool variableUpdater(Menuitem mi)
    {
        char *vartoupdate_s;
        int vartoupdate_i;
        IoHelpers::IOdisplay("current value is : ");
        mi.displayCurrentValue();
        IoHelpers::IOdisplayLn("");
        mi.takeUserInput();
        return false;
    }
    /**
 * @brief call the menu function
 * 
 * @param mi 
 * @return true if call was successfull (return bool from the callback).
 * if not successfull, prompt again in the outside loop. 
 */
    bool callMenuCallbackFunction(Menuitem mi)
    {
        if (mi.mFonction_form3 != NULL)
            return mi.mFonction_form3(mi.mkey, mi.mname.c_str());
        if (mi.mFonction_form2 != NULL)
            return mi.mFonction_form2(mi.mname.c_str());
        if (mi.mFonction_form1 != NULL)
            return mi.mFonction_form1();
        return false;
    }
};
} // namespace CONSOLEMENU_NAMESPACE
