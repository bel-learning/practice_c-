#ifndef MENUVIEW_H
#define MENUVIEW_H
#include <ncurses.h>

/**
 * @file MenuView.h
 *
 * @brief Declaration of GetMenuView function
 */

/**
 * @brief A function that handles the printing and interactive logic for the main menu
 *
 * @param main The main window object to display the menu options
 * @param cal The Calendar object to perform actions on (polymorphism)
 * @return An integer representing the status of the function execution
 */
int GetMenuView(WINDOW *main, Calendar *cal);

#endif // MENUVIEW_H
