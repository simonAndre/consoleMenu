#include <consoleMenu.h>
#include <Arduino.h>

//prototypes
void CheckSerial();
void setupSerialMenu();
void DisplayInfos(const char *infos);
ushort WaitforInput();
//variables
// Menu consolemenu = Menu(DisplayInfos, WaitforInput);
Menu consolemenu = Menu();

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

bool menu1(const char *menuname)
{
    Serial.print("__GNUG__:");
    Serial.println(__GNUG__);
    Serial.print("__cplusplus:");
    Serial.println(__cplusplus);
    Serial.print("build time :");
    Serial.println(__TIMESTAMP__);
    return false;
}

bool menu2(const char *menuname)
{
    Serial.print("callback menu1 called from");
    Serial.println(menuname);
    return true;
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
    consolemenu.addMenuitem("build infos", menu1, 0);
    consolemenu.addMenuitem("action 2", menu2, 0);
    ushort menu3id = consolemenu.addMenuitem("sous-menu 3", NULL, 0);
    // level 2 menus, under the item [menu3id]
    consolemenu.addMenuitem("init string and stay", menu2, menu3id);
    consolemenu.addMenuitem("display string and stay", menu2, menu3id);
}

/*****  stdio functions for serial console menu ********/
void DisplayInfos(const char *infos)
{
    Serial.print(infos);
}

ushort WaitforInput()
{
    bool inputdone = false;
    do
    {
        if (Serial.available() > 0)
        {
            String provinput = Serial.readString();
            char c = provinput.charAt(0);
            if (c > 10)
                return c - 48;
            return c;
        }
        delay(50);
    } while (!inputdone);
}
