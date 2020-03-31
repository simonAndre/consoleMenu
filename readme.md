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
```c
    //define options
    MenuOptions menuoptions;
    menuoptions.addBack = true;
    menuoptions.addExitForEachLevel = true;

    // declaration,
    // DisplayInfos: IO callback to render the menu
    // WaitforInput: IO callback to wait and read the user input
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
### callbacks
simple function based on this prototype. :

```c
bool menu1(const char *menuname);
```
+ parameters :

  + menuname : label of the menuitem bind o nthis callback during the declaration.

+ return value :  if the return value from the callback is true, the menu is exited after execution of this function. Otherwise, we stay in the current menu and wait for another action.

###  menu call
simply use displayMenu();

```c
m.displayMenu();
```
this call will exit when the user make his choice and the callback give the control back to the main program flow. 