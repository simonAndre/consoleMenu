
// #pragma once

// #include "Menuitem.hpp"
#include <exception>
#include <iterator> // std::iterator, std::input_iterator_tag
#include <map>
#include <string>
#include "Menubase.h"
#include "MenuitemBack.h"

namespace CONSOLEMENU_NAMESPACE
{

template <ushort sizeMenu>
class Menu : public Menubase
{

public:
    /**
 * @brief Construct a new console Menu object with options specified
 * 
 * @param displayCallback function displaying the menu to the user from the complete char array
 * @param inputCallback function waiting the user input
 * @param options initialization options
 */
    Menu(fp_IOdisplay displayCallback, fp_IOinput inputCallback, MenuOptions options) : Menubase(options)
    {
        _displayCallback = displayCallback;
        _inputCallback = inputCallback;
        this->setOptions(options);
        internalInit();
    }

    /**
 * @brief Construct a new console Menu object with default options
 * 
 */
    Menu(fp_IOdisplay displayCallback, fp_IOinput inputCallback) : Menubase()
    {
        _displayCallback = displayCallback;
        _inputCallback = inputCallback;
        internalInit();
    }

    Menu(fp_IOdisplay displayCallback, fp_IOinputId inputId) : Menubase()
    {
        _displayCallback = displayCallback;
        _inputMenuEntry = inputId;
        internalInit();
    }

    /**
     * @brief Construct a new Menu object
     * use default callbacks for IO display and input
     */
    Menu() : Menubase()
    {
        _displayCallback = (fp_IOdisplay)IoHelpers::IOdisplay<const char *>;
        _inputMenuEntry = Menu::waitforInputIntDefaultCallback;
        internalInit();
    }

    virtual SubMenu *getRootMenu() override
    {
        return _rootmenuitem;
    }

    virtual bool addChild(SubMenu *parent, Menuitem *child) override
    {
        child->SetParent(parent);
        try
        {
            insertMewMenuitem(child);
        }
        catch (std::runtime_error &e)
        {
            IoHelpers::IOdisplayLn(e.what());
            return false;
        }
        return true;
    }

    virtual SubMenu *addSubMenu(const char *label) override
    {
       return  _rootmenuitem->addSubMenu(label);
    }

    /**
     * @brief  get the string (as a char array) to display the menu for the current hierarchy
     * 
     */
    virtual void launchMenu() override
    {
        launchMenu(_rootmenuitem, nullptr);
    }

    /**
     * @brief to call in the main loop : listen to an input from the serial UI and react displaying the root menu
     * this method can be implemnted in your code, in this case, call Menu::launchMenu() when you want to display the root menu.
     * currently only implemented for arduino
     */
    virtual void LoopCheckSerial() override
    {
#if CONSOLEMENU_EMBEDDED_MODE
        if (!this->_isserialmenuative && Serial.available() > 0)
        {
            Serial.flush();
            // enter serial menu:
            this->_isserialmenuative = true;
            launchMenu();
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
    virtual Menuitem *getByKey(ushort menukey) override
    {
        ushort menuid = this->_menukeys.at(menukey);
        return _menuArray[menuid];
    }

    virtual Menuitem *getById(ushort menuid) override
    {
        return _menuArray[menuid];
    }

    virtual ushort size() override
    {
        return this->_lastmenuindex;
    }

    virtual void displayMenu(SubMenu *parent) override
    {
        displayMenu(parent, nullptr);
    }

    virtual void addMenuKey(ushort menukey, ushort menuid) override
    {
        if (_menukeys.find(menukey) != _menukeys.end())
        {
            memset(_errorbuffer, 0, sizeof _errorbuffer);
            sprintf(_errorbuffer, "a menuitem was already declared in the menuitem collection with the key {}", menukey);
            throw std::runtime_error(_errorbuffer);
        }
        _menukeys[menukey] = menuid;
    }





private:
    std::array<Menuitem *, sizeMenu + 2> _menuArray; // sizeMenu+2 to handle 2 added special menuitems : back and exit
    std::map<ushort, ushort> _menukeys;              // dictionary of menykeys:menuid
    ushort _lastmenuindex = 0;
    SubMenu *_rootmenuitem{nullptr};
    MenuitemBack *_backmenuitem{nullptr};
    Menuitem *_exitmenuitem{nullptr};
    fp_IOdisplay _displayCallback;
    fp_IOinput _inputCallback;
    fp_IOinputId _inputMenuEntry;
    char _errorbuffer[150]{'\0'};
    bool _finalconfigurationDone = false;

#if CONSOLEMENU_EMBEDDED_MODE
    bool _isserialmenuative = false;
#endif

    void insertMewMenuitem(Menuitem *mi)
    {
        if (_lastmenuindex > sizeMenu + 2)
        {
            memset(_errorbuffer, 0, sizeof _errorbuffer);
            sprintf(_errorbuffer, "too many items for this menu collection, try to increase the template parameter [sizeMenu], current value is : %i", sizeMenu);
            throw std::runtime_error(_errorbuffer);
        }

        mi->SetId(_lastmenuindex);

        if (mi->getMenuKey() != CONSOLEMENU_NOMENUKEY)
        {
            this->addMenuKey(mi->getMenuKey(), mi->getId());
        }
        _menuArray[_lastmenuindex] = mi;
        _lastmenuindex++;
    }

    /**
 * @brief default implementation for fp_IOinput
 * 
 * @return ushort 
 */
    static ushort waitforInputIntDefaultCallback()
    {
        ushort i;
        try
        {
            if (IoHelpers::TakeUserInput("", &i, 1, _menuDefaultTimeout))
            {
                return i;
            }
        }
        catch (std::runtime_error &e)
        {
            IoHelpers::IOdisplayLn(e.what());
        }
        return 0;
    }

    bool internalInit()
    {

        _rootmenuitem = new SubMenu(this, "");
        Menuitem *miroot = (Menuitem *)_rootmenuitem;

        try
        {
            insertMewMenuitem(miroot);
        }
        catch (std::runtime_error &e)
        {
            delete _rootmenuitem;
            IoHelpers::IOdisplay("error inserting root menu-item");
            IoHelpers::IOdisplayLn(e.what());
            return false;
        }

        _backmenuitem = new MenuitemBack(this);
        try
        {
            insertMewMenuitem(_backmenuitem);
        }
        catch (std::runtime_error &e)
        {
            delete _backmenuitem;
            IoHelpers::IOdisplay("error inserting back menu-item");
            IoHelpers::IOdisplayLn(e.what());
            return false;
        }

        return true;
    }

    bool finalConfiguration()
    {
        for (ushort i = 0; i < _lastmenuindex; i++)
        {
            Menuitem *mi = _menuArray[i];
            if (mi && mi->getType() == menutype::hierarchymenu){
                // add grouped callbacks to every childs

                SubMenu *sm = (SubMenu *)mi;
                Menuitem **childs = sm->getChilds();
                std::vector<fp_callback1>::const_iterator itcallbacks;
                for (itcallbacks = sm->callbacksForChilds.begin(); itcallbacks != sm->callbacksForChilds.end(); ++itcallbacks)
                {
                    for (size_t j = 0; j < sm->getChildCount(); j++)
                    {
                        Menuitem* childmi = childs[j];
                        childmi->addCallback(*itcallbacks);
                    }
                }
                delete childs;
            }
        }

        // add exit menu items
        if (!_exitmenuitem)
        {
            _exitmenuitem = new Menuitem(this, CONSOLEMENU_MENU_EXIT, NULL, menutype::exit);
            try
            {
                insertMewMenuitem(_exitmenuitem);
            }
            catch (std::runtime_error &e)
            {
                delete _exitmenuitem;
                IoHelpers::IOdisplay("error inserting exit menu-item");
                IoHelpers::IOdisplayLn(e.what());
                return false;
            }
        }

        return true;
    }

    /**
 * @brief display the menu at a specific hierarchy level
 * 
 * @param parent : the hierarchy menu-item parent of the submenu to display
 * @param backparent : point to the hierarchy menu-item fo the displayed back menu-item (if one on this submenu)
 * @return std::map<ushort, ushort> 
 */
    std::map<ushort, ushort> displayMenu(SubMenu *parent, SubMenu *backparent)
    {

        if (!_finalconfigurationDone)
        {
            finalConfiguration();
            _finalconfigurationDone = true;
        }

        if (parent == _rootmenuitem)
            IoHelpers::IOdisplay(CONSOLEMENU_MENU_HEADERLINE);
        else
        {
            IoHelpers::IOdisplay("---- ");
            IoHelpers::IOdisplay(parent->getLabel());
            IoHelpers::IOdisplay(" ----\n");
        }

        ushort ix = 0;
        std::map<ushort, ushort> menuitems;

        //add the regular menus items for this hierarchy
        for (ushort i = 0; i < _lastmenuindex; i++)
        {
            Menuitem *mi = _menuArray[i];
            if (mi)
            {
                if ((mi->getParent() && mi->getParent() == parent) ||
                    (mi == (Menuitem *)_backmenuitem && _menuoptions.addBack && parent != _rootmenuitem) ||                                      // condition for back Menu
                    (mi == (Menuitem *)_exitmenuitem && (_menuoptions.addExitForEachLevel || (_menuoptions.addBack && parent == _rootmenuitem))) // condition for exit Menu
                )
                {
                    ix++;
                    mi->display(ix);
                    if (mi->getType() == menutype::back)
                    {
                        MenuitemBack *mib = (MenuitemBack *)mi;
                        mib->SetParent(parent);
                    }
                    menuitems.insert(std::pair<ushort, ushort>(ix, mi->getId()));
                }
                // if (!backparent && parent != _rootmenuitem && mi->getType() != menutype::back && mi->getType() != menutype::exit)
                //     backparent = mi->getParent();
            }
        }

        return menuitems;
    }

    /**
 * @brief compute the menu rendering given the context
 * for internal recursive calls
 * 
 * @param parent 
 * @param backparent 
 */
    void launchMenu(SubMenu *parent, SubMenu *backparent)
    {
        bool done = false;
        
        _menuDefaultTimeout = _menuoptions.expirationTimeSec;

        do
        {
            std::map<ushort, ushort> menuitems = displayMenu(parent, backparent);

            IoHelpers::IOdisplay(CONSOLEMENU_MENU_CHOOSEACTION);

            try
            {
                ushort inputi = USHRT_MAX;
                //redisplay!! TODO
                if (_inputMenuEntry)
                {
                    inputi = _inputMenuEntry();
                }
                else if (_inputCallback)
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
                char strbuff[4];
                sprintf(strbuff, "%i\n", inputi);
                _displayCallback(strbuff);
#endif
                if (inputi == 0)
                    done = true;
                else if (menuitems.find(inputi) != menuitems.end())
                {
                    Menuitem *mi = _menuArray[menuitems.at(inputi)];
                    SubMenu *mih = (SubMenu *)mi;
                    SelectActionResult actionres;
                    switch (mi->getType())
                    {
                    case menutype::hierarchymenu:
                        _displayCallback("\n");
                        //recursive call to display the underlying menu
                        launchMenu(mih, parent);
                        return;
                        break;
                    case menutype::back:
                        _displayCallback("\n");
                        launchMenu(((MenuitemBack *)parent)->getParent(), nullptr);
                        return;
                        break;
                    case menutype::exit:
                        done = true;
                        return;
                        break;
                    default:
                        actionres = mi->selectAction();
                        done = actionres.exitRequested;
                        break;
                    }
                }
            }
            catch (const std::exception &e)
            {
                continue;
            }
        } while (!done);
        _displayCallback(CONSOLEMENU_MENU_EXITED);
    }
};
} // namespace CONSOLEMENU_NAMESPACE
