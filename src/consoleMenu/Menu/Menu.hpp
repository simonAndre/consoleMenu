
#pragma once

// #include "Menuitem.hpp"
#include <exception>
#include <iterator> // std::iterator, std::input_iterator_tag
#include <map>
#include <string>
#include "Menubase.h"

namespace CONSOLEMENU_NAMESPACE
{
// //forward declaration
// class Menuitem;

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
        _menuoptions = options;
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
        _inputId = inputId;
        internalInit();
    }

    /**
     * @brief Construct a new Menu object
     * use default callbacks for IO display and input
     */
    Menu() : Menubase()
    {
        _displayCallback = (fp_IOdisplay)IoHelpers::IOdisplay<const char *>;
        _inputId = Menu::waitforInputIntDefaultCallback;
        internalInit();
    }

    virtual MenuitemHierarchy *getRootMenu() override
    {
        return _rootmenuitem;
    }

    virtual bool addChild(MenuitemHierarchy *parent, Menuitem *child) override
    {
        child->setParent(parent);
        insertMewMenuitem(child);
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
    void LoopCheckSerial()
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
        this->_lastmenuindex;
    }

    virtual void displayMenu(MenuitemHierarchy *parent) override
    {
        displayMenu(parent, nullptr);
    }

private:
    std::array<Menuitem *, sizeMenu + 2> _menuArray{nullptr}; // sizeMenu+2 to handle 2 added special menuitems : back and exit
    std::map<ushort, ushort> _menukeys;                       // dictionary of menykeys:menuid
    ushort _lastmenuindex = 0;
    MenuitemHierarchy *_rootmenuitem{nullptr};
    MenuitemBack *_backmenuitem{nullptr};
    Menuitem *_exitmenuitem{nullptr};
    fp_IOdisplay _displayCallback;
    fp_IOinput _inputCallback;
    fp_IOinputId _inputId;
    char _errorbuffer[150];

#if CONSOLEMENU_EMBEDDED_MODE
    bool _isserialmenuative = false;
#endif

    bool insertMewMenuitem(Menuitem *mi)
    {
        if (_lastmenuindex > sizeMenu + 2)
        {
            memset(_errorbuffer, 0, sizeof _errorbuffer);
            sprintf(_errorbuffer, "too many items for this menu collection, try to increase the template parameter [sizeMenu], current value is : %i", sizeMenu);
            throw std::runtime_error(_errorbuffer);
        }

        mi->setId(_lastmenuindex);

        if (mi->getMenuKey() != CONSOLEMENU_NOMENUKEY)
        {
            if (_menukeys.find(mi->getMenuKey()) != _menukeys.end())
            {
                memset(_errorbuffer, 0, sizeof _errorbuffer);
                sprintf(_errorbuffer, "a menuitem was already declared in the menuitem collection with the key {}", mi->getMenuKey());
                return false;
            }
            _menukeys[mi->getMenuKey()] = mi->getId();
        }
        _menuArray[_lastmenuindex] = mi;
        _lastmenuindex++;
        return true;
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

    bool internalInit()
    {
        _rootmenuitem = new MenuitemHierarchy(this, "");
        Menuitem *miroot = (Menuitem *)_rootmenuitem;
        if (!insertMewMenuitem(miroot))
        {
            delete _rootmenuitem;
            return false;
        }
        _backmenuitem = new MenuitemBack(this);
        if (!insertMewMenuitem(_backmenuitem))
        {
            delete _backmenuitem;
            return false;
        }

        return true;
    }

    bool appendLastItems()
    {
        _exitmenuitem = new Menuitem(this, CONSOLEMENU_MENU_EXIT, NULL, menutype::exit);
        if (!insertMewMenuitem(_exitmenuitem))
        {
            delete _exitmenuitem;
            return false;
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
    std::map<ushort, ushort> displayMenu(MenuitemHierarchy *parent, MenuitemHierarchy *backparent)
    {

        if (!_exitmenuitem)
        {
            appendLastItems();
        }

        if (parent == _rootmenuitem)
            IoHelpers::IOdisplay(CONSOLEMENU_MENU_HEADERLINE);
        else
        {
            IoHelpers::IOdisplay("---- ");
            IoHelpers::IOdisplay(parent->getName());
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
                        mib->setParent(parent);
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
    void launchMenu(MenuitemHierarchy *parent, MenuitemHierarchy *backparent)
    {
        bool done = false;

        do
        {
            std::map<ushort, ushort> menuitems = displayMenu(parent, backparent);

            IoHelpers::IOdisplay(CONSOLEMENU_MENU_CHOOSEACTION);

            try
            {
                ushort inputi = USHRT_MAX;
                //redisplay!! TODO
                if (_inputId)
                {
                    inputi = _inputId();
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
                char strbuff[3];
                sprintf(strbuff, "%i", inputi);
                _displayCallback(strbuff);
#endif
                if (menuitems.find(inputi) != menuitems.end())
                {
                    Menuitem *mi = _menuArray[menuitems.at(inputi)];
                    MenuitemHierarchy *mih = (MenuitemHierarchy *)mi;
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
                        done = mi->selectAction();
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
