#ifndef STATE_H
#define STATE_H

/**
 * @file state.h
 *
 * @brief Enumeration of the main states in the application
 */

enum MainState {
    MenuView, /**< The state for the menu view. */
    CalendarView, /**< The state for the calendar view. */
    EventView, /**< The state for the event view. */
    ImportView, /**< The state for the import view. */
    ExportView, /**< The state for the export view. */
    SearchView /**< The state for the search view. */
};

/**
 * @brief Enumeration of the menu options in the application.
 */
enum Menu {
    SMonthlyCalendar = 0, /**< The option for the monthly calendar. */
    SWeeklyCalendar = 1, /**< The option for the weekly calendar. */
    SDailyCalendar = 2, /**< The option for the daily calendar. */
    SAddEvent = 3, /**< The option for adding an event. */
    SImportView = 4, /**< The option for the import view. */
    SExportView = 5, /**< The option for the export view. */
    SSearchView = 6, /**< The option for the search view. */
    SQuit = 7 /**< The option for quitting the application. */
};

#endif // STATE_H