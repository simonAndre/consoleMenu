#pragma once

#include <string.h>

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
    static bool _takeUserInputPrim(const char *promptmessage, fp_tester tester, char *outstr, size_t stringbuffersize, ushort trials)
    {
        ushort i = 0;
        do
        {
            if (!TakeUserInput(promptmessage, outstr, stringbuffersize, 1))
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
                    return true;
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
 * @brief diplay a message followed by a carriage return to the user on the available console (use Serial for embedded devices, std::cout for computers)
 * 
 * @param infos 
 */
    static void IOdisplayLn(const char *infos)
    {
        IOdisplay(infos);
        IOdisplay("\n");
    }
    /**
 * @brief diplay an int value followed by a carriage return to the user on the available console (use Serial for embedded devices, std::cout for computers)
 * 
 * @param infos 
 */
    static void IOdisplayLn(int ivalue)
    {
        IOdisplay(ivalue);
        IOdisplay("\n");
    }
    static void IOdisplayLn(unsigned int ivalue)
    {
        IOdisplay(ivalue);
        IOdisplay("\n");
    }
    static void IOdisplayLn(long ivalue)
    {
        IOdisplay(ivalue);
        IOdisplay("\n");
    }
    static void IOdisplayLn(unsigned long ivalue)
    {
        IOdisplay(ivalue);
        IOdisplay("\n");
    }
    /**
 * @brief diplay a bool value followed by a carriage return to the user on the available console (use Serial for embedded devices, std::cout for computers)
 * 
 * @param infos 
 */
    static void IOdisplayLn(bool value)
    {
        IOdisplay(value);
        IOdisplay("\n");
    }

    /**
 * @brief diplay a float/double value followed by a carriage return to the user on the available console (use Serial for embedded devices, std::cout for computers)
 * 
 * @param infos 
 */
    static void IOdisplayLn(double fvalue)
    {
        IOdisplay(fvalue);
        IOdisplay("\n");
    }

    /**
 * @brief diplay a message to the user on the available console (use Serial for embedded devices, std::cout for computers)
 * 
 * @param infos 
 */
    static void IOdisplay(const char *infos)
    {
#if CONSOLEMENU_EMBEDDED_MODE
        Serial.print(infos);
#else
        std::cout << infos;
#endif
    }
    /**
 * @brief diplay an int value to the user on the available console (use Serial for embedded devices, std::cout for computers)
 * 
 * @param infos 
 */
    static void IOdisplay(int ivalue)
    {
#if CONSOLEMENU_EMBEDDED_MODE
        Serial.print(ivalue);
#else
        std::cout << ivalue;
#endif
    }
    static void IOdisplay(long ivalue)
    {
#if CONSOLEMENU_EMBEDDED_MODE
        Serial.print(ivalue);
#else
        std::cout << ivalue;
#endif
    }
    static void IOdisplay(unsigned long ivalue)
    {
#if CONSOLEMENU_EMBEDDED_MODE
        Serial.print(ivalue);
#else
        std::cout << ivalue;
#endif
    }

    static void IOdisplay(unsigned int ivalue)
    {
#if CONSOLEMENU_EMBEDDED_MODE
        Serial.print(ivalue);
#else
        std::cout << ivalue;
#endif
    }
    /**
 * @brief diplay a float/double value to the user on the available console (use Serial for embedded devices, std::cout for computers)
 * 
 * @param infos 
 */
    static void IOdisplay(double fvalue)
    {
#if CONSOLEMENU_EMBEDDED_MODE
        Serial.print(fvalue);
#else
        std::cout << fvalue;
#endif
    }

    /**
 * @brief diplay a bool value to the user on the available console (use Serial for embedded devices, std::cout for computers)
 * 
 * @param infos 
 */
    static void IOdisplay(bool bvalue)
    {
#if CONSOLEMENU_EMBEDDED_MODE
        if (bvalue)
            Serial.print("y");
        else
            Serial.print("n");
#else
        if (bvalue)
            std::cout << "y";
        else
            std::cout << "n";
#endif
    }

    /**
 * @brief Wait for a user input given from the available console (use Serial for embedded devices, std::cin for computers)
 * 
 * @return const char* 
 */
    static bool WaitforInput(char *outstring, size_t stringbuffersize)
    {
        const char *proviput;
        size_t proviputsize;
#if CONSOLEMENU_EMBEDDED_MODE
        bool inputdone = false;
        Serial.flush();
        do
        {
            delay(10);
            if (Serial.available() > 0)
            {
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
                    IOdisplayLn("oversize input.");
                    return false;
                }
            }
        } while (!inputdone);
#else
        std::string input;
        std::cin >> input;
        proviputsize = input.length();
        proviput = input.c_str();
#endif

        if (proviputsize < stringbuffersize)
        {
            strncpy(outstring, proviput, proviputsize);
            outstring[proviputsize] = '\0';
            return true;
        }
        else
        {
            IOdisplayLn("oversize input.");
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
    static bool TakeUserInput(const char *promptmessage, char *outstring, size_t stringbuffersize, ushort trials)
    {
        IOdisplay(promptmessage);
        IOdisplay(">");
        ushort t = 0;
        while (!WaitforInput(outstring, stringbuffersize) && t++ < trials)
        {
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
    static bool TakeUserInput(const char *promptmessage, int *outnumber, ushort trials)
    {
        ushort int_nbdigitmax = 10;
        char userstring[int_nbdigitmax + 1];
        if (_takeUserInputPrim(promptmessage, testint, userstring, int_nbdigitmax, trials))
        {
            *outnumber = atoi(userstring);
            return true;
        }
        else
            return false;
    }
    static bool TakeUserInput(const char *promptmessage, ushort *outnumber, ushort trials)
    {
        int i;
        if (TakeUserInput(promptmessage, &i, trials))
        {
            if (i >= 65535)
                return false;
            *outnumber = (ushort)i;
            return true;
        }
    }
    static bool TakeUserInput(const char *promptmessage, unsigned char *outnumber, ushort trials)
    {
        int i;
        if (TakeUserInput(promptmessage, &i, trials))
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
    static bool TakeUserInput(const char *promptmessage, bool *outvalue, ushort trials)
    {
        ushort siz = 2;
        char userstring[siz];
        if (_takeUserInputPrim(promptmessage, testbool, userstring, siz, trials))
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
    static bool TakeUserInput(const char *promptmessage, double *outnumber, ushort trials)
    {
        ushort double_nbdigitmax = 15;
        char userstring[double_nbdigitmax];
        if (_takeUserInputPrim(promptmessage, testdecimal, userstring, double_nbdigitmax, trials))
        {
            *outnumber = atof(userstring);
            return true;
        }
        else
            return false;
    }
};
} // namespace CONSOLEMENU_NAMESPACE