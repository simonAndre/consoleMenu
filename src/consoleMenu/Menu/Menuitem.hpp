#pragma once

namespace CONSOLEMENU_NAMESPACE
{

//forward declaration
class Menu;

class Menuitem
{
private:
    Menu *_menuinstance;

public:
    ushort mid;
    Menuitem *mparent = 0;
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

    // Menuitem(Menu menuinstance)
    // {
    //     _menuinstance = menuinstance;
    // };

    /**
 * @brief Construct a new Menuitem object
 * 
 * @param menuname 
 * @param id 
 * @param parentid 
 * @param menuFonction 
 * @param type 
 */
    Menuitem(Menu *menuinstance, const char *menuname, ushort id, Menuitem *parent, menutype type)
    {
        _menuinstance = menuinstance;
        mid = id;
        mparent = parent;
        mname = std::string(menuname);
        mtype = type;
    }

    bool isUpdaterMenuItem()
    {
        return this->mtype >= 50 && this->mtype <= 60;
    }

    /**
 * @brief display the menu-item current value, if variableToUpdate is set (for updaters menu-items)
 * 
 * @param displaycb : display callback
 * @param addbrackets : true to surround with brackets
 */
    void displayCurrentValue()
    {
        if (this->variableToUpdate != NULL)
        {
            switch (this->mtype)
            {
            case menutype::variableUpdater_s:
                IoHelpers::IOdisplay((char *)this->variableToUpdate);
                break;
            case menutype::variableUpdater_i:
                IoHelpers::IOdisplay(*(int *)this->variableToUpdate);
                break;
            case menutype::variableUpdater_us:
                IoHelpers::IOdisplay(*(ushort *)this->variableToUpdate);
                break;
            case menutype::variableUpdater_uc:
                IoHelpers::IOdisplay(*(unsigned char *)this->variableToUpdate);
                break;
            case menutype::variableUpdater_d:
                IoHelpers::IOdisplay(*(double *)this->variableToUpdate);
                break;
            case menutype::variableUpdater_b:
                IoHelpers::IOdisplay(*(bool *)this->variableToUpdate);
                break;
            }
        }
    }

    /**
 * @brief take the user input, if variableToUpdate is set (for updaters menu-items)
 * 
 * @return true 
 * @return false 
 */
    bool takeUserInput()
    {
        if (this->variableToUpdate != NULL)
        {
            switch (this->mtype)
            {
            case menutype::variableUpdater_s:
                return IoHelpers::TakeUserInput("enter new value>", (char *)this->variableToUpdate, this->stringToUpdateSize, this->inputtrials);
            case menutype::variableUpdater_i:
                return IoHelpers::TakeUserInput("enter new value>", (int *)this->variableToUpdate, this->inputtrials);
            case menutype::variableUpdater_us:
                return IoHelpers::TakeUserInput("enter new value>", (ushort *)this->variableToUpdate, this->inputtrials);
            case menutype::variableUpdater_uc:
                return IoHelpers::TakeUserInput("enter new value>", (unsigned char *)this->variableToUpdate, this->inputtrials);
            case menutype::variableUpdater_d:
                return IoHelpers::TakeUserInput("enter new value>", (double *)this->variableToUpdate, this->inputtrials);
            case menutype::variableUpdater_b:
                if (CONSOLEMENU_UPDATERMENU_BOOLMODESWITCH == 1)
                {
                    *(bool *)this->variableToUpdate = !*(bool *)this->variableToUpdate;
                    return true;
                }
                else
                {
                    return IoHelpers::TakeUserInput("enter new value (0/1)>", (bool *)this->variableToUpdate, this->inputtrials);
                }
            default:
                break;
            }
        }
        return false;
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