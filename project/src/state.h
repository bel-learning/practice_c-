#ifndef STATE_H
#define STATE_H
enum MainState {
    MenuView, CalendarView, EventView, ImportView, ExportView, SearchView
};

enum Menu {
    SMonthlyCalendar = 0,  SWeeklyCalendar = 1,
    SDailyCalendar = 2, SAddEvent = 3, 
    SImportView = 4, 
    SExportView = 5,
    SSearchView = 6,
    SQuit = 7, 
};

#endif // STATE_H