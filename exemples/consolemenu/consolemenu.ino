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
bool getVersionMenu(const char *menuname);
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

Menubase *_consolemenu;

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
    if(_consolemenu->LoopCheckSerial())
        Serial.Print("we're out of the menu");
    delay(50);
}

void setupSerialMenu()
{
    _consolemenu = new Menu<20>();
    //define options
    MenuOptions menuoptions;
    menuoptions.addBack = true;
    menuoptions.addExitForEachLevel = true;
    _consolemenu->setOptions(menuoptions);
    // menus & submenus definition
    // root menus
    SubMenu *root = _consolemenu->getRootMenu();
    SubMenu *menu_testinput = root->addSubMenu("submenu test inputs");
    SubMenu *menu_submenu1 = root->addSubMenu("Sub menu 1");
    // more levels can be chained...
    SubMenu *menu_submenu2 = menu_submenu1->addSubMenu("Sub menu 2");

    root->addMenuitemCallback("simple menu, no params", simpleMenu);     // simple callback without parameter, see function simpleMenu
    root->addMenuitemCallback("action menu with param", getVersionMenu); // callback with menu name passed as parameter, see function menuParamName
    root->addMenuitemCallback("test prompted inputs", testIO);           // callback with menu name passed as parameter, see function menuParamName

    // level 2 menus, under the item [submenu1id]
    menu_submenu1->addMenuitemCallback("set string", initStringValue);
    menu_submenu1->addMenuitemCallback("display string and stay", DisplayStringValue);
    menu_submenu1->addMenuitemCallback("set int value", initIntValue);
    menu_submenu1->addMenuitemCallback("display int value and stop", DisplayIntValue);
    //this menu is dynamic : its name is provided by the function [switchMenuDisplay], it can be updated depending of the context
    menu_submenu1->addMenuitemCallback(switchMenuDisplay, (ushort)MyMenuKeys::switchmenu1, switchMenu);

    menu_submenu2->addMenuitemCallback("build infos", buildInfos);
    //another dynamic menu bind to the same callbacks with a different key
    menu_submenu2->addMenuitemCallback(switchMenuDisplay, (ushort)MyMenuKeys::switchmenu2, switchMenu);

    menu_testinput->addMenuitemCallback("display value str1", displaystr1);
    menu_testinput->addMenuitemUpdater("change str1", (char *)staticString, sizeof(staticString));
    menu_testinput->addMenuitemCallback("display value int1", displayint1);
    menu_testinput->addMenuitemUpdater("change int1", &int1);
    menu_testinput->addMenuitemCallback("display value bool1", displaybool1);
    menu_testinput->addMenuitemUpdater("change bool1", &bool1);
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

bool getVersionMenu()
{
    Serial.print("current consoleMenu version: ");
    char *version = Menubase::getVersion();
    Serial.println(version);
    return false;
}

bool initIntValue()
{
    _intvalue = ArduinoTakeUserInput_i("enter an int value:");
    return false;
}

bool initStringValue()
{
    _stringvalue = ArduinoTakeUserInput_s("enter an string value:");
    return false;
}

bool DisplayStringValue()
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