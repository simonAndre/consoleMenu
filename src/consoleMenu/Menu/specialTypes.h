#pragma once
#include "../commontypes.h"

namespace CONSOLEMENU_NAMESPACE
{

/**
* @brief callback called on menu selection. form without parameters.
* 
* @return false if an error occured or just to break the callbacks chain. If False, in case of chained callbacks, further callbacks won't be called
*/
typedef bool (*fp_callback1)(void);

/**
* @brief callback called on menu selection
* 
* @param <menu name> will be the menuitem name
* @return false if an error occured or just to break the callbacks chain. If False, in case of chained callbacks, further callbacks won't be called

     */
typedef bool (*fp_callback2)(const char *);

/**
* @brief callback called on menu selection
* 
* @param <key> will be the menuitem key
* @param <menu name> will be the menuitem name
* @return false if an error occured or just to break the callbacks chain. If False, in case of chained callbacks, further callbacks won't be called
     */
typedef bool (*fp_callback3)(ushort, const char *);
typedef void (*fp_IOdisplay)(const char *);

typedef const char *(*fp_IOinput)(void);
/**
 * @brief prototype of pointer to a function aiming at listen to an IO input for an id. 
 * 
 */
typedef ushort (*fp_IOinputId)(void);
typedef const char *fp_namingcallback(ushort menukey);
typedef struct
{
     bool addBack = true;
     bool addExitForEachLevel = true; //if false and addBack is true : the exist menuitem is only available at the root
     const char *id_separator = " - ";
     bool addCurrentState = true; // if true : display the current state for each menuitem, for updater menu items only.
     ushort badInputRepeats = 1;  // nb of allowed trials for bad input before failing the input of a menu.
     ushort expirationTimeSec = CONSOLEMENU_MENU_DEFAULTMENUSELECTIONTIMEOUT;// time after when the menu is automatically exited without any user interaction.
     bool breakCallbackChainOnFirstError = true;  //break the callback chain : no further execution of callbacks and get back to the menu display (if previous callback where declared, they have already completed).
} MenuOptions;

typedef struct{
     bool callbacksSuccessfull=true;    // action completed successfully
     bool exitRequested=false;           // one of the callback require to exit of the menu
} SelectActionResult;

enum menutype
{
     rootmenu, //only one root menu
     externalFunction,
     exit, //exit the menu
     back, //back to the parent menuitem
     hierarchymenu,

     //value from 50 to 60 are reserved for variableUpdaters
     variableUpdater_i = 50,  //variable updater int
     variableUpdater_us = 51, //variable updater ushort
     variableUpdater_d = 52,  //variable double
     variableUpdater_s = 53,  //variable string
     variableUpdater_b = 54,  //variable bool
     variableUpdater_uc = 55  //variable unsigned char
};

} // namespace CONSOLEMENU_NAMESPACE