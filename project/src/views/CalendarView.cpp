#include "../state.h"
#include "../calendar.h"
#include "../window.h"
#include "../util.h"

void DisplayEvents(WINDOW * win, const vector<Event *> & events) {
    wclear(win);
    int middle = getmaxx(win) / 2;
    wattron(win, A_BOLD);
    mvwprintw(win, 2, middle-6, "Events\n\n");
    wattroff(win, A_BOLD);

    if(events.size() == 0) {
        wprintw(win, "No events\n");
    }
    for(const Event * e : events) {
        wprintw(win, "%s\n", e->toString().c_str());
        // wprintw(win, "%s\n", e->ge);
    }
    wrefresh(win);
}

int DisplayMonthly(WINDOW * lWindow, WINDOW * rWindow,  Calendar * cal, int MAX_ROWS, int MAX_COLS) {
    Datetime showDate = getCurrentDateTime();
    
    int selected = 0;
    int monthDay = getDaysInMonth(showDate);
    int ch;
     while(true) {
        wrefresh(rWindow);
        wclear(lWindow);
        wclear(rWindow);

        if (selected == 0) {
            if(ch == KEY_LEFT || ch == KEY_SLEFT || ch == 260) {
                showDate = decreaseMonth(showDate);
                monthDay = getDaysInMonth(showDate);
            }
            if(ch == KEY_RIGHT || ch == KEY_SRIGHT || ch == 261) {
                showDate = increaseMonth(showDate);
                monthDay = getDaysInMonth(showDate);
            }
            if(ch == KEY_DOWN) {
                selected++;
                showDate.day = selected;
            }
            if(ch == KEY_UP) {
                selected = monthDay;
            }
        }
        else if (selected != 0) {
            switch(ch) {
                case KEY_LEFT: 
                case KEY_SLEFT:
                    selected--;
                    break;
                
                case KEY_RIGHT:
                case KEY_SRIGHT:
                    selected++;
                    if(selected > monthDay)
                        selected = 1;
                    break;
                
                case KEY_DOWN:
                    if (selected > monthDay - 7)
                        selected = 0;
                    else 
                        selected += 7;
                    break;

                case KEY_UP:
                    if (selected < 7)
                        selected = 0;
                    else
                        selected -= 7;
                    break;
            }
            if(selected)
                showDate.day = selected;
        }

        cal->print(lWindow, showDate, selected);
        // wrefresh()
        // mvwprintw(rWindow, 2, rCols/2 - 8, "Events");
        if (selected)
            DisplayEvents(rWindow, cal->getEvents(showDate));

        wrefresh(lWindow);
        wrefresh(rWindow);

        ch = getch();

        if(ch == 'q') {
            wclear(lWindow);
            wclear(rWindow);
            delwin(lWindow);
            delwin(rWindow);
            return 1;
        }
        
    }
}

int DisplayWeekly(WINDOW * lWindow, WINDOW * rWindow,  Calendar * cal, int MAX_ROWS, int MAX_COLS) {

}

int GetCalendarView(int type, WINDOW * main, Calendar * cal) {
    int MAX_ROWS, MAX_COLS;
    getmaxyx(main, MAX_ROWS, MAX_COLS);
    
    WINDOW * lWindow = subwin(main, MAX_ROWS - 3, MAX_COLS/2 - 2, 1,1);
    WINDOW * rWindow = subwin(main, MAX_ROWS - 3, MAX_COLS/2 - 4, 1,MAX_COLS/2+2);
    

    int lRows = MAX_ROWS - 3;
    int lCols = MAX_COLS/2 - 2;

    int rRows = MAX_ROWS - 3;
    int rCols = MAX_COLS/2 - 4;

   

    if (type == Menu::SMonthlyCalendar) {
        
        int state = DisplayMonthly(lWindow, rWindow, cal, MAX_ROWS, MAX_COLS);
        return state;
    }

    if (type == Menu::SWeeklyCalendar) {
        mvwprintw(lWindow,2,4, "Weekly Calendar");
        wrefresh(lWindow);

        while(true) {
            int ch = getch();
            if(ch == 'q') {
                wclear(lWindow);
                delwin(lWindow);

                return 1;
            }
        }
    }

    if (type == Menu::SDailyCalendar) {
        wrefresh(lWindow);

        mvwprintw(lWindow,2,4, "Daily Calendar");
        while(true) {
            int ch = getch();
            if(ch == 'q') {
                wclear(lWindow);
                delwin(lWindow);
                return 1;
            }
        }
    }

    
}