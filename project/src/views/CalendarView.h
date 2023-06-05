#ifndef CALENDARVIEW_H
#define CALENDARVIEW_H
#include <ncurses.h>
#include "../calendar.h"

/**
 * @file CalendarView.h
 *
 * @brief Declaration of functions in CalendarView
 */

/**
 * @brief A function that deals with printing and interactive logic of Calendar Views
 *
 * @param type The type of view to display (e.g., Monthly, Weekly, Daily)
 * @param main The main window object to output the view
 * @param cal The Calendar object containing the polymorphic structure for the view
 * @return An integer representing the status of the function executi
 */
int GetCalendarView(int type, WINDOW *main, Calendar *cal);

#endif // CALENDARVIEW_H