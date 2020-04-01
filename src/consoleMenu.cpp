#include <exception>
#include <iterator> // std::iterator, std::input_iterator_tag
#include "consoleMenu.h"

using namespace std;
using namespace consoleMenu;

#define PARENTID_FORSPECIALMENU 9999
#define RECOMPUTEPARENT 9999

#ifdef ARDUINO
// arduino code
#endif
/*
* @menuname
*/
Menu::Menuitem::Menuitem(const char *menuname, ushort id, ushort parentid, pf_callback menuFonction, Menu::menutype type)
{
    mid = id;
    mparentid = parentid;
    mname = string(menuname);
    mFonction = menuFonction;
    mtype = type;
}

// insert common specials menuitems displayed before the regular MI.
void Menu::insertSpecialsMI()
{
    _menuCollection.insert(pair<ushort, Menuitem>(
        1,
        Menuitem("< back", 1, PARENTID_FORSPECIALMENU, NULL, Menu::menutype::back)));
}
// append common specials menuitems displayed after the regular MI.
void Menu::appendSpecialsMI()
{
    ushort itemid = _menuCollection.size();
    itemid++;
    _menuCollection.insert(pair<ushort, Menuitem>(
        itemid,
        Menuitem("> exit", itemid, PARENTID_FORSPECIALMENU, NULL, Menu::menutype::exit)));

    _isMenuCollectionComplete = true;
}
// Menu::Menu(pf_IOdisplay displayCallback, pf_IOinput inputCallback)
// {
//     _displayCallback = displayCallback;
//     _inputCallback = inputCallback;
//     // insert special menu entry for internal use:
//     insertSpecialsMI();
// }
Menu::Menu(pf_IOdisplay displayCallback, pf_IOinput inputCallback, MenuOptions options)
{
    _displayCallback = displayCallback;
    _inputCallback = inputCallback;
    _menuoptions = options;
    // insert special menu entry for internal use:
    insertSpecialsMI();
}

// Add a menu item
// menuname : name
// parentid : for a submenu : id of the parent item.
// menuFonction : pointeur to the function to call for this menu. NULL for a hierarchy menu. if if the return value of the callback is true : exit the menu after execution of this function, else stay in the current menu and wait for another action
// return : menuid
ushort Menu::addMenuitem(const char *menuname, pf_callback menuFonction, ushort parentid)
{
    if (parentid > 0)
    {
        if (_menuCollection.find(parentid) == _menuCollection.end())
            throw runtime_error("no menu item are already declared in the menuitem collection for this id");
    }
    ushort itemid = _menuCollection.size() + 1;
    if (menuFonction == NULL)
        _menuCollection.insert(pair<ushort, Menuitem>(
            itemid,
            Menuitem(menuname, itemid, parentid, NULL, Menu::menutype::hierarchymenu)));
    else
        _menuCollection.insert(pair<ushort, Menuitem>(
            itemid,
            Menuitem(menuname, itemid, parentid, menuFonction, Menu::menutype::externalFunction)));

    return itemid;
}
void Menu::displayMenu()
{
    displayMenu(0, 0);
}

//for internal recursive calls
void Menu::displayMenu(short hierarchyId, short lasthierachyid)
{
    string display("");
    ushort ix = 0;
    map<ushort, Menu::Menuitem>::iterator it;
    map<ushort, ushort> menuitems;

    if (!_isMenuCollectionComplete)
        appendSpecialsMI();

    //add the regular menus items for this hierarchy
    for (it = _menuCollection.begin(); it != _menuCollection.end(); ++it)
    {
        if (it->second.mparentid == hierarchyId ||
            (it->second.mtype == Menu::menutype::back &&                                       // condition for back Menu
             _menuoptions.addBack && hierarchyId > 0) ||                                       // condition for back Menu
            (it->second.mtype == Menu::menutype::exit &&                                       // condition for exit Menu
             (_menuoptions.addExitForEachLevel || (_menuoptions.addBack && hierarchyId == 0))) // condition for exit Menu
        )
        {
            display.append(to_string(++ix)).append(_menuoptions.id_separator).append(it->second.mname).append("\n");
            menuitems.insert(pair<ushort, ushort>(ix, it->second.mid));
        }
        if (lasthierachyid == RECOMPUTEPARENT && it->second.mtype == Menu::menutype::hierarchymenu && it->first == hierarchyId)
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
            inputi = stoi(input);
            menuitemid = menuitems.at(inputi);
            map<ushort, Menu::Menuitem>::iterator it2 = _menuCollection.find(menuitemid);
            if (it2 != _menuCollection.end())
            {
                Menuitem mi = it2->second;

                switch (mi.mtype)
                {
                case Menu::menutype::hierarchymenu:
                    //recursive call to display the underlying menu
                    displayMenu(it2->first, hierarchyId);
                    return;
                case Menu::menutype::back:
                    displayMenu(lasthierachyid, RECOMPUTEPARENT);
                    return;
                case Menu::menutype::exit:
                    return;
                case Menu::menutype::externalFunction:
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
