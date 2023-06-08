#ifndef EXPORTVIEW_H
#define EXPORTVIEW_H
#include <ncurses.h>
#include "../calendar.h"

/**
 * @file ExportView.h
 *
 * @brief Declaration of functions in ExportView
 */

/**
 * \brief Displays the export view and allows the user to export events to a file.
 *
 * This function presents the export view to the user, where they can enter a filename to export the events to. The
 * exported events are obtained from the EventDictionary and saved to the specified file. The function uses the ncurses
 * library for displaying the user interface.
 *
 * \param win Pointer to the ncurses window where the export view is displayed.
 * \param storage Pointer to the EventDictionary object containing the events to export.
 *
 * \return An integer representing the state of the program after the export:
 *         - 0: The export operation finished.
 *         - 1: The export operation was forced to exit.
 */
int GetExportView(WINDOW *win, EventDictionary *storage);

#endif // EXPORTVIEW_H
