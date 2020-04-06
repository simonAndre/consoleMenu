#include <consoleMenu.h>
#include <Arduino.h>

//prototypes
void CheckSerial();
void setupSerialMenu();
bool initIntValue(const char *menuname);
bool initStringValue(const char *menuname);
bool DisplayStringValue(const char *menuname);
bool DisplayIntValue();
bool simpleMenu();
bool buildInfos();
bool menuParamName(const char *menuname);
bool switchMenu(ushort menukey, const char *menuname);
const char *switchMenuDisplay(ushort menukey);
String ArduinoTakeUserInput_s(const char *promptmessage);
int ArduinoTakeUserInput_i(const char *promptmessage);
bool displaystr1();
bool displayint1();
bool displaybool1();

// variables
char staticString[30];
int int1 = 1234;
bool bool1;
int _intvalue;
String _stringvalue("-");
bool _switchmenuValue1 = false;
bool _switchmenuValue2 = false;
char buff[50];

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
    consolemenu.addCallbackMenuitem("simple menu, no params", simpleMenu, 0);    // simple callback without parameter, see function simpleMenu
    consolemenu.addCallbackMenuitem("action menu with param", menuParamName, 0); // callback with menu name passed as parameter, see function menuParamName
    consolemenu.addCallbackMenuitem("test prompted inputs", testIO, 0);          // callback with menu name passed as parameter, see function menuParamName
    ushort testinputsid = consolemenu.addHierarchyMenuitem("submenu test inputs", 0);
    ushort submenu1id = consolemenu.addHierarchyMenuitem("Sub menu 1", 0);
    // level 2 menus, under the item [submenu1id]
    consolemenu.addCallbackMenuitem("set string", initStringValue, submenu1id);
    consolemenu.addCallbackMenuitem("display string and stay", DisplayStringValue, submenu1id);
    consolemenu.addCallbackMenuitem("set int value", initIntValue, submenu1id);
    consolemenu.addCallbackMenuitem("display int value and stop", DisplayIntValue, submenu1id);
    //this menu is dynamic : its name is provided by the function [switchMenuDisplay], it can be updated depending of the context
    consolemenu.addDynamicCallbackMenuitem(switchMenuDisplay, switchMenu, submenu1id, (ushort)MyMenuKeys::switchmenu1);

    // more levels can be chained...
    ushort submenu2id = consolemenu.addHierarchyMenuitem("sub menu 2", submenu1id);
    consolemenu.addCallbackMenuitem("build infos", buildInfos, submenu2id); //still a simple menu
    //another dynamic menu bind to the same callbacks with a different key
    consolemenu.addDynamicCallbackMenuitem(switchMenuDisplay, switchMenu, submenu2id, (ushort)MyMenuKeys::switchmenu2);

    consolemenu.addCallbackMenuitem("display value str1", displaystr1, testinputsid);
    consolemenu.addUpdaterMenuitem("change str1", testinputsid, (char *)staticString, sizeof(staticString), 1);
    consolemenu.addCallbackMenuitem("display value int1", displayint1, testinputsid);
    consolemenu.addUpdaterMenuitem("change int1", testinputsid, &int1, 2);
    consolemenu.addCallbackMenuitem("display value bool1", displaybool1, testinputsid);
    consolemenu.addUpdaterMenuitem("change bool1", testinputsid, &bool1, 2);
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

bool initIntValue(const char *menuname)
{
    _intvalue = ArduinoTakeUserInput_i("enter an int value:");
    return false;
}

bool initStringValue(const char *menuname)
{
    _stringvalue = ArduinoTakeUserInput_s("enter an string value:");
    return false;
}

bool DisplayStringValue(const char *menuname)
{
    Serial.print("string content : ");
    Serial.println(_stringvalue);
    return false;
}
bool DisplayIntValue()
{
    Serial.print("int value : ");
    Serial.println(_intvalue);
    return true;
}

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

String ArduinoTakeUserInput_s(const char *promptmessage)
{
    Serial.print(promptmessage);
    Serial.print(">");
    bool inputdone = false;
    do
    {
        delay(20);
        if (Serial.available() > 0)
        {
            String provinput = Serial.readString();
            Serial.println();
            return provinput;
        }
    } while (!inputdone);
}

int ArduinoTakeUserInput_i(const char *promptmessage)
{
    String userinput = ArduinoTakeUserInput_s(promptmessage);
    try
    {
        return atoi(userinput.c_str());
    }
    catch (const std::exception &e)
    {
        Serial.print("bad input, try again");
        throw e;
    }
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
bool displayint1()
{
    IoHelpers::IOdisplayLn(int1);
    return false;
}
bool displaybool1()
{
    IoHelpers::IOdisplayLn(bool1);
    return false;
}