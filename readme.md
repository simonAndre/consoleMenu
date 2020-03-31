# consoleMenu, a simple console UI hierarchy menu

target : any environment : embedded, linux or windows.
mainly done for embedded, its main purpose is to be simple and very lightweight with a small memory footprint.

work on a callback basis : each call to a memnu item is link to a simple callback function.

## build, test

+ build : use the Makefile.
  + linux : `make consoleMenu`
+ test : 
  + build tu test via the makefile : `make test`
  + run the test : `./test.exe`


## usage

example of the definition of a 3 level hierarchical menu:
+ setup : 
```c
MenuOptions menuoptions;
menuoptions.addBack = true;
menuoptions.addBack = false;
consoleMenu m(DisplayInfos, WaitInput, menuoptions);
ushort menu1id = m.addMenuitem("menu 1", menu1, 0);
ushort menu2id = m.addMenuitem("menu 2", menu2, 0);
ushort menu3id = m.addMenuitem("menu 3", NULL, 0);
m.addMenuitem("menu 3-1", menu1, menu3id);
m.addMenuitem("menu 3-2", menu2, menu3id);
ushort menu33id = m.addMenuitem("menu 3-3", NULL, menu3id);
m.addMenuitem("menu 3-3-1", menu1, menu33id);
m.addMenuitem("menu 3-3-2", menu2, menu33id);
```
+ callbacks : in the upper exemple, we use 2 functions for the callback, like this :

```c
bool menu1()
{
    cout << "menu1 called\n";
    return true;
}
```

+ menu call : simply use displayMenu();

```c
m.displayMenu();
```
this call will exit when the user make his choice and the callback give the control back to the main program flow. 