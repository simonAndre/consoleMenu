# consoleMenu, a simple console UI hierarchy menu


(c) Simon ANDRE, 2019

target : any environment : embedded, linux or windows.
mainly done for embedded, its main purpose is to be simple and very lightweight with a small memory footprint.

work on a callback basis : each call to a menu item is linked to a simple callback function. 
Or even simpler, a pointer to the variable to update can just be hooked to a menu item.

## tagets

+ linux computers
+ tested embedded microcontroler :
  + esp32 series (espressif)
  + for AVR series, use the STL lib (to be tested )

## build, test

+ build : use the Makefile.
  + linux : `make all`
  + windows : `mingw32-make all`
+ exemple :
  + build the exemple via the makefile : `make exemple`
  + run the exemple : `./exemple` or `./exemple.exe`
+ unit tests:
  + on the first run:
    + linux : `make tests-prep`
    + windows : `mingw32-make tests-prep`
  + for each neq test run:
    + linux : `make tests`
    + windows : `mingw32-make tests`  
  + WIP

## exemple of rendering : 

```
---- submenu inputs w callbacks ----
1 - < back
2 - display value str1
3 - change str1 [=first string]
4 - change int1 [=1234]
5 - change float1 [=5688.56]
6 - change bool1 [=ON]
7 - > exit
please choose an action: >
```

## usage

### Menu initialisation 

you need to provide the size of the menu collection as a template parameter. : 
> Menubase *mymenu =new Menu<**menu-size**>(); 

### fluent configuration

The base element of the configuration is the [menu-item](#Menu-items).

Each menu-item match a line in the menu. Menu-items can be organized hierarchically within submenus.

To start the menu configuration, from the menuinstance use the method addSubMenu or get the root menu-item with getRootMenu method.

From the submenu, you can add other child items with one of this fluent method : addSubMenu, addMenuitemCallback, addMenuitemUpdater...

And you can chain from there the configuration of the menu-item properties : 

```C++
mysubmenu->addMenuitemCallback("menuitem label")->setMenuKey(2)->addCallback(firstCallback)->addCallback(secondCallback)->addExit(); 
```
this example add a menu-item of type MenuitemCallback to the submenu [mysubmenu] and set its menukey to 2 (optional, internal id are automatically given) and hook 2 callbacks and signal the a call to this menu-item will exit from the menu to resume the control flow to the main program.

### setup : 

#### the easy way 
```C++
#include <consoleMenu.h>
Menubase *mymenu =new Menu<5>();   //declare a menu sized for 5 menu-items
// menus & submenus definition
SubMenu *root = mymenu->getRootMenu();        //get the root menu-item
SubMenu *submenu1 = root->addSubMenu("Submenu 1");  // declare and get the menu-item for the submenu1
 
// add a callback menu-item to the root menu:
root->addMenuitemCallback("simple menu and exit", simpleMenu);    // simpleMenuis a simple callback without parameter, function is : [bool simpleMenu();]
// ... (see below for more examples)
```

#### the complete way
example of the definition of a 3 level hierarchical menu:

```C++
// include the library
#include <consoleMenu.h>
Menubase *mymenu =new Menu<15>();   //declare a menu sized for 15 menu-items

//optional : define some options
MenuOptions menuoptions;
menuoptions.addBack = true;
menuoptions.addExitForEachLevel = true;
mymenu->setOptions(mo);

// menus & submenus definition
// root menus
SubMenu *root = mymenu->getRootMenu();        //get the root menu-item
SubMenu *submenu1 = root->addSubMenu("Submenu inputs w callbacks");
SubMenu *submenu2 = root->addSubMenu("submenu inputs w updaters");
root->addMenuitemCallback("simple menu and exit", simpleMenu);    // simpleMenuis a simple callback without parameter, function is : [bool simpleMenu();]
root->addMenuitemCallback("consoleMenu version", getVersionMenu); // callback with menu name passed as parameter, see function menuParamName

root->addMenuitemCallback("test prompted inputs", testIO); // callback with menu name passed as parameter, see function menuParamName
// level 2 menus, under the item [submenu1]
submenu1->addMenuitemCallback("set string", initStringValue);
submenu1->addMenuitemCallback("set int value", initIntValue);
// this menu is dynamic : its name is provided by the function [switchMenuDisplay], it can be updated depending of the context
submenu1->addMenuitemCallback(switchMenuDisplay, (ushort)MyMenuKeys::switchmenu1, switchMenu);

// more levels can be chained...
SubMenu *submenu3 = submenu1->addSubMenu("sub menu 2");
submenu3->addMenuitemCallback("build infos", buildInfos); //still a simple menu
// another dynamic menu bind to the same callbacks with a different key
submenu3->addMenuitemCallback(switchMenuDisplay, (ushort)MyMenuKeys::switchmenu2, switchMenu);

// menuitemUpdater automatically update a variable (given by its pointer) from the user input value.
submenu2->addMenuitemUpdater("change str1", (char *)staticString, sizeof(staticString));    // example with a string
submenu2->addMenuitemUpdater("change int1", &int1);     // example with an int
submenu2->addMenuitemUpdater("change float1", &float1);     // example with a float
submenu2->addMenuitemUpdater("change bool1", &bool1);      // example with a bool (rendered as ON/OFF or Y/N..)

// and display the root menu (can be call later, in the main loop for example)
mymenu->launchMenu();
```

### call the menu to render

simply use Menu::launchMenu() this call will exit when the user make his choice and the callback give the control back to the main program flow.

#### method 1

suitable on a computer or in a specific embedded method):

```C++
mymenu->launchMenu();
```

#### method 2

in a process loop (for exemple : the loop() arduino method):

```C++
mymenu->LoopCheckSerial();
```


### callbacks

simple function based on one of the following prototypes. 
choosing one prototype or another is up to you depending on your implementation choices.
For all of these prototypes, the return bool value is meant to signal a break in the callback chain :
+ if its **true** : signal a normal execution of the callback, if other callback follow in the chain, they will be executed afterward, if not the control is given back to display the menu again to let the user select another menu option.
+ if its **false** : break the callback chain : no further execution of callbacks and get back to the menu display (if previous callback where declared, they have already completed). 

#### form 1

the simplest, no parameters (hence, you need to define one function per menuitem).

```C++
bool menucallback();
```

#### form 2

+ the parameter menukey is the user-key of the selected menuitem (set in the Menu::addCallbackMenuitem method)
+ the parameter menuname is the label of the selected menuitem (as seen by the end-user).

```C++
bool menucallback(ushort menukey, const char *menuname);
```

### Menu-items

A menu is made of several menu-items of one of the following types : 
+ SubMenu : 
  + This type of menu-item is handling the hierarchy of the menu. 
  + From the SubMenu items, it's possible to nest every type of menu-item.
  + The root menu-item is of SubMenu type.
+ MenuitemCallback : 
  + call a function performing a specific action
  + see the [callback](#callbacks) section to consult the supported forms (or prototypes) of the callback functions you can declare.
  + the SetNamingCallback method can be used to label dynamically the menu-item. Otherwise, a static label must be provided on the declaration of the menu-item. 
+ MenuitemUpdater : 
  + Provides a straight-forward method to prompt the user for an update on the content of a variable, without any callback.
  + A pointer to the target variable is given as parameter to this method

some special menu-items like exit and back entries are added automatically by the library (given your options) and doesn't need to be counted for the collection size to declare with the [menu type template](#Menu-initialisation). 

### Menu items declaration methods

The menu-items are declared from the parent SubMenu they will be nested under. The first SubMenu is the root, it is retrieved by this way : 
> SubMenu *root = mymenu->getRootMenu();


#### SubMenu menu-item

add an item heading to a submenu

```c++
/**
* @brief add a submenu item hierarchy
* 
* @param label : label displayed
* @return new SubMenu* created 
*/
 SubMenu *addSubMenu(const char *label);


// example : 
SubMenu *newsubmenu = parentitem->addSubMenu("new submenu");
```

#### callback menu-item

add a menu item calling a given function.


Overloads of addMenuitemCallback : 

```c++
  /**
   * @brief add a submenu item calling a given function - static labelling
   * 
   * @param label : label displayed
   * @param menukey : key to use for this menu (the unicity of this key is under your concern, this menu-item con't be created if it's not unique).
   * @param onselectFunc : pointer to the function to call for this menu. on the form @fp_callback3.
   * If the return value of the function is true : exit the menu after execution of this function, 
   * else stay in the current menu and wait for another action
   * @return true : new menu-item correctly added to the collection.
   * @return false : an issue occured (typically, check the unicity of menukey)
   */
  bool addMenuitemCallback(const char *label, ushort menukey, fp_callback3 onselectFunc);

  /**
   * @brief add a submenu item calling a given function - dynamic labelling
   * 
   * @param namingFunc : callback to a function provinding dynamically the menuitem name (possibly given the menukey)
   * @param menukey : key to use for this menu (the unicity of this key is under your concern, this menu-item con't be created if it's not unique).
   * @param onselectFunc : pointer to the function to call for this menu. on the form @fp_callback3.
   * If the return value of the function is true : exit the menu after execution of this function, 
   * else stay in the current menu and wait for another action
   * @return true : new menu-item correctly added to the collection.
   * @return false : an issue occured (typically, check the unicity of menukey)
     */
  bool addMenuitemCallback(fp_namingcallback namingFunc, ushort menukey, fp_callback3 onselectFunc);

  /**
   * @brief add a submenu item calling a given simple function (no params) - static labelling
   * 
   * @param label : label displayed
   * @param onselectFunc : pointer to the function to call for this menu. on the form @fp_callback1 (no params).
   * If the return value of the function is true : exit the menu after execution of this function, 
   * else stay in the current menu and wait for another action
   * @return true : new menu-item correctly added to the collection.
   * @return false : an issue occured (typically, check the unicity of menukey)
   */
  bool addMenuitemCallback(const char *label, fp_callback1 onselectFunc);

  /**
   * @brief add a submenu item calling a given simple function (no params) - dynamic labelling
   * 
   * @param namingFunc : callback to a function provinding dynamically the menuitem name (possibly given the menukey)
   * @param onselectFunc : pointer to the function to call for this menu. on the form @fp_callback1 (no params).
   * If the return value of the function is true : exit the menu after execution of this function, 
   * else stay in the current menu and wait for another action
   * @return true : new menu-item correctly added to the collection.
   * @return false : an issue occured (typically, check the unicity of menukey)
   */
  bool addMenuitemCallback(fp_namingcallback namingFunc, fp_callback1 onselectFunc);
```

#### Updater menu-item 

Provides a straight-forward method to prompt the user for an update on the content of a variable, without any callback.
A pointer to the target variable is given as parameter to this method.
The target variable can be of any of simple valued type like : 
+ c-string (char*)
+ int
+ ushort (unsigned short)
+ double/float
+ bool (true value can be input as y or 1 and false : n or 0)...

c-string types are handled specifically as we must provide the size of the string buffer as a parameter.

prototypes :

```c++

 /**
 * @brief add a menu item designed for one goal : update a value in a given pointer to a variable.
     * 
 * 
 * @tparam T 
 * @param label 
 * @param variableToUpdate : pointer to the variable to be updated 
 * @param stringsize : size max of the c-string (buffer of char) 
 * @return MenuitemUpdaterbase* to chain with other configuration for this MenuItem
 */
  template <typename T>
  bool addMenuitemUpdater(const char *label, T *variableToUpdate)
  {
      MenuitemUpdater<T> *miu = new MenuitemUpdater<T>(this->_menuinstance, label, this, menutype::variableUpdater_i);
      miu->setVarToUpdate(variableToUpdate);
      miu->setInputTrials(this->_menuinstance->getOptions().badInputRepeats);
      return this->_menuinstance->addChild(this, miu);
  }

  /**
   * @brief add a menu item designed for one goal : update a string value in a given pointer to a variable.
   * 
   * @param label 
   * @param variableToUpdate : pointer to the variable to be updated
   * @param stringsize : size max of the c-string (buffer of char)
   * @return MenuitemUpdaterbase* to chain with other configuration for this MenuItem
   */
  bool addMenuitemUpdater(const char *label, char *variableToUpdate, size_t stringsize);
```



## Licence

This code is released under the MIT License.