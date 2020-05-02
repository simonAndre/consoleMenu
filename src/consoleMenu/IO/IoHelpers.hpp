#include <string.h>
#include <iostream>
#include "../commontypes.h"
#if defined(ARDUINO)
#include "Arduino.h"
#endif

namespace CONSOLEMENU_NAMESPACE
{
typedef bool (*fp_tester)(char);

/**
 * @brief provides default callbacks for targets like pure computer or arduino device.
 * these callbacks can also be provided by the high level implementation 
 * 
 */
class IoHelpers
{

private:
    /**
 * @brief prompt a message, wait for an input string and test if this string conforms to the tester method. 
 * 
 * @param promptmessage : message prompt to display
 * @param tester : callback to test every char in the string
 * @param outstr : the input string to cast back to something or a null pointer of it fails and expired the given trials
 * @param stringbuffersize : size of the string buffer
 * @param trials : max number of trials 
 * @return true : good input
 * @return false : input failure, expiration of the given trials, don't use the outstr pointer.
 */
    static bool _takeUserInputPrim(const char *promptmessage, fp_tester tester, char *outstr, size_t stringbuffersize, ushort trials, ushort timeout_sec = 60)
    {
        ushort i = 0;
        do
        {
            if (!TakeUserInput(promptmessage, outstr, stringbuffersize, 1, timeout_sec))
            {
                i++;
                continue;
            }
            size_t p = 0, siz = strlen(outstr);
            do
            {
                char c = outstr[p];
                if (!tester(c))
                {
                    if (trials > 1)
                        IOdisplayLn("bad input, try again..");
                    i++;
                    break;
                }
                if (p == siz - 1)
                {
                    return true;
                }
                p++;
            } while (p < siz);
        } while (i < trials);
        if (trials > 1)
            IOdisplayLn("Input failed, trials exceeded.");
        return false;
    }

public:
    IoHelpers()
    {
    }
    ~IoHelpers() {}

    /**
 * @brief diplay a message to the user on the available console (use Serial for embedded devices, std::cout for computers)
 * 
 * @tparam T 
 * @param infos 
 */
    template <typename T>
    static void IOdisplay(T infos)
    {
#if CONSOLEMENU_EMBEDDED_MODE
        Serial.print(infos);
#else
        std::cout << infos;
#endif
    }

    static void IOdisplay(bool bvalue)
    {
#if CONSOLEMENU_EMBEDDED_MODE
        if (bvalue)
            Serial.print(CONSOLEMENU_DISPLAYFORBOOLVALUE_TRUE);
        else
            Serial.print(CONSOLEMENU_DISPLAYFORBOOLVALUE_FALSE);
#else
        if (bvalue)
            std::cout << CONSOLEMENU_DISPLAYFORBOOLVALUE_TRUE;
        else
            std::cout << CONSOLEMENU_DISPLAYFORBOOLVALUE_FALSE;
#endif
    }

    /**
 * @brief diplay a message followed by a carriage return to the user on the available console (use Serial for embedded devices, std::cout for computers)
 * 
 * @tparam T 
 * @param infos 
 */
    template <typename T>
    static void IOdisplayLn(const T &infos)
    {
#if CONSOLEMENU_EMBEDDED_MODE
        Serial.println(infos);
#else
        std::cout << infos << '\n';
#endif
    }

    /**
 * @brief Wait for a user input given from the available console (use Serial for embedded devices, std::cin for computers)
 * 
 * @param outstring : user input 
 * @param stringbuffersize : buffer size
 * @param timeout_sec : expiration time (currently, only for embedded devices)
 * @return true 
 * @return false 
 */
    static bool WaitforInput(char *outstring, size_t stringbuffersize, ushort timeout_sec)
    {
        const char *proviput;
        size_t proviputsize;
#if CONSOLEMENU_EMBEDDED_MODE
        bool inputdone = false;
        Serial.flush();
        uint32_t wait_ms = 0;
        do
        {
            delay(10);
            wait_ms += 10;
            if (Serial.available() > 0)
            {
                wait_ms = 0;
                String as_provinput = Serial.readStringUntil('\n');
                proviputsize = as_provinput.length();
                // proviput = as_provinput.c_str();
                // inputdone = true;
                if (proviputsize < stringbuffersize)
                {
                    strncpy(outstring, as_provinput.c_str(), proviputsize);
                    outstring[proviputsize] = '\0';
                    return true;
                }
                else
                {
                    return false;
                }
            }
        } while (!inputdone && wait_ms < (timeout_sec * 1000));
        if (!inputdone)
            throw std::runtime_error(CONSOLEMENU_MENU_MENUSELECTIONTIMEOUTMessage);
#else
        // TODO : handle expiration

        // std::cout << "timeoutvalue=" << timeout_sec << "\n";

        if (timeout_sec > 0)
        {
            std::string input;
            std::cin >> input;
            proviputsize = input.length();
            proviput = input.c_str();
        }

#endif

        if (proviputsize < stringbuffersize)
        {
            strncpy(outstring, proviput, proviputsize);
            outstring[proviputsize] = '\0';
            return true;
        }
        else
        {
            return false;
        }
    }

    /**
 * @brief prompt a message, wait for an input string
 * 
 * @param promptmessage : message prompt to display
 * @param outstring : out input string  
 * @param stringbuffersize : size of the string buffer
 * @return true : good input, 
 * @return false : bad input : oversize input string 
 */
    static bool TakeUserInput(const char *promptmessage, char *outstring, size_t stringbuffersize, ushort trials, ushort timeout_sec = 60)
    {
        IOdisplay(promptmessage);
        IOdisplay(">");
        ushort t = 0;

        while (!WaitforInput(outstring, stringbuffersize, timeout_sec) && t++ < trials)
        {
            IOdisplayLn(CONSOLEMENU_MENU_OVERSIZEINPUT);
        }
        if (t < trials)
            return true;

        return false;
    }

    static bool testint(char c)
    {
        return (c >= 48 && c <= 57);
    }
    static bool testbool(char c)
    {
        return (c == 48 || c == 49 || c == 110 || c == 121); // 0 or 1 or y or n
    }
    static bool testdecimal(char c)
    {
        return ((c >= 48 && c <= 57) || c == 46 || c == 44);
    }

    /**
 * @brief wait for an integer input. 
 * 
 * @param promptmessage : message prompt to display
 * @param outnumber : pointer to the input variable
 * @param trials : max number of trials 
 * @return true : good input
 * @return false : input failure, expiration of the given trials, don't use the outnumber pointer.
 */
    static bool TakeUserInput(const char *promptmessage, int *outnumber, ushort trials, ushort timeout_sec = 60)
    {
        const ushort int_nbdigitmax = 10;
        char userstring[int_nbdigitmax + 1];
        if (_takeUserInputPrim(promptmessage, testint, userstring, int_nbdigitmax, trials, timeout_sec))
        {
            *outnumber = atoi(userstring);
            return true;
        }
        else
            return false;
    }
    static bool TakeUserInput(const char *promptmessage, ushort *outnumber, ushort trials, ushort timeout_sec = 60)
    {
        int i;
        if (TakeUserInput(promptmessage, &i, trials, timeout_sec))
        {
            if (i >= 65535)
                return false;
            *outnumber = (ushort)i;
            return true;
        }
    }
    static bool TakeUserInput(const char *promptmessage, unsigned char *outnumber, ushort trials, ushort timeout_sec = 60)
    {
        int i;
        if (TakeUserInput(promptmessage, &i, trials, timeout_sec))
        {
            if (i >= 255)
                return false;
            *outnumber = (unsigned char)i;
            return true;
        }
    }

    /**
 * @brief bool input code as  0 / 1 or y / n
 * 
 * @param promptmessage 
 * @param outvalue 
 * @param trials 
 * @return true 
 * @return false 
 */
    static bool TakeUserInput(const char *promptmessage, bool *outvalue, ushort trials, ushort timeout_sec = 60)
    {
        const ushort siz = 2;
        char userstring[siz];
        if (_takeUserInputPrim(promptmessage, testbool, userstring, siz, trials, timeout_sec))
        {
            if (*userstring == 'y' || *userstring == '1')
                *outvalue = true;
            else if (*userstring == 'n' || *userstring == '0')
                *outvalue = false;
            else
                return false;

            return true;
        }
        else
            return false;
    }

    /**
 * @brief wait for an decimal input. 
 * 
 * @param promptmessage : message prompt to display
 * @param outnumber : pointer to the input variable
 * @param trials : max number of trials 
 * @return true : good input
 * @return false : input failure, expiration of the given trials, don't use the outnumber pointer.
 */
    static bool TakeUserInput(const char *promptmessage, double *outnumber, ushort trials, ushort timeout_sec = 60)
    {
        const ushort double_nbdigitmax = 15;
        char userstring[double_nbdigitmax];
        if (_takeUserInputPrim(promptmessage, testdecimal, userstring, double_nbdigitmax, trials, timeout_sec))
        {
            *outnumber = atof(userstring);
            return true;
        }
        else
            return false;
    }
    static bool TakeUserInput(const char *promptmessage, float *outnumber, ushort trials, ushort timeout_sec = 60)
    {
        const ushort double_nbdigitmax = 15;
        char userstring[double_nbdigitmax];
        if (_takeUserInputPrim(promptmessage, testdecimal, userstring, double_nbdigitmax, trials, timeout_sec))
        {
            *outnumber = atof(userstring);
            return true;
        }
        else
            return false;
    }
};
} // namespace CONSOLEMENU_NAMESPACE
