# consoleMenu, a simple console UI hierarchy menu

(c) Simon ANDRE, 2019

target : any environment : embedded, linux or windows.
mainly done for embedded, its main purpose is to be simple and very lightweight with a small memory footprint.

work on a callback basis : each call to a memnu item is link to a simple callback function.



## build, test

+ build : use the Makefile.
  + linux : `make all`
  + windows : `mingw32-make all`
+ test : 
  + build tu test via the makefile : `make test`
  + run the test : `./test.exe`


## usage

example of the definition of a 3 level hierarchical menu:
### setup : 

#### the easy way 
```C++
#include <consoleMenu.h>
Menu consolemenu = Menu();
   // menus & submenus definition
    // root menus
    m.addMenuitem("action 1", menu1, 0);
    m.addMenuitem("action 2", menu2, 0);
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
    consoleMenu m(DisplayInfos, WaitforInput, menuoptions);

    // menus & submenus definition
    // root menus
    m.addMenuitem("action 1", menu1, 0);
    m.addMenuitem("action 2", menu2, 0);
    ushort menu3id = m.addMenuitem("sous-menu 3", NULL, 0);
    // level 2 menus, under the item [menu3id]
    m.addMenuitem("init string and stay", initStringValue, menu3id);
    m.addMenuitem("display string and stay", DisplayStringValue, menu3id);
    m.addMenuitem("menu init int and stop", initIntValue, menu3id);

    // more levels can be chained...
    ushort menu33id = m.addMenuitem("sous-menu 3-3", NULL, menu3id);
    m.addMenuitem("menu 3-3-1", menu1, menu33id);
    m.addMenuitem("menu 3-3-2", menu2, menu33id);

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
simple function based on this prototype. :

```C++
bool menu1(const char *menuname);
```
+ parameters :

  + menuname : label of the menuitem bind o nthis callback during the declaration.

+ return value :  if the return value from the callback is true, the menu is exited after execution of this function. Otherwise, we stay in the current menu and wait for another action.


## Licence

This code is released under the MIT License.