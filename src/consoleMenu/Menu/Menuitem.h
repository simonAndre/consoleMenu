#pragma once
#include <string>
#include <string.h>
#include <stdexcept>
#include "../commontypes.h"
#include "specialTypes.h"
#include <vector>
#include "../wrapfunc.hpp"

namespace CONSOLEMENU_NAMESPACE
{

//forward declaration
class Menubase;
class SubMenu;

enum menuitemflags
{
   exitemnu,     //an exit option has been configured : exit of menu display after execution of callbacks
   menukeyset,   // a menukey has been given to this item
   menuidset,   // a menu id has been given to this item
   menuparentset // a parent has been defined for this  item
};

class Menuitem
{
private:
   std::vector<fp_callback1>
       _callbacksForm1;
   std::vector<fp_callback3>
       _callbacksForm3;
   bitflag flags;

protected:
   Menubase *_menuinstance{nullptr};
   ushort _mid = 0;
   SubMenu *_mparent{nullptr};           // if defined, it must be a SubMenu
   ushort _mkey = CONSOLEMENU_NOMENUKEY; //optional
   const char* _mname;
   menutype _mtype;

public:
   Menuitem();

   /**
 * @brief Construct a new Menuitembase object
 * 
 * @param menuname 
 * @param id 
 * @param parentid 
 * @param menuFonction 
 * @param type 
 */
   Menuitem(Menubase *menuinstance, const char *menuname, SubMenu *parent, menutype type);

   void setMenuInstance(Menubase *mi);

   SubMenu *getParent();
   Menuitem *SetParent(SubMenu *parent);
   Menuitem *SetLabel(const char *name);
   virtual const char *getLabel();
   void setType(menutype type);
   menutype getType();
   virtual ushort getMenuKey();

   /**
  * @brief add a Menu Key to this menu-item (throw runtimeExeption if this menukey alreaky exists)
  * 
  * @param menukey 
  * @return Menuitem* to chain with other configuration for this MenuItem
  */
   Menuitem *setMenuKey(ushort menukey);
   ushort getId();
   virtual Menuitem* SetId(ushort id);
   /**
 * @brief display the menuitem 
 * 
 * @param dispcallback function to use to render (UI dependant)
 * @param idx_menu : num to use for menuitem selection
 */
   virtual void display(ushort idx_menu);

   virtual SelectActionResult selectAction();

   /**
 * @brief Add a callback function to this menuitem (aditive feature : don't override preexisting defined actions)
 * 
 * @param newcallback prototype form = [bool function();]
 * @return Menuitem* to chain with other configuration for this MenuItem
 */
   Menuitem *addCallback(fp_callback1 newcallback);

   /**
 * @brief Add a callback function to this menuitem (aditive feature : don't override preexisting defined actions)
 * 
 * @param newcallback prototype form = [bool function(ushort menukey,const char* itemlabel);]
 * @return Menuitem* to chain with other configuration for this MenuItem
 */
   Menuitem *addCallback(fp_callback3 newcallback);

   /**
 * @brief Add an exit signal in the callback loop for this menu item.
 * the menu will be exited and the control flow will be resumed right after the Menu::launchMenu() method (or the Menu::LoopCheckSerial() method)
 * 
 * @return Menuitem* 
 */
   Menuitem *addExit();
};

} // namespace CONSOLEMENU_NAMESPACE