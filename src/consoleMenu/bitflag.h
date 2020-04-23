#pragma once
#include "Namespace.h"
namespace CONSOLEMENU_NAMESPACE
{
    /**
     * @brief it flags manager stored in an unsigned char (memory efficiant for small devices)
     * 
     */
class bitflag
{
private:
    unsigned char _store=0x0;

public:
    bitflag();
    bitflag(unsigned char init );

    /**
 * @brief get the fag state (true of false)
 * 
 * @param iflag : index of flag (0 to 7)
 * @return true 
 * @return false 
 */
    bool Get(char iflag);

    /**
     * @brief switch the flag state and get the new value
     * 
     * @param iflag : index of flag to switch (0 to 7)
     * @return true 
     * @return false 
     */
    bool Switch(char iflag);

    /**
     * @brief Set the Flag state 
     * 
     * @param iflag : index of flag to switch (0 to 7)
     * @param val true of false
     */
    void Set(char iflag,bool val);
};

} // namespace CONSOLEMENU_NAMESPACE