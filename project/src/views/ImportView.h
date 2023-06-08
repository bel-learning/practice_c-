#ifndef IMPORTVIEW_H
#define IMPORTVIEW_H

#include <ncurses.h>
#include "../calendar.h"

/**
 * @file ImportView.h
 *
 * @brief Declaration of functions in ImportView
 */

/**
 * \brief Displays the import view and allows the user to import events from a file.
 *
 * This function presents the import view to the user, where they can enter the filename of the file containing the
 * events to be imported. The function reads the file, parses each line to create event objects, and adds them to the
 * EventDictionary for storage. The function uses the ncurses library for displaying the user interface.
 *
 * \param win Pointer to the ncurses window where the import view is displayed.
 * \param storage Pointer to the EventDictionary object where the imported events will be stored.
 *
 * \return An integer representing the state of the program after importing events:
 *         - 1: The import operation was canceled through exit.
 *         - 0: The import operation finished.
 */
int GetImportView(WINDOW *win, EventDictionary *storage);

#endif // IMPORTVIEW_H
