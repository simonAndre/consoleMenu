
#pragma once

// #include "Menuitem.hpp"
#include <exception>
#include <iterator> // std::iterator, std::input_iterator_tag
#include <map>
#include <string>

#define PARENTID_FORSPECIALMENU 9999
#define RECOMPUTEPARENT 9999

namespace CONSOLEMENU_NAMESPACE
{

class Menu
{

public:
    /**
 * @brief Construct a new console Menu object with default options
 * 
 */
    Menu(pf_IOdisplay displayCallback, pf_IOinput inputCallback)
    {
        _displayCallback = displayCallback;
        _inputCallback = inputCallback;
        // insert special menu entry for internal use:
        insertSpecialsMI();
    }

    /**
 * @brief Construct a new console Menu object with options specified
 * 
 * @param displayCallback function displaying the menu to the user from the complete char array
 * @param inputCallback function waiting the user input
 * @param options initialization options
 */
    Menu(pf_IOdisplay displayCallback, pf_IOinput inputCallback, MenuOptions options)
    {
        _displayCallback = displayCallback;
        _inputCallback = inputCallback;
        _menuoptions = options;
        // insert special menu entry for internal use:
        insertSpecialsMI();
    }

    /**
     * @brief add an item to the menu, part of the menu setting to be done before calling displayMenu
     * 
     * @param menuname 
     * @param menuFonction : pointeur to the function to call for this menu. NULL for a hierarchy menu. if if the return value of the callback is true : exit the menu after execution of this function, else stay in the current menu and wait for another action
     * @param parentid : for a submenu : id of the parent item.
     * @return ushort : menuid created, to reference on the submenu if this menu is supposed to be a hierarchy menu
     */
    ushort addMenuitem(const char *menuname, pf_callback menuFonction, ushort parentid)
    {
        if (parentid > 0)
        {
            if (_menuCollection.find(parentid) == _menuCollection.end())
                throw std::runtime_error("no menu item are already declared in the menuitem collection for this id");
        }
        ushort itemid = _menuCollection.size() + 1;
        if (menuFonction == NULL)
            _menuCollection.insert(std::pair<ushort, Menuitem>(
                itemid,
                Menuitem(menuname, itemid, parentid, NULL, menutype::hierarchymenu)));
        else
            _menuCollection.insert(std::pair<ushort, Menuitem>(
                itemid,
                Menuitem(menuname, itemid, parentid, menuFonction, menutype::externalFunction)));

        return itemid;
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

private:
    std::map<ushort, Menuitem> _menuCollection;
    pf_IOdisplay _displayCallback;
    pf_IOinput _inputCallback;
    MenuOptions _menuoptions;
    bool _isMenuCollectionComplete = false;

    /**
     * @brief insert common specials menuitems displayed before the regular MI.
     * 
     */
    void insertSpecialsMI()
    {
        _menuCollection.insert(std::pair<ushort, Menuitem>(
            1,
            Menuitem("< back", 1, PARENTID_FORSPECIALMENU, NULL, menutype::back)));
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
            Menuitem("> exit", itemid, PARENTID_FORSPECIALMENU, NULL, menutype::exit)));

        _isMenuCollectionComplete = true;
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
        std::string display("");
        ushort ix = 0;
        std::map<ushort, Menuitem>::iterator it;
        std::map<ushort, ushort> menuitems;

        if (!_isMenuCollectionComplete)
            appendSpecialsMI();

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
#if CONSOLEMENU_EMBEDDED_MODE
                char buff[4];
                sprintf(buff, "%i", ix);
                display.append(buff).append(_menuoptions.id_separator).append(it->second.mname).append("\n");
#else
                display.append(std::to_string(ix)).append(_menuoptions.id_separator).append(it->second.mname).append("\n");
#endif
                menuitems.insert(std::pair<ushort, ushort>(ix, it->second.mid));
            }
            if (lasthierachyid == RECOMPUTEPARENT && it->second.mtype == menutype::hierarchymenu && it->first == hierarchyId)
                lasthierachyid = it->second.mparentid;
        }

        ushort menuitemid;
        bool done = false;
        display.append("please choose a menu: >");
        do
        {
            _displayCallback(display.c_str());
            const char *input = _inputCallback();
            try
            {
                ushort inputi = USHRT_MAX;
#if CONSOLEMENU_EMBEDDED_MODE
                inputi = (ushort)atoi(input);
#else
                inputi = std::stoi(input);
#endif

                menuitemid = menuitems.at(inputi);
                std::map<ushort, Menuitem>::iterator it2 = _menuCollection.find(menuitemid);
                if (it2 != _menuCollection.end())
                {
                    Menuitem mi = it2->second;

                    switch (mi.mtype)
                    {
                    case menutype::hierarchymenu:
                        //recursive call to display the underlying menu
                        displayMenu(it2->first, hierarchyId);
                        return;
                    case menutype::back:
                        displayMenu(lasthierachyid, RECOMPUTEPARENT);
                        return;
                    case menutype::exit:
                        return;
                    case menutype::externalFunction:
                        //call the menu function
                        // if not successfull, prompt again in the outside loop.
                        done = mi.mFonction(mi.mname.c_str());
                        break;
                    default:
                        break;
                    }
                }
            }
            catch (const std::exception &e)
            {
                continue;
            }

        } while (!done);
    }
};
} // namespace CONSOLEMENU_NAMESPACE
