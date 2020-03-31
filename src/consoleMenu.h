
#include <map>
#include <string>

typedef bool (*pf_menu)(void);
typedef void (*pf_display)(const char *);
typedef const char *(*pf_input)(void);
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
        externalFunction,
        hierarchymenu,
        exit,
        back
    };

    class Menuitem
    {
    private:
    public:
        Menuitem(const char *menuname, ushort id, ushort parentid, pf_menu menuFonction, consoleMenu::menutype type);
        ushort mid;
        ushort mparentid = 0;
        std::string mname;
        consoleMenu::menutype mtype;
        pf_menu mFonction;
    };

    std::map<ushort, Menuitem> _menuCollection;
    ushort getHierarchyCollectionCardinality(ushort hierarchyid);
    ushort _hierarchyIx = 0; // current hierarchy
    pf_display _displayCallback;
    pf_input _inputCallback;
    MenuOptions _menuoptions;

public:
    consoleMenu(pf_display displayCallback, pf_input inputCallback, MenuOptions options);
    ushort addMenuitem(const char *menuname, pf_menu menuFonction, ushort parentid);
    // get the string (as a char array) to display the menu for the current hierarchy
    void displayMenu();
    // perform the selection for the menu item;
    // return true if successfull (otherwise, bad input, prompt again in the outside loop).
    bool Selection(short menuitemid);
};