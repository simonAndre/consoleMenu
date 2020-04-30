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
bool addedlog(ushort menukey, const char *menuname);
bool addedlog2();
bool addedlog3();
const char *switchMenuDisplay(ushort menukey);
bool testIO();
bool displaystr1();
bool SetTimeout();

// variables
char staticString[30];
char tempbuff[50];
int int1 = 1234;
float float1 = 5688.5567;
bool bool1;
int _intvalue;

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
    switchmenu2,
    int1updater
};

const char *dynlabel_intvalue()
{
    snprintf(tempbuff, sizeof(tempbuff), "(current=%i)", _intvalue);
    return (const char *)tempbuff;
}

void SetupMenu()
{
    mymenu = new Menu<21>(); //declare a menu sized for 19 menu-items

    // DisplayInfos: IO callback to render the menu
    // WaitforInput: IO callback to wait and read the user input
    MenuOptions mo;
    mo.addBack = true;
    mo.addExitForEachLevel = true;
    mo.expirationTimeSec = 10;
    mymenu->setOptions(mo);

    // menus & submenus definition
    // root menus
    SubMenu *root = mymenu->getRootMenu(); //get the root menu-item
    // submenu definition - method 1 : from the root menu
    SubMenu *submenu1 = root->addSubMenu("Submenu inputs w callbacks");
    // submenu definition - method 2 : from the global menu instance
    SubMenu *submenu2 = mymenu->addSubMenu("submenu inputs w updaters")->addCallbackToChilds(addedlog3);
    // SubMenu *submenu2 = root->addSubMenu("submenu inputs w updaters")->addCallbackToChilds(addedlog3);
    root->addMenuitemCallback("simple menu and exit", simpleMenu)->addExit(); // simpleMenuis a simple callback without parameter, function is : [bool simpleMenu();]
    // callback with menu name passed as parameter, see function menuParamName and callback chained
    root->addMenuitemCallback("consoleMenu version", getVersionMenu)->addCallback(addedlog)->addCallback(addedlog2);

    root->addMenuitemCallback("test prompted inputs", testIO); // callback with menu name passed as parameter, see function menuParamName
    // level 2 menus, under the item [submenu1]
    submenu1->addMenuitemCallback("set string", initStringValue);
    submenu1->addMenuitemCallback("display string", DisplayStringValue);
    submenu1->addMenuitemCallback("set timeout", SetTimeout);
    submenu1->addMenuitemCallback("set int value", initIntValue);
    submenu1->addMenuitemCallback("display int value", DisplayIntValue);
    // this menu is dynamic : its name is provided by the function [switchMenuDisplay], it can be updated depending of the context
    submenu1->addMenuitemCallback(switchMenuDisplay, (ushort)MyMenuKeys::switchmenu1, switchMenu);

    // more levels can be chained...
    SubMenu *submenu3 = submenu1->addSubMenu("sub menu 2")->addCallbackToChilds(addedlog3);
    submenu3->addMenuitemCallback("build infos", buildInfos); //still a simple menu
    // another dynamic menu bind to the same callbacks with a different key
    submenu3->addMenuitemCallback(switchMenuDisplay, (ushort)MyMenuKeys::switchmenu2, switchMenu);

    submenu2->addMenuitemCallback("display value str1", displaystr1);
    submenu2->addMenuitemUpdater("change str1", (char *)staticString, sizeof(staticString));
    auto miu_changint = submenu2->addMenuitemUpdater("change int1, enter 3 will break the 1st callback", &int1);
    submenu2->addMenuitemUpdater("change float1", &float1);
    submenu2->addMenuitemUpdater("change bool1", &bool1)->addCallback(addedlog)->addCallback(addedlog2);

    submenu2->addMenuitem()->SetLabel("int1=50")->addLambda([]() { _intvalue = 50; })->SetDynLabel(dynlabel_intvalue, true)->addCallback(DisplayIntValue);

    submenu2->addMenuitem()->SetLabel("int1=100")->addLambda([]() { _intvalue = 100; })->addCallback(DisplayIntValue);

    if (miu_changint)
    {
        miu_changint->setMenuKey(MyMenuKeys::int1updater);
        miu_changint->addCallback(addedlog);
        miu_changint->addCallback(addedlog2);
    }

    strcpy(staticString, "first string");
}

/***********   main   ******************/
int main()
{

    cout
        << "\n\n";
    SetupMenu();
    cout << "size menu : " << mymenu->size() << '\n';

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
    return true;
}
bool getVersionMenu()
{
    cout << "current consoleMenu version: " << mymenu->getVersion() << '\n';
    return true;
}

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
    return true;
}
bool DisplayStringValue()
{
    cout << "string content : " << _cstringvalue << '\n';
    return true;
}
bool DisplayIntValue()
{
    cout << "int value : " << _intvalue << '\n';
    return true;
}
bool addedlog(ushort menukey, const char *menuname)
{
    cout << "added log : menu-item " << menuname << " called" << endl;
    if (menukey == MyMenuKeys::int1updater && int1 == 3)
    {
        cout << "3 has been provided, addedlog callback break, addedlog2 will not be called" << endl;
        return false;
    }
    return true;
}

bool addedlog2()
{
    cout << "added log 2" << endl;
    return true;
}
bool addedlog3()
{
    cout << "added log 3" << endl;
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
    return true;
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
    return true;
}

bool displaystr1()
{
    IoHelpers::IOdisplayLn(staticString);
    return true;
}
