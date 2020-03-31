#include <iostream>
#include "consoleMenu.h"
using namespace std;

bool initIntValue(const char *menuname);
bool initStringValue(const char *menuname);
bool DisplayStringValue(const char *menuname);
bool menu1(const char *menuname);
bool menu2(const char *menuname);

/*****  stdio functions ********/
void DisplayInfos(const char *infos)
{
    cout << infos;
}

const char *WaitforInput()
{
    string input;
    cin >> input;
    return input.c_str();
}

/***********   main   ******************/
int main()
{
    cout << "\n\n";

    //define options
    MenuOptions menuoptions;
    menuoptions.addBack = true;
    menuoptions.addExitForEachLevel = true;

    // declaration,
    // DisplayInfos: IO callback to render the menu
    // WaitforInput: IO callback to wait and read the user input
    consoleMenu m(DisplayInfos, WaitforInput, menuoptions);

    // menus & submenus definition
    // root menus
    m.addMenuitem("action 1", menu1, 0);
    m.addMenuitem("action 2", menu2, 0);
    ushort menu3id = m.addMenuitem("sous-menu 3", NULL, 0);
    // level 2 menus, under the item [menu3id]
    m.addMenuitem("init string and stay", initStringValue, menu3id);
    m.addMenuitem("display string and stay", DisplayStringValue, menu3id);
    m.addMenuitem("menu init int and stop", initIntValue, menu3id);

    // more levels can be chained...
    ushort menu33id = m.addMenuitem("sous-menu 3-3", NULL, menu3id);
    m.addMenuitem("menu 3-3-1", menu1, menu33id);
    m.addMenuitem("menu 3-3-2", menu2, menu33id);

    //and display the root menu
    m.displayMenu();

    return 0;
}

/********* menus callbacks ***********/

bool menu1(const char *menuname)
{
    cout << "callback menu1 called from " << menuname << '\n';
    return true;
}
bool menu2(const char *menuname)
{
    cout << "callback menu1 called from " << menuname << '\n';
    return true;
}

int _intvalue;
bool initIntValue(const char *menuname)
{
    cout << "enter an int value:";
    try
    {
        cin >> _intvalue;
    }
    catch (const std::exception &e)
    {
        std::cerr << e.what() << '\n';
        return false;
    }
    return true;
}
string _cstringvalue("-");
bool initStringValue(const char *menuname)
{
    cout << "enter an string value:";
    try
    {
        cin >> _cstringvalue;
    }
    catch (const std::exception &e)
    {
        std::cerr << e.what() << '\n';
    }
    return false;
}
bool DisplayStringValue(const char *menuname)
{
    cout << "string content : " << _cstringvalue << '\n';
    return false;
}