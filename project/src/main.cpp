#include <ncurses.h>
#include <curses.h>
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

using namespace std;



int main() {
    Calendar * cal = nullptr;

    MainState state;
    int row = 20;
    int col = 80;
    WINDOW * mainWin;
    if ( (mainWin = initscr()) == NULL ) {
        fprintf(stderr, "Error initialising ncurses.\n");
        exit(EXIT_FAILURE);
    }
    noecho();
    keypad(mainWin, TRUE);

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
                
                if (choice == Menu::SQuit)
                    break;

                if (choice == Menu::SDailyCalendar || 
                    choice == Menu::SMonthlyCalendar ||
                    choice == Menu::SWeeklyCalendar
                ) {
                    state = MainState::CalendarView;
                    break;
                }
                if (choice == Menu::SAddEvent) {
                    state = MainState::EventView;
                    break;
                }
                break;
            case MainState::CalendarView:
                refresh();
                clear();
                box(mainWin, 0,0);
                res = GetCalendarView(choice, mainWin, cal);
                state = MainState::MenuView;
                break;

            case MainState::EventView:
                refresh();
                clear();
                box(mainWin, 0,0);
                res = AddEventView(mainWin, cal);
                state = MainState::MenuView;
                break;

        }

        if(choice == Menu::SQuit)
            break;
    }
    clear();
    delwin(mainWin);
    endwin();

    return 0;
}

