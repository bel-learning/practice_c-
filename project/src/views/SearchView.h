#ifndef SEARCHVIEW_H
#define SEARCHVIEW_H
#include <ncurses.h>
#include "../calendar.h"

/**
 * @file SearchView.h
 *
 * @brief Declaration of functions in SearchView
 */

/**
 * \brief Initializes search view and brings interactivity.
 *
 * This function allows the user to search for events by typing a search query in a window. The search is performed
 * by matching the input against event titles using the match() function. The user can navigate through the search
 * results and select an event to view its details. The function uses the ncurses library for displaying the user
 * interface.
 *
 * \param win Pointer to the ncurses window where the search interface is displayed.
 * \param storage Pointer to the EventDictionary object containing the events to search through.
 *
 * \return An integer representing the state of the program after the search:
 *         - 0: The search operation completed successfully.
 *         - Any other value: An error or a specific state as defined by the program.
 */
int GetSearchView(WINDOW *main, EventDictionary *storage);

#endif // SEARCHVIEW_H
