#include <iostream>
#include <consoleMenu.h>
using namespace std;

//prototypes
bool initIntValue(const char *menuname);
bool initStringValue(const char *menuname);
bool DisplayStringValue();
bool DisplayIntValue();
bool simpleMenu();
bool buildInfos();
bool menuParamName(const char *menuname);
bool switchMenu(ushort menukey, const char *menuname);
const char *switchMenuDisplay(ushort menukey);
bool testIO();

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
ushort WaitforInput2()
{
    ushort input;
    cin >> input;
    return input;
}

Menu m = Menu();
// Menu m = Menu(DisplayInfos, WaitforInput);

enum MyMenuKeys
{
    switchmenu1,
    switchmenu2
};

void SetupMenu()
{
    // declaration,
    // DisplayInfos: IO callback to render the menu
    // WaitforInput: IO callback to wait and read the user input
    // menus & submenus definition
    // root menus
    MenuOptions mo;
    mo.addBack = true;
    mo.addExitForEachLevel = true;
    m.setOptions(mo);

    m.addCallbackMenuitem("simple menu, no params", simpleMenu, 0); // simple callback without parameter, see function simpleMenu
    m.addCallbackMenuitem("action 2", menuParamName, 0);            // callback with menu name passed as parameter, see function menuParamName
    m.addCallbackMenuitem("test inputs", testIO, 0);                // callback with menu name passed as parameter, see function menuParamName
    ushort submenu1id = m.addHierarchyMenuitem("Sub menu 1", 0);
    // level 2 menus, under the item [submenu1id]
    m.addCallbackMenuitem("set string", initStringValue, submenu1id);
    m.addCallbackMenuitem("display string and stay", DisplayStringValue, submenu1id);
    m.addCallbackMenuitem("set int value", initIntValue, submenu1id);
    m.addCallbackMenuitem("display int value and stop", DisplayIntValue, submenu1id);
    //this menu is dynamic : its name is provided by the function [switchMenuDisplay], it can be updated depending of the context
    m.addDynamicCallbackMenuitem(switchMenuDisplay, switchMenu, submenu1id, (ushort)MyMenuKeys::switchmenu1);

    // more levels can be chained...
    ushort submenu2id = m.addHierarchyMenuitem("sub menu 2", submenu1id);
    m.addCallbackMenuitem("build infos", buildInfos, submenu2id); //still a simple menu
    //another dynamic menu bind to the same callbacks with a different key
    m.addDynamicCallbackMenuitem(switchMenuDisplay, switchMenu, submenu2id, (ushort)MyMenuKeys::switchmenu2);
}

/***********   main   ******************/
int main()
{
    cout << "\n\n";
    SetupMenu();

    // display the root menu
    m.displayMenu();

    return 0;
}

/********* menus callbacks ***********/
bool simpleMenu()
{
    cout << "__GNUG__ : " << __GNUG__ << '\n';
    cout << "__cplusplus : " << __cplusplus << '\n';
    cout << "build time : " << __TIMESTAMP__ << '\n';
    cout
        << "callback menu1 called, exit." << '\n';
    return true;
}
bool buildInfos()
{
    cout << "__GNUG__ : " << __GNUG__ << '\n';
    cout << "__cplusplus : " << __cplusplus << '\n';
    cout << "build time : " << __TIMESTAMP__ << '\n';
    cout << "come back to the menu" << '\n';
    return false;
}
bool menuParamName(const char *menuname)
{
    cout << "callback menu called with the menu display name as param: " << menuname << '\n';
    return false;
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
    return false;
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
bool DisplayStringValue()
{
    cout << "string content : " << _cstringvalue << '\n';
    return false;
}
bool DisplayIntValue()
{
    cout << "int value : " << _intvalue << '\n';
    return true;
}
bool _switchmenuValue1 = false;
bool _switchmenuValue2 = false;
bool switchMenu(ushort menukey, const char *menuname)
{
    cout << "Menu " << menuname << " called, now value is : ";
    switch ((MyMenuKeys)menukey)
    {
    case MyMenuKeys::switchmenu1:
        _switchmenuValue1 = !_switchmenuValue1;
        cout << _switchmenuValue1 << '\n';
        break;
    case MyMenuKeys::switchmenu2:
        _switchmenuValue2 = !_switchmenuValue2;
        cout << _switchmenuValue2 << '\n';
        break;
    default:
        throw std::runtime_error("menu key not implemented is switchMenu");
    }
    return false;
}
char buff[50];

const char *switchMenuDisplay(ushort menukey)
{
    switch ((MyMenuKeys)menukey)
    {
    case MyMenuKeys::switchmenu1:
        sprintf(buff, "switch value is %i", _switchmenuValue1);
        break;
    case MyMenuKeys::switchmenu2:
        sprintf(buff, "switch value is %i", _switchmenuValue2);
        break;
    default:
        sprintf(buff, "menukey %i not managed", menukey);
        break;
    }

    return buff;
}

bool testIO()
{
    char inputstr[5];
    IoHelpers::TakeUserInput_s("input a string (less than 5 digits)>", inputstr, sizeof(inputstr));
    IoHelpers::IOdisplay("your entered:");
    IoHelpers::IOdisplayLn(inputstr);

    int i;
    if (IoHelpers::TakeUserInput_i("input an int>", &i, 2))
    {
        IoHelpers::IOdisplay("your entered:");
        IoHelpers::IOdisplayLn(i);
    }

    double f;
    if (IoHelpers::TakeUserInput_f("input a decimal >", &f, 2))
    {
        IoHelpers::IOdisplay("your entered:");
        IoHelpers::IOdisplayLn(f);
    }
    return false;
}