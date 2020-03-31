#include <exception>
#include <iterator> // std::iterator, std::input_iterator_tag
#include "consoleMenu.h"
using namespace std;

consoleMenu::Menuitem::Menuitem(const char *menuname, ushort id, ushort parentid, pf_menu menuFonction, consoleMenu::menutype type)
{
    mid = id;
    mparentid = parentid;
    mname = string(menuname);
    mFonction = menuFonction;
    mtype = type;
}

consoleMenu::consoleMenu(pf_display displayCallback, pf_input inputCallback, MenuOptions options)
{
    _displayCallback = displayCallback;
    _inputCallback = inputCallback;
    _menuoptions = options;
}

// Add a menu item
// menuname : name
// parentid : for a submenu : id of the parent item.
// menuFonction : pointeur to the function to call for this menu. NULL for a hierarchy menu.
// return : menuid
ushort consoleMenu::addMenuitem(const char *menuname, pf_menu menuFonction, ushort parentid)
{
    if (parentid > 0)
    {
        if (_menuCollection.find(parentid) == _menuCollection.end())
            throw runtime_error("no menu item are already declared in the menuitem collection for this id");
    }
    ushort itemid = _menuCollection.size() + 1;
    // ushort index = getHierarchyCollectionCardinality(parentid) + 1;
    Menuitem newmi(menuname, itemid, parentid, menuFonction, consoleMenu::menutype::externalFunction);
    _menuCollection.insert(pair<ushort, Menuitem>(itemid, newmi));
    return itemid;
}

void consoleMenu::displayMenu()
{
    string display("");
    ushort ix = 0;
    map<ushort, consoleMenu::Menuitem>::iterator it;
    map<ushort, ushort> menuitems;
    // add menu back if we are in a hierarchy menu
    if (_menuoptions.addBack && this->_hierarchyIx > 0)
    {
        display.append(to_string(++ix)).append(_menuoptions.id_separator).append("< back\n");
        menuitems.insert(pair<ushort, ushort>(ix, 99998));
    }
    //add the regular menus items for this hierarchy
    for (it = _menuCollection.begin(); it != _menuCollection.end(); ++it)
        if (it->second.mparentid == this->_hierarchyIx)
        {
            display.append(to_string(++ix)).append(_menuoptions.id_separator).append(it->second.mname).append("\n");
            menuitems.insert(pair<ushort, ushort>(ix, it->second.mid));
        }

    //add menu exit
    if (_menuoptions.addExitForEachLevel || (_menuoptions.addBack && this->_hierarchyIx == 0))
    {
        display.append(to_string(++ix)).append(_menuoptions.id_separator).append("> exit\n");
        menuitems.insert(pair<ushort, ushort>(ix, 99999));
    }
    ushort menuitemid;
    do
    {
        display.append("please choose a menu: >");
        _displayCallback(display.c_str());
        const char *input = _inputCallback();
        try
        {
            ushort inputi = USHRT_MAX;
            inputi = stoi(input);
            menuitemid = menuitems.at(inputi);
        }
        catch (...)
        {
            continue;
        }

    } while (menuitemid == 0 || !Selection(menuitemid));
}

bool consoleMenu::Selection(short menuitemid)
{
    map<ushort, Menuitem>::iterator it;
    it = _menuCollection.find(menuitemid);
    if (it == _menuCollection.end())
        return false;
    if (it->second.mFonction == NULL)
    {
        _hierarchyIx = it->first;
        displayMenu();
    }
    else
    {
        //call the menu function
        it->second.mFonction();
    }
    return true;
}

// return the number of items under this hierarchy
ushort consoleMenu::getHierarchyCollectionCardinality(ushort hierarchyid)
{
    ushort count = 0;
    map<ushort, Menuitem>::iterator it = _menuCollection.begin();
    do
    {
        if (it->second.mparentid == hierarchyid)
        {
            count++;
        }
        it++;
    } while (it != _menuCollection.end());
    return count;
}
