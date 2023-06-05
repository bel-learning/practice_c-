#include <ncurses.h>
#include <iostream>
#include <iomanip>
#include <chrono>
#include <ctime>
#include "datetime.h"
#include "calendar.h"
#include "util.h"
#include "state.h"
#include "views/MenuView.h"
#include "views/CalendarView.h"
#include "views/EventView.h"
#include "views/ImportView.h"
#include "views/ExportView.h"
#include "views/SearchView.h"
#include "event.h"

using namespace std;

int main()
{
    Calendar *cal = nullptr;
    EventDictionary storage;
    // Controls App main stage
    MainState state;

    // int row = 20;
    // int col = 80;

    // Initializing NCURSES
    WINDOW *mainWin;
    if ((mainWin = initscr()) == NULL)
    {
        fprintf(stderr, "Error initialising ncurses.\n");
        exit(EXIT_FAILURE);
    }
    noecho();
    keypad(mainWin, TRUE);
    start_color();
    init_pair(1, COLOR_GREEN, COLOR_BLACK);
    init_pair(2, COLOR_RED, COLOR_BLACK);
    init_pair(3, COLOR_BLACK, COLOR_YELLOW);
    init_pair(4, COLOR_YELLOW, COLOR_BLACK);

    bool running = true;
    state = MainState::MenuView;
    int choice = 0;
    int res = 0;
    // Main APP LOOP
    while (running)
    {
        
        // MAIN APP STATES
        switch (state)
        {
        case MainState::MenuView:
            box(mainWin, 0, 0);
            choice = GetMenuView(mainWin, cal);
            switch (choice)
            {
            case Menu::SQuit:
                break;

            case Menu::SDailyCalendar:
            case Menu::SMonthlyCalendar:
            case Menu::SWeeklyCalendar:
                state = MainState::CalendarView;
                break;
            case Menu::SAddEvent:
                state = MainState::EventView;
                break;
            case Menu::SImportView:
                state = MainState::ImportView;
                break;
            case Menu::SExportView:
                state = MainState::ExportView;
                break;
            case Menu::SSearchView:
                state = MainState::SearchView;
                break;
            }
            break;
        case MainState::CalendarView:
            refresh();
            clear();
            box(mainWin, 0, 0);
            if (choice == Menu::SMonthlyCalendar)
                cal = new MonthlyCalendar();
            if (choice == Menu::SWeeklyCalendar)
                cal = new WeeklyCalendar();
            if (choice == Menu::SDailyCalendar)
                cal = new DailyCalendar();
            //
            cal->referDictionary(&storage);
            res = GetCalendarView(choice, mainWin, cal);
            state = MainState::MenuView;
            if (cal != nullptr)
                delete cal;
            cal = nullptr;
            break;

        case MainState::EventView:
            refresh();
            clear();
            box(mainWin, 0, 0);
            res = AddEventView(mainWin, cal, &storage);
            state = MainState::MenuView;
            break;

        case MainState::ImportView:
            refresh();
            clear();
            box(mainWin, 0, 0);
            res = GetImportView(mainWin, &storage);
            state = MainState::MenuView;
            break;

        case MainState::ExportView:
            refresh();
            clear();
            box(mainWin, 0, 0);
            res = GetExportView(mainWin, &storage);
            state = MainState::MenuView;
            break;
        case MainState::SearchView:
            refresh();
            clear();
            box(mainWin, 0, 0);
            res = GetSearchView(mainWin, &storage);
            state = MainState::MenuView;
            break;
        }

        if (choice == Menu::SQuit)
            break;
    }
    // writeToDebug(res);
    clear();
    delwin(mainWin);
    endwin();

    return 0;
}
