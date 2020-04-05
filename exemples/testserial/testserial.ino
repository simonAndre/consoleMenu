
#include <Arduino.h>

typedef unsigned short ushort;
typedef bool (*fp_tester)(char);

String TakeUserInput_s(const char *promptmessage)
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

bool testint(char c)
{
    return (c >= 48 && c <= 57);
}
bool testdecimal(char c)
{
    return ((c >= 48 && c <= 57) || c == 46 || c == 44);
}

/**
 * @brief prompt a message, wait for an input string and test if this string conforms to the tester method. 
 * If it's OK, return the string that can be eventually cast to a specific type.
 * If not: the user will be prompted again [trials] times.
 * 
 * @param promptmessage : displayed to the user
 * @param tester : callback to test every char in the string
 * @param trials : max number of trials 
 * @return const char* : the input string to cast back to something or a null pointer of it fails and expired the given trials
 */
const char *_takeUserInputPrim(const char *promptmessage, fp_tester tester, ushort trials)
{
    ushort i = 0;
    bool ok = false;
    String userinput;
    do
    {
        userinput = TakeUserInput_s(promptmessage);
        for (size_t p = 0; p < userinput.length() - 1; p++)
        {
            char c = userinput.charAt(p);
            Serial.print(c);
            Serial.print("-");
            if (!tester(c))
            {
                Serial.println("bad input, try again..");
                i++;
                ok = false;
                break;
            }
            ok = true;
        }
    } while (i > 0 && i < trials);
    if (ok)
        return userinput.c_str();
    return nullptr;
}

bool TakeUserInput_i(const char *promptmessage, int *outnumber)
{
    const char *userstring = _takeUserInputPrim(promptmessage, testdecimal, 3);
    if (userstring != nullptr)
    {
        *outnumber = atoi(userstring);
        return true;
    }
    else
        return false;
}
bool TakeUserInput_f(const char *promptmessage, double *outnumber)
{
    const char *userstring = _takeUserInputPrim(promptmessage, testdecimal, 3);
    if (userstring != nullptr)
    {
        *outnumber = atof(userstring);
        return true;
    }
    else
        return false;
}

void setup()
{
    Serial.begin(115200);
}

void loop()
{
    String s = TakeUserInput_s("entrez une chaine>");
    Serial.println(s);
    Serial.println();

    int i;
    if (TakeUserInput_i("entrez un entier >", &i))
    {
        Serial.println();
        Serial.println(i);
    }

    double f;
    if (TakeUserInput_f("entrez un decimal >", &f))
    {
        Serial.println();
        Serial.println(f);
    }
}