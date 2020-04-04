#pragma once
#include "consoleMenu/Namespace.hpp"

namespace CONSOLEMENU_NAMESPACE
{
/**
 * @brief provides default callbacks for targets like pure computer or arduino device.
 * these callbacks can also be provided by the high level implementation 
 * 
 */
class IOcallbacks
{

private:
public:
    IOcallbacks()
    {
    }
    ~IOcallbacks() {}

    static void displayInfosDefaultCallback(const char *infos)
    {
#if CONSOLEMENU_EMBEDDED_MODE
        Serial.print(infos);
#else
        std::cout << infos;
#endif
    }

    const char *WaitforInput()
    {
#if CONSOLEMENU_EMBEDDED_MODE
// used waitforInputIntDefaultCallback instead
#else
        std::string input;
        std::cin >> input;
        return input.c_str();
#endif
    }
    static ushort waitforInputIntDefaultCallback()
    {
#if CONSOLEMENU_EMBEDDED_MODE
        bool inputdone = false;
        do
        {
            if (Serial.available() > 0)
            {
                String provinput = Serial.readString();
                char c = provinput.charAt(0);
                return c - 48;
            }
            delay(50);
        } while (!inputdone);
#else
        ushort input;
        std::cin >> input;
        return input;
#endif
    }
};
} // namespace CONSOLEMENU_NAMESPACE
