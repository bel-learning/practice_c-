#include <ncurses.h>
#include <iostream>  
#include <iomanip>
#include <chrono>
#include <ctime>
#include "datetime.h"
#include "calendar.h"
#include "util.h"
#include "window.h"
#include "state.h"
#include "views/MenuView.h"
#include "views/CalendarView.h"
#include "views/EventView.h"
#include "views/ImportView.h"
#include "views/ExportView.h"
#include "views/SearchView.h"
#include "event.h"

using namespace std;

void addDummyData(EventDictionary * storage) {
    Datetime start = getCurrentDateTime();
    Datetime end = getCurrentDateTime();
    start.hour = 12;
    end.hour = 13;

    storage->addRepeatEvent(new NormalEvent("Meeting1", "...", Event::Repeat::Monthly, "nowhere", start, end));

    start.hour = 13;
    end.hour = 14;
    storage->addRepeatEvent(new NormalEvent("Study sap", "...", Event::Repeat::Monthly, "nowhere", start, end));

    start.day = 5;
    start.hour = 14;
    end.day = 5;
    end.hour = 15;
    storage->addRepeatEvent(new NormalEvent("Study pa2", "...", Event::Repeat::Weekly, "nowhere", start, end));

    start.day = 4;
    start.hour = 20;
    end.day = 4;
    end.hour = 21;
    storage->addEvent(new Task("Finish pa2", "...", Event::Repeat::None,  start, end, true));

    start.day = 4;
    start.hour = 2;
    end.day = 4;
    end.hour = 2;
    storage->addEvent(new Task("Finish la2", "...", Event::Repeat::None,  start, end, false));

 
    end.day = 4;
    end.hour = 23;
    storage->addEvent(new Deadline("Complete project", "...", Event::Repeat::None,  end));

}

int main() {
    Calendar * cal = nullptr;
    EventDictionary storage;
    addDummyData(&storage);
    MainState state;
    // int row = 20;
    // int col = 80;
    WINDOW * mainWin;
    if ( (mainWin = initscr()) == NULL ) {
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
    // mvwprintw(mainWin, 1,1, "hello");
    while(running) {
        mvwprintw(mainWin, 1,1, "state - %d", state);
        mvwprintw(mainWin, 2,1, "choice - %d", choice);
        refresh();
        switch(state) {
            case MainState::MenuView:
                box(mainWin, 0,0);
                choice = GetMenuView(mainWin, cal);
                switch(choice) {
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
                box(mainWin, 0,0);
                if(choice == Menu::SMonthlyCalendar)
                    cal = new MonthlyCalendar();
                if(choice == Menu::SWeeklyCalendar)
                    cal = new WeeklyCalendar();
                if(choice == Menu::SDailyCalendar)
                    cal = new DailyCalendar();
                // 
                cal->referDictionary(&storage);
                res = GetCalendarView(choice, mainWin, cal);
                state = MainState::MenuView;
                if(cal != nullptr)
                    delete cal;
                cal = nullptr;
                break;

            case MainState::EventView:
                refresh();
                clear();
                box(mainWin, 0,0);
                res = AddEventView(mainWin, cal, &storage);
                state = MainState::MenuView;
                break;

            case MainState::ImportView:
                refresh();
                clear();
                box(mainWin, 0,0);
                res = GetImportView(mainWin, &storage);
                state = MainState::MenuView;
                break;
            
            case MainState::ExportView:
                refresh();
                clear();
                box(mainWin, 0,0);
                res = GetExportView(mainWin, &storage);
                state = MainState::MenuView;
                break;
            case MainState::SearchView:
                refresh();
                clear();
                box(mainWin, 0,0);
                res = GetSearchView(mainWin, &storage);
                state = MainState::MenuView;
                break;
        }

        if(choice == Menu::SQuit)
            break;
    }
    // writeToDebug(res);
    clear();
    delwin(mainWin);
    endwin();

    return 0;
}

