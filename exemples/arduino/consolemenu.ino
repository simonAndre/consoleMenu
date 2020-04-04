#include <consoleMenu.h>
#include <Arduino.h>

//prototypes
void CheckSerial();
void setupSerialMenu();
bool initIntValue(const char *menuname);
bool initStringValue(const char *menuname);
bool DisplayStringValue(const char *menuname);
bool simpleMenu();
bool buildInfos();
bool menuParamName(const char *menuname);
bool switchMenu(ushort menukey, const char *menuname);
const char *switchMenuDisplay(ushort menukey);
String takeUserInput();

//variables
// Menu consolemenu = Menu(DisplayInfos, WaitforInput);
Menu consolemenu = Menu();

enum MyMenuKeys
{
    switchmenu1,
    switchmenu2
};

void setup()
{
    Serial.begin(115200);
    setupSerialMenu();
}

void loop()
{
    // CheckSerial();
    consolemenu.LoopCheckSerial();
    delay(50);
}

void setupSerialMenu()
{
    //define options
    MenuOptions menuoptions;
    menuoptions.addBack = true;
    menuoptions.addExitForEachLevel = true;
    consolemenu.setOptions(menuoptions);
    // menus & submenus definition
    // root menus
    consolemenu.addCallbackMenuitem("simple menu, no params", simpleMenu, 0); // simple callback without parameter, see function simpleMenu
    consolemenu.addCallbackMenuitem("action 2", menuParamName, 0);            // callback with menu name passed as parameter, see function menuParamName
    ushort submenu1id = consolemenu.addHierarchyMenuitem("Sub menu 1", 0);
    // level 2 menus, under the item [submenu1id]
    consolemenu.addCallbackMenuitem("init string and stay", initStringValue, submenu1id);
    consolemenu.addCallbackMenuitem("display string and stay", DisplayStringValue, submenu1id);
    consolemenu.addCallbackMenuitem("menu init int and stop", initIntValue, submenu1id);
    //this menu is dynamic : its name is provided by the function [switchMenuDisplay], it can be updated depending of the context
    consolemenu.addDynamicCallbackMenuitem(switchMenuDisplay, switchMenu, submenu1id, (ushort)MyMenuKeys::switchmenu1);

    // more levels can be chained...
    ushort submenu2id = consolemenu.addHierarchyMenuitem("sub menu 2", submenu1id);
    consolemenu.addCallbackMenuitem("build infos", buildInfos, submenu2id); //still a simple menu
    //another dynamic menu bind to the same callbacks with a different key
    consolemenu.addDynamicCallbackMenuitem(switchMenuDisplay, switchMenu, submenu2id, (ushort)MyMenuKeys::switchmenu2);
}

/********* menus callbacks ***********/
bool simpleMenu()
{
    Serial.println("callback menu1 called, exit.");
    return true;
}
bool buildInfos()
{
    Serial.print("__GNUG__:");
    Serial.println(__GNUG__);
    Serial.print("__cplusplus:");
    Serial.println(__cplusplus);
    Serial.print("build time :");
    Serial.println(__TIMESTAMP__);
    return false;
}
bool menuParamName(const char *menuname)
{
    Serial.print("callback menu called with the menu display name as param: ");
    Serial.println(menuname);
    return false;
}

int _intvalue;
bool initIntValue(const char *menuname)
{
    Serial.print("enter an int value:");
    String userinput = takeUserInput();
    try
    {
        _intvalue = atoi(userinput.c_str());
    }
    catch (const std::exception &e)
    {
        Serial.print("bad input, try again");
        return false;
    }
    return true;
}

String _stringvalue("-");
bool initStringValue(const char *menuname)
{
    Serial.print("enter an string value:");
    _stringvalue = takeUserInput();
    return false;
}

bool DisplayStringValue(const char *menuname)
{
    Serial.print("string content : ");
    Serial.println(_stringvalue);
    return false;
}

bool _switchmenuValue1 = false;
bool _switchmenuValue2 = false;
bool switchMenu(ushort menukey, const char *menuname)
{
    Serial.print("Menu ");
    Serial.print(menuname);
    Serial.print(" called, now value is : ");
    switch ((MyMenuKeys)menukey)
    {
    case MyMenuKeys::switchmenu1:
        _switchmenuValue1 = !_switchmenuValue1;
        Serial.println(_switchmenuValue1);
        break;
    case MyMenuKeys::switchmenu2:
        _switchmenuValue2 = !_switchmenuValue2;
        Serial.println(_switchmenuValue2);
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

String takeUserInput()
{
    bool inputdone = false;
    do
    {
        if (Serial.available() > 0)
        {
            String provinput = Serial.readString();
            return provinput;
        }
        delay(50);
    } while (!inputdone);
}