#include <iostream>
#include <array>
#include <consoleMenu.h>
#include <typeinfo>
using namespace std;

//prototypes
bool initIntValue();
bool initStringValue();
bool DisplayStringValue();
bool DisplayIntValue();
bool simpleMenu();
bool buildInfos();
bool getVersionMenu();
bool switchMenu(ushort menukey, const char *menuname);
const char *switchMenuDisplay(ushort menukey);
bool testIO();
bool displaystr1();
bool SetTimeout();

// variables
char staticString[30];
int int1 = 1234;
float float1 = 5688.5567;
bool bool1;

Menubase *mymenu;

/*****  stdio functions ********/
void DisplayInfos(const char *infos)
{
    cout << infos;
}

// exemple of a method controlling the IO input (provided by default by the library)
ushort WaitforInput()
{
    ushort input;
    cin >> input;
    return input;
}

// next init form to use custom methods controlling IO display and input
// Menu m = Menu(DisplayInfos, WaitforInput);

enum MyMenuKeys
{
    switchmenu1,
    switchmenu2
};

void SetupMenu()
{
    mymenu = new Menu<19>();
    // declaration,
    // DisplayInfos: IO callback to render the menu
    // WaitforInput: IO callback to wait and read the user input
    // menus & submenus definition
    // root menus
    MenuOptions mo;
    mo.addBack = true;
    mo.addExitForEachLevel = true;
    mo.expirationTimeSec = 10;
    mymenu->setOptions(mo);

    MenuitemHierarchy* root = mymenu->getRootMenu();
    MenuitemHierarchy *submenu1 = root->addMenuitemHierarchy("Submenu inputs w callbacks");
    MenuitemHierarchy *submenu2 = root->addMenuitemHierarchy("submenu inputs w updaters");
    root->addMenuitemCallback("simple menu and exit", simpleMenu);  // simple callback without parameter, see function simpleMenu
    root->addMenuitemCallback("consoleMenu version", getVersionMenu); // callback with menu name passed as parameter, see function menuParamName

    root->addMenuitemCallback("test prompted inputs", testIO); // callback with menu name passed as parameter, see function menuParamName
    // // level 2 menus, under the item [submenu1]
    submenu1->addMenuitemCallback("set string", initStringValue);
    submenu1->addMenuitemCallback("display string and stay", DisplayStringValue);
    submenu1->addMenuitemCallback("set timeout", SetTimeout);
    submenu1->addMenuitemCallback("set int value", initIntValue);
    submenu1->addMenuitemCallback("display int value and stop", DisplayIntValue);
    // //this menu is dynamic : its name is provided by the function [switchMenuDisplay], it can be updated depending of the context
    submenu1->addMenuitemCallback(switchMenuDisplay, (ushort)MyMenuKeys::switchmenu1, switchMenu);

    // // more levels can be chained...
    MenuitemHierarchy *submenu3 = submenu1->addMenuitemHierarchy("sub menu 2");
    submenu3->addMenuitemCallback("build infos", buildInfos); //still a simple menu
    // //another dynamic menu bind to the same callbacks with a different key
    submenu3->addMenuitemCallback(switchMenuDisplay, (ushort)MyMenuKeys::switchmenu2, switchMenu);

    submenu2->addMenuitemCallback("display value str1", displaystr1);
    submenu2->addMenuitemUpdater("change str1", (char *)staticString, sizeof(staticString));
    submenu2->addMenuitemUpdater("change int1", &int1);
    submenu2->addMenuitemUpdater("change float1", &float1);
    submenu2->addMenuitemUpdater("change bool1", &bool1);

    strcpy(staticString, "first string");
}

/***********   main   ******************/
int main()
{
    // IoHelpers::IOdisplayG<const char *>("popo");
    // IoHelpers::IOdisplayG<int>(54);
    // auto _displayCallback = (consoleMenu033::fp_IOdisplay)IoHelpers::IOdisplayG<const char *>;
    // const char *cs = "coucou";
    // _displayCallback(cs);
    // _displayCallback("mlkmkl\n");

    // std::array<Menuitem, 20> itemarray;
    // Menuitem newmi(&m, "testaddroot", 1, NULL, menutype::rootmenu);

    // itemarray[3] = newmi;
    // std::cout
    //     << "size of Menuitem: " << sizeof(Menuitem) << '\n';
    // std::cout << "size of itemarray: " << itemarray.size() << '\n';
    // std::cout << "max_size of itemarray: " << itemarray.max_size() << '\n';

    cout
        << "\n\n";
    SetupMenu();
    cout << "size menu : " << mymenu->size() << '\n';

    // for (size_t i = 1; i < 10; i++)
    // {
    //     cout << i << " -> " << m.getById(i)->mname << '\n';
    // }

    // m.displayMenu(0);

    // display the root menu
    mymenu->launchMenu();

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
bool getVersionMenu()
{
    cout << "current consoleMenu version: " << mymenu->getVersion() << '\n';
    return false;
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
    return false;
}
int _timeoutvalue;
bool SetTimeout()
{
    cout << "enter new timeout for the menu:";
    try
    {
        cin >> _timeoutvalue;
        MenuOptions op = mymenu->getOptions();
        op.expirationTimeSec = _timeoutvalue;
        mymenu->setOptions(op);
    }
    catch (const std::exception &e)
    {
        std::cerr << e.what() << '\n';
        return false;
    }
    return false;
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
    IoHelpers::TakeUserInput("input a string (less than 5 digits)>", inputstr, sizeof(inputstr), 2);
    IoHelpers::IOdisplay("your entered:");
    IoHelpers::IOdisplayLn(inputstr);

    int i;
    if (IoHelpers::TakeUserInput("input an int>", &i, 2))
    {
        IoHelpers::IOdisplay("your entered:");
        IoHelpers::IOdisplayLn(i);
    }

    double f;
    if (IoHelpers::TakeUserInput("input a decimal >", &f, 2))
    {
        IoHelpers::IOdisplay("your entered:");
        IoHelpers::IOdisplayLn(f);
    }
    return false;
}

bool displaystr1()
{
    IoHelpers::IOdisplayLn(staticString);
    return false;
}
