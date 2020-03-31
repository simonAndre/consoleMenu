#include <iostream>
#include "consoleMenu.h"
using namespace std;

bool initIntValue();
bool initStringValue();
bool DisplayStringValue();
bool menu1();
bool menu2();

/*****  stdio functions ********/
void DisplayInfos(const char *infos)
{
    cout << infos;
}

const char *WaitInput()
{
    string input;
    cin >> input;
    return input.c_str();
}

/***********   main   ******************/
int main()
{
    cout << "starting test consoleMenu\n";

    MenuOptions menuoptions;
    menuoptions.addBack = true;
    menuoptions.addExitForEachLevel = true;
    consoleMenu m(DisplayInfos, WaitInput, menuoptions);
    m.addMenuitem("menu 1", menu1, 0);
    m.addMenuitem("menu 2", menu2, 0);
    ushort menu3id = m.addMenuitem("sous-menu 3", NULL, 0);
    m.addMenuitem("init string and stay", initStringValue, menu3id);
    m.addMenuitem("display string and stay", DisplayStringValue, menu3id);
    m.addMenuitem("menu init int and stop", initIntValue, menu3id);

    ushort menu33id = m.addMenuitem("sous-menu 3-3", NULL, menu3id);
    m.addMenuitem("menu 3-3-1", menu1, menu33id);
    m.addMenuitem("menu 3-3-2", menu2, menu33id);

    m.displayMenu();

    return 0;
}

/********* menus callbacks ***********/

bool menu1()
{
    cout << "menu1 called\n";
    return true;
}
bool menu2()
{
    cout << "menu2 called\n";
    return true;
}

int _intvalue;
bool initIntValue()
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
bool initStringValue()
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
bool DisplayStringValue()
{
    cout << "string content : " << _cstringvalue << '\n';
    return false;
}