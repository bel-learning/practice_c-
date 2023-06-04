#ifndef STATE_H
#define STATE_H
enum MainState {
    MenuView, CalendarView, EventView
};

enum Menu {
    SMonthlyCalendar = 0,  SWeeklyCalendar = 1,
    SDailyCalendar = 2, SAddEvent = 3,
    SQuit = 4
};

#endif // STATE_H