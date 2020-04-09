#include "catch/catch.hpp"
#include <iostream>
#include <consoleMenu.h>
#include <typeinfo>

// exemple of a method controlling the IO input (provided by default by the library)
std::string mdisplay("");
void DisplayInfos(const char *infos)
{
    mdisplay.append(infos);
}
ushort WaitforInput()
{
    return 0;
}

SCENARIO("menu set ups & options", "[setup]")
{
    GIVEN("a menu has been declared")
    {
        Menu m = Menu(DisplayInfos, WaitforInput);
        WHEN("on the first initialisation with default options")
        {
            THEN("it contains one item, wich is the bak item")
            {
                REQUIRE(m.size() == 1);
                REQUIRE(m.getById(1)->mname == CONSOLEMENU_MENU_BACK);
            }
            THEN("it display only the exit item")
            {
                mdisplay = "";
                m.displayMenu(0);
                REQUIRE(mdisplay == CONSOLEMENU_MENU_EXIT);
            }
        }
    }
}