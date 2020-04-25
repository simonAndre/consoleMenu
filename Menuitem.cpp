
#include "src/consoleMenu/Menu/Menuitem.h"
#include "src/consoleMenu/Menu/Menubase.h"
#include "src/consoleMenu/IO/IoHelpers.hpp"

namespace CONSOLEMENU_NAMESPACE
{

Menuitem::Menuitem() {}

/**
 * @brief Construct a new Menuitembase object
 * 
 * @param menuname 
 * @param id 
 * @param parentid 
 * @param menuFonction 
 * @param type 
 */
Menuitem::Menuitem(Menubase *menuinstance, const char *menuname, SubMenu *parent)
{
    this->_menuinstance = menuinstance;
    this->_mparent = parent;
    this->SetLabel(menuname);
}

void Menuitem::setMenuInstance(Menubase *mi)
{
    this->_menuinstance = mi;
}

SubMenu *Menuitem::getParent()
{
    return (SubMenu *)this->_mparent;
}
Menuitem *Menuitem::SetParent(SubMenu *parent)
{
    if (!this->flags.Get(menuitemflags::menuparentset) || this->getType()== menutype::back)
    {
        this->_mparent = parent;
        this->flags.Set(menuitemflags::menuparentset, true);
        return this;
    }
    else
        throw std::runtime_error("SetParent can be called only once per menu-item");
}
    
Menuitem* Menuitem::SetLabel(const char *name)
{
    this->_mname =name;
    return this;
}

void Menuitem::setType(menutype type)
{
    this->_mtype = type;
}
menutype Menuitem::getType()
{
    return this->_mtype;
}
ushort Menuitem::getMenuKey()
{
    return this->_mkey;
}
ushort Menuitem::getId()
{
    return this->_mid;
}
Menuitem* Menuitem::SetId(ushort id)
{
    if (!this->flags.Get(menuitemflags::menuidset))
    {
        this->_mid = id;
        this->flags.Set(menuitemflags::menuidset, true);
        return this;
    }
    else
        throw std::runtime_error("SetId can be called only once per menu-item");
}

const char *Menuitem::getLabel()
{
    if (this->_mNamingFonction)
        return this->_mNamingFonction(this->_mkey);
    return this->_mname;
}


Menuitem *Menuitem::addExit()
{
    this->flags.Set(menuitemflags::exitemnu, true);
    return this;
}

Menuitem *Menuitem::setMenuKey(ushort menukey)
{
    if (!this->flags.Get(menuitemflags::menukeyset))       
    {
        this->_menuinstance->addMenuKey(menukey, this->getId());
        this->_mkey = menukey;
        this->flags.Set(menuitemflags::menukeyset, true);
        return this;
    }
    else
        throw std::runtime_error("setMenuKey can be called only once per menu-item");
}
/**
 * @brief display the menuitem 
 * 
 * @param dispcallback function to use to render (UI dependant)
 * @param idx_menu : num to use for menuitem selection
 */
void Menuitem::display(ushort idx_menu)
{
    //use menuname
    IoHelpers::IOdisplay(idx_menu);
    IoHelpers::IOdisplay(this->_menuinstance->getOptions().id_separator);
    IoHelpers::IOdisplay(this->getLabel());
    IoHelpers::IOdisplayLn("");
}

/**
 * @brief call the menu functions. Stop the callback call on the first failing callback (returning false) 
 * 
 * @return SelectActionResult
 */
SelectActionResult Menuitem::selectAction()
{
    IoHelpers::IOdisplayLn("");
    SelectActionResult res;

    res.exitRequested = this->flags.Get(menuitemflags::exitemnu);

//execute the lambdas
    std::vector<std::function<void()>>::const_iterator it_lambda;
    for (it_lambda = _lambdas_const.begin(); it_lambda != _lambdas_const.end(); ++it_lambda)
    {
        (*it_lambda)(); // call the lambda
    }

    //execute the _callbacksForm1
    std::vector<fp_callback1>::const_iterator it_form1;
    for (it_form1 = _callbacksForm1.begin(); it_form1 != _callbacksForm1.end(); ++it_form1)
    {
        res.callbacksSuccessfull &= (*it_form1)(); // call the callback form1
        if (!res.callbacksSuccessfull && this->_menuinstance->getOptions().breakCallbackChainOnFirstError)
            return res;
    }

    //execute the _callbacksForm3
    std::vector<fp_callback3>::const_iterator it_form3;
    for (it_form3 = _callbacksForm3.begin(); it_form3 != _callbacksForm3.end(); ++it_form3)
    {
        res.callbacksSuccessfull &= (*it_form3)(this->getMenuKey(), this->getLabel()); // call the callback form3
        if (!res.callbacksSuccessfull && this->_menuinstance->getOptions().breakCallbackChainOnFirstError)
            return res;
    }
    return res;
}

Menuitem *Menuitem::addCallback(fp_callback1 newcallback)
{
    this->_callbacksForm1.push_back(newcallback);
    return this;
}

Menuitem *Menuitem::addCallback(fp_callback3 newcallback)
{
    this->_callbacksForm3.push_back(newcallback);
    return this;
}

Menuitem *Menuitem::addLambda(std::function < void()> func)
{
   this->_lambdas_const.push_back(func);
    return this;
}

Menuitem *Menuitem::SetNamingCallback(fp_namingcallback namingFonction)
{
    _mNamingFonction = namingFonction;
    return this;
}

} // namespace CONSOLEMENU_NAMESPACE