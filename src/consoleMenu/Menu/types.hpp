#pragma once

namespace CONSOLEMENU_NAMESPACE
{

/**
     * @brief callback called on menu selection.
     * 
     * @param callback param will be the menuitem name
     * @return if the return value is true : exit the menu after execution of this function,
     * else stay in the current menu and wait for another action
     */
typedef bool (*pf_callback)(const char *);
typedef void (*pf_IOdisplay)(const char *);
typedef const char *(*pf_IOinput)(void);

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