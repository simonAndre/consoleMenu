# consoleMenu, a simple console UI hierarchy menu

(c) Simon ANDRE, 2019

target : any environment : embedded, linux or windows.
mainly done for embedded, its main purpose is to be simple and very lightweight with a small memory footprint.

work on a callback basis : each call to a memnu item is linked to a simple callback function.



## build, test

+ build : use the Makefile.
  + linux : `make all`
  + windows : `mingw32-make all`
+ exemple :
  + build tu test via the makefile : `make test`
  + run the test : `./test.exe`

## exemple of rendering : 

```
---------------------
1 - simple menu, no params
2 - switch OFF feature1
3 - test prompted inputs
4 - submenu test inputs 2
5 - Sub menu 1
6 - > exit
please choose an action: >
```

## usage

example of the definition of a 3 level hierarchical menu:
### setup : 

#### the easy way 
```C++
#include <consoleMenu.h>
Menu consolemenu = Menu();
// menus & submenus definition
// root menus
m.addCallbackMenuitem("action 1", menu1, 0);
m.addCallbackMenuitem("action 2", menu2, 0);
// ... (see below for more examples)
```

#### the complete way

```C++
// include the library
#include <consoleMenu.h>

//optional : define options
MenuOptions menuoptions;
menuoptions.addBack = true;
menuoptions.addExitForEachLevel = true;

// declaration,
// DisplayInfos: IO callback to render the menu : optional, a default callback is provided within the library
// WaitforInput: IO callback to wait and read the user input : optional, a default callback is provided within the library
// menuoptions : opional : can also be called after the initialization with the Menu::setOptions method
Menu m(DisplayInfos, WaitforInput, menuoptions);

// menus & submenus definition
// root menus
m.addCallbackMenuitem("simple menu, no params", simpleMenu, 0); // simple callback without parameter, see function simpleMenu
m.addCallbackMenuitem("action 2", menuParamName, 0);            // callback with menu name passed as parameter, see function menuParamName
ushort submenu1id = m.addHierarchyMenuitem("Sub menu 1", 0);
// level 2 menus, under the item [submenu1id]
m.addCallbackMenuitem("init string and stay", initStringValue, submenu1id);
m.addCallbackMenuitem("display string and stay", DisplayStringValue, submenu1id);
m.addCallbackMenuitem("menu init int and stop", initIntValue, submenu1id);
//this menu is dynamic : its name is provided by the function [switchMenuDisplay], it can be updated depending of the context
m.addDynamicCallbackMenuitem(switchMenuDisplay, switchMenu, submenu1id, (ushort)MyMenuKeys::switchmenu1);

// more levels can be chained...
ushort submenu2id = m.addHierarchyMenuitem("sub menu 2", submenu1id);
m.addCallbackMenuitem("build infos", buildInfos, submenu2id); //still a simple menu
//another dynamic menu bind to the same callbacks with a different key
m.addDynamicCallbackMenuitem(switchMenuDisplay, switchMenu, submenu2id, (ushort)MyMenuKeys::switchmenu2);

// updaters without callbacks:
m.addUpdaterMenuitem("change string content", submenu2id, (char *)mystring, sizeof(mystring), 2);
m.addUpdaterMenuitem("change int value", submenu2id, &myint, 2);
m.addUpdaterMenuitem("change boolean value (alternative to switch)", submenu2id, &mybool, 2);


//and display the root menu
m.displayMenu();
```

### call the menu to render

simply use Menu::displayMenu() this call will exit when the user make his choice and the callback give the control back to the main program flow.

#### method 1

suitable on a computer or in a specific embedded method):

```C++
m.displayMenu();
```

#### method 2

in a process loop (for exemple : the loop() arduino method):

```C++
consolemenu.LoopCheckSerial();
```


### callbacks

simple function based on one of the following prototypes. 
choosing one prototype or another is up to you depending on your implementation choices.
For all of these prototypes, the return bool value is meant to signal a menu exit  :
+ if its **true** : the menu is exited just after the callback and the control flow is resumed right after the Menu::displayMenu() method (or the Menu::LoopCheckSerial() method).
+ if its **false** : the menu is reloaded just after the callback to let the user select another menu option. 

#### form 1

the simplest, no parameters (hence, you need to define one function per menuitem).

```C++
bool menucallback();
```

#### form 2

the parameter menuname is the display of the selected menuitem (as seen by the end-user).

```C++
bool menucallback(const char *menuname);
```

#### form 3

+ the parameter menukey is the user-key of the selected menuitem (set in the Menu::addCallbackMenuitem method)
+ the parameter menuname is the label of the selected menuitem (as seen by the end-user).

```C++
bool menucallback(ushort menukey, const char *menuname);
```

### Menu items declaration methods

#### hierarchy menu-item

add an item heading to a submenu

```c++
ushort addHierarchyMenuitem(const char *menuname, ushort parentid);
```

with :

+ menuname : label of the menuitem (as seen by the end-user)
+ parentid : if this menuitem is under a submenu : id of the parent item
+ --> return value : id of the new menu-item created. To be used on the next submenu items as a reference to this item.

#### callback menu-item

add a menu item calling a given function.

```c++
ushort addCallbackMenuitem(const char *menuname, fp_callback menuFonction, ushort parentid);
```

with :

+ menuname : label of the menuitem (as seen by the end-user)
+ menuFonction : pointer to the function to call for this menu. this function can be on the form 1 or 2 described in the upper *callbacks section*. 
+ parentid : if this menuitem is under a submenu : id of the parent item
+ --> return value : id of the new menu-item created.

#### callback menu-item with user-key

add a menu item calling a given function, same as preceeding option with an additionnal user-key allowing the grouping of several menu-items on the same callback.

```c++
ushort addCallbackMenuitem(const char *menuname, fp_callback3 menuFonction, ushort parentid, ushort menukey);
```

with :

+ menuname : label of the menuitem (as seen by the end-user)
+ menuFonction : pointer to the function to call for this menu. this function must be on the form 3 described in the upper *callbacks section*. 
+ parentid : if this menuitem is under a submenu : id of the parent item
+ menukey : key to use for this menu (the unicity of this key is under your concern, an exception will be thrown if it's not unique). This key is passed back to the callback
+ --> return value : id of the new menu-item created.

#### callback menu-item with dynamic label

Add to the preceeding option a callback to define from your code the menu label given the updated context. The use of a user-key (menukey) is necessary for this feature.

```c++
ushort addDynamicCallbackMenuitem(fp_namingcallback menunamefunction, fp_callback3 menuFonction, ushort parentid, ushort menukey);
```

with :

+ menunamefunction : callback to a function provinding dynamically the menuitem name (possibly given the menukey)
+ menuFonction : pointer to the function to call for this menu. this function must be on the form 3 described in the upper *callbacks section*.
+ parentid : if this menuitem is under a submenu : id of the parent item
+ menukey : key to use for this menu (the unicity of this key is under your concern, an exception will be thrown if it's not unique). This key is passed back to the callback
+ --> return value : id of the new menu-item created.

#### Updater menu-item 

Provides a straight-forward method to prompt the user for an update on the content of a variable, without any callback.
A pointer to the target variable is given as parameter to this method.
The target variable can be of the following types: 
+ c-string (char*)
+ int
+ ushort (unsigned short)
+ double/float
+ bool (true value can be input as y or 1 and false : n or 0)

```c++
// for a string:
ushort addUpdaterMenuitem(const char *menuname, ushort parentid, char *variableToUpdate, size_t stringsize, ushort trials)

// for other supported types : 
ushort addUpdaterMenuitem(const char *menuname, ushort parentid, <TYPE> *variableToUpdate, ushort trials)
```

with : 

+ menuname : label of the menuitem (as seen by the end-user)
+ parentid : if this menuitem is under a submenu : id of the parent item
+ variableToUpdate : pointer to the variable to be updated
+ stringsize (for string only) : size max of the c-string (buffer of char)
+ trials : number of given trials before issuing a failure and exits the assignement loop.
+ --> return value : id of the new menu-item created.





## Licence

This code is released under the MIT License.