#include <iostream>
#include "consoleMenu.h"
using namespace std;

class MemberMenusTester
{
private:
    int _intvalue;
    string _cstringvalue;

public:
    MemberMenusTester(/* args */);
    bool initIntValue();
    bool initCstringValue();
};

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

int main()
{
    cout << "starting test consoleMenu\n";
    MenuOptions menuoptions;
    menuoptions.addBack = true;
    menuoptions.addBack = false;
    consoleMenu m(DisplayInfos, WaitInput, menuoptions);
    ushort menu1id = m.addMenuitem("menu 1", menu1, 0);
    ushort menu2id = m.addMenuitem("menu 2", menu2, 0);
    ushort menu3id = m.addMenuitem("menu 3", NULL, 0);
    m.addMenuitem("menu 3-1", menu1, menu3id);
    m.addMenuitem("menu 3-2", menu2, menu3id);
    ushort menu33id = m.addMenuitem("menu 3-3", NULL, menu3id);
    m.addMenuitem("menu 3-3-1", menu1, menu33id);
    m.addMenuitem("menu 3-3-2", menu2, menu33id);

    m.displayMenu();

    return 0;
}

bool MemberMenusTester::initIntValue()
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
bool MemberMenusTester::initCstringValue()
{
    cout << "enter an string value:";
    try
    {
        cin >> _cstringvalue;
    }
    catch (const std::exception &e)
    {
        std::cerr << e.what() << '\n';
        return false;
    }
    return true;
}