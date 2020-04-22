#pragma once
#include "Menuitem.h"
#include "specialTypes.h"
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
    MenuitemCallback(Menubase *menuinstance, const char *menuname, SubMenu *parent, menutype type, ushort menukey);

    MenuitemCallback();

    void SetNamingCallback(fp_namingcallback namingFonction);
    void setMenuKey(ushort key);
    virtual const char *getLabel() override;
};
} // namespace CONSOLEMENU_NAMESPACE