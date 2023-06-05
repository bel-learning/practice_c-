#ifndef EVENTVIEW_H
#define EVENTVIEW_H
#include <ncurses.h>
#include "../event.h"
#include "../calendar.h"

/**
 * @file EventView.h
 *
 * @brief Declaration of AddEventView and GetEventView.
 */

/**
 * @brief A function that deals with printing and interactive logic for adding events
 *
 *
 * @param main The main window object to display input prompts
 * @param cal The Calendar that has polymorphic structure
 * @param storage The EventDictionary object for event storage
 * @return An integer representing the status of the function execution
 */
int AddEventView(WINDOW *main, Calendar *cal, EventDictionary *storage);

/**
 * @brief A function that deals with printing and interactive logic for viewing events
 *
 * @param main The main window object to display the events
 * @param storage The EventDictionary object containing the events
 * @param event The Event object representing the event to be viewed
 * @return An integer representing the status of the function execution
 */
int GetEventView(WINDOW *main, EventDictionary *storage, const Event *event);
// Just a helper function used in moving events
vector<Interval> findFreeIntervals(const Event *event, EventDictionary *storage);
#endif // EVENTVIEW_H