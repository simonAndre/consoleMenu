#pragma once
#include "Menuitem.h"
#include "specialTypes.h"
// #include "Menuitem.hpp"
// #include "MenuitemCallback.hpp"
// #include "MenuitemUpdater.hpp"
// #include "MenuitemHierarchy.hpp"
// #include "Menu.hpp"
namespace CONSOLEMENU_NAMESPACE
{

class MenuitemCallback : public Menuitem
{
private:
protected:
    fp_callback1 _mFonction_form1{nullptr};
    fp_callback3 _mFonction_form3{nullptr};
    fp_namingcallback *_mNamingFonction{nullptr};

public:
    /**
 * @brief Construct a new MenuitemCallback object
 * 
 * @param menuname 
 * @param id 
 * @param parentid 
 * @param menuFonction 
 * @param type 
 */
    MenuitemCallback(Menubase *menuinstance, const char *menuname, MenuitemHierarchy *parent, menutype type, ushort menukey);

    MenuitemCallback();

    void SetCallback(fp_callback1 menuFonction);
    void SetCallback(fp_callback3 menuFonction);
    void SetNamingCallback(fp_namingcallback namingFonction);
    void setMenuKey(ushort key);
    virtual void display(ushort idx_menu) override;

    /**
 * @brief call the menu function
 * 
 * @return true if call was successfull (return bool from the callback).
 * if not successfull, false to prompt again in the outside loop. 
 */
    virtual bool selectAction() override;
};
} // namespace CONSOLEMENU_NAMESPACE