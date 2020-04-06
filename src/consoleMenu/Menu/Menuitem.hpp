#pragma once

namespace CONSOLEMENU_NAMESPACE
{
class Menuitem
{
private:
public:
    ushort mid;
    ushort mparentid = 0;
    ushort mkey = 0; //optional
    std::string mname;
    menutype mtype;
    fp_callback1 mFonction_form1 = NULL;
    fp_callback2 mFonction_form2 = NULL;
    fp_callback3 mFonction_form3 = NULL;
    fp_namingcallback *mNamingFonction = NULL;
    void *variableToUpdate = NULL;
    ushort inputtrials;
    size_t stringToUpdateSize;

    /**
 * @brief Construct a new Menuitem object
 * 
 * @param menuname 
 * @param id 
 * @param parentid 
 * @param menuFonction 
 * @param type 
 */
    Menuitem(const char *menuname, ushort id, ushort parentid, menutype type)
    {
        mid = id;
        mparentid = parentid;
        mname = std::string(menuname);
        mtype = type;
    }

    void SetCallback(fp_callback1 menuFonction)
    {
        mFonction_form1 = menuFonction;
    }
    void SetCallback(fp_callback2 menuFonction)
    {
        mFonction_form2 = menuFonction;
    }
    void SetCallback(fp_callback3 menuFonction)
    {
        mFonction_form3 = menuFonction;
    }
    void SetNamingCallback(fp_namingcallback namingFonction)
    {
        mNamingFonction = namingFonction;
    }
    void SetVarToUpdate(void *varptr)
    {
        variableToUpdate = varptr;
    }
    void SetInputTrials(ushort trials)
    {
        inputtrials = trials;
    }
    void SetStringToUpdateSize(size_t size)
    {
        stringToUpdateSize = size;
    }
};
} // namespace CONSOLEMENU_NAMESPACE