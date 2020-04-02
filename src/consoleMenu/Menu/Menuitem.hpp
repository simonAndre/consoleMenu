#pragma once
#include <consoleMenu/Menu/types.hpp>

namespace CONSOLEMENU_NAMESPACE
{
class Menuitem
{
private:
public:
    /**
 * @brief Construct a new Menuitem object
 * 
 * @param menuname 
 * @param id 
 * @param parentid 
 * @param menuFonction 
 * @param type 
 */
    Menuitem(const char *menuname, ushort id, ushort parentid, pf_callback menuFonction, menutype type)
    {
        mid = id;
        mparentid = parentid;
        mname = std::string(menuname);
        mFonction = menuFonction;
        mtype = type;
    }

    ushort mid;
    ushort mparentid = 0;
    std::string mname;
    menutype mtype;
    pf_callback mFonction;
};
} // namespace CONSOLEMENU_NAMESPACE