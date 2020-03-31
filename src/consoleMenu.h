
#include <map>
#include <string>
// callback called on menu selection.
// if the return value is true : exit the menu after execution of this function,
// else stay in the current menu and wait for another action
// callback param will be the menuitem name
typedef bool (*pf_callback)(const char *);
typedef void (*pf_IOdisplay)(const char *);
typedef const char *(*pf_IOinput)(void);
typedef unsigned short ushort;

typedef struct
{
    bool addBack = true;
    bool addExitForEachLevel = true; //if false and addBack is true : the exist menuitem is only available at the root
    const char *id_separator = " - ";
} MenuOptions;

class consoleMenu
{

private:
    enum menutype
    {
        externalFunction = 0,
        exit = 1, //exit the menu
        back = 2, //back to the parent menuitem
        hierarchymenu = 3
    };

    class Menuitem
    {
    private:
    public:
        Menuitem(const char *menuname, ushort id, ushort parentid, pf_callback menuFonction, consoleMenu::menutype type);
        ushort mid;
        ushort mparentid = 0;
        std::string mname;
        consoleMenu::menutype mtype;
        pf_callback mFonction;
    };

    std::map<ushort, Menuitem> _menuCollection;
    pf_IOdisplay _displayCallback;
    pf_IOinput _inputCallback;
    MenuOptions _menuoptions;
    bool _isMenuCollectionComplete = false;
    // insert common specials menuitems displayed before the regular MI.
    void
    insertSpecialsMI();
    // append common specials menuitems displayed after the regular MI.
    void appendSpecialsMI();
    void displayMenu(short hierarchyId, short lasthierachyid);

public:
    consoleMenu(pf_IOdisplay displayCallback, pf_IOinput inputCallback, MenuOptions options);
    ushort addMenuitem(const char *menuname, pf_callback menuFonction, ushort parentid);
    // get the string (as a char array) to display the menu for the current hierarchy
    void displayMenu();
};