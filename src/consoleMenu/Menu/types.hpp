#pragma once

namespace CONSOLEMENU_NAMESPACE
{

/**
     * @brief callback called on menu selection. form without parameters.
     * 
     * @return if the return value is true : exit the menu after execution of this function,
     * else stay in the current menu and wait for another action
     */
typedef bool (*fp_callback1)(void);

/**
     * @brief callback called on menu selection
     * 
     * @param <menu name> will be the menuitem name
     * @return if the return value is true : exit the menu after execution of this function,
     * else stay in the current menu and wait for another action
     */
typedef bool (*fp_callback2)(const char *);

/**
     * @brief callback called on menu selection
     * 
     * @param <key> will be the menuitem key
     * @param <menu name> will be the menuitem name
     * @return if the return value is true : exit the menu after execution of this function,
     * else stay in the current menu and wait for another action
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
} MenuOptions;

enum menutype
{
     externalFunction = 0,
     exit = 1, //exit the menu
     back = 2, //back to the parent menuitem
     hierarchymenu = 3
};

} // namespace CONSOLEMENU_NAMESPACE