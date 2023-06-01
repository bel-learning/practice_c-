#include "../state.h"
#include "../calendar.h"
#include "../window.h"
#include "../util.h"


int DisplayMonthly(WINDOW * lWindow, WINDOW * rWindow,  Calendar * cal, int MAX_ROWS, int MAX_COLS) {
    Datetime showDate = getCurrentDateTime();

    int selected = 0;
    int monthDay = getDaysInMonth(showDate);
    int ch;
     while(true) {
        string pr = to_string(showDate.month);
        const char * num1 = pr.c_str();
        mvwprintw(rWindow, 2, 2, num1);
        wrefresh(rWindow);
        wclear(lWindow);

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
        }

        
        cal->print(lWindow, showDate, selected);
        // mvwprintw(rWindow, 2, rCols/2 - 8, "Events");
        wrefresh(lWindow);
        wrefresh(rWindow);
        wclear(rWindow);
        ch = getch();
        string str = to_string(ch);
        const char * num = str.c_str();
        mvwprintw(rWindow, 2,2, num);
        string selectedStr = to_string(selected);
        const char * num2 = selectedStr.c_str();
        mvwprintw(rWindow, 4,4, num2);  
        
        wrefresh(rWindow);
        if(ch == 'q') {
            wclear(lWindow);
            wclear(rWindow);
            delwin(lWindow);
            delwin(rWindow);

            return 1;
        }
       
        
        
    }
}

int GetCalendarView(int type, WINDOW * main, Calendar * cal) {
    int MAX_ROWS, MAX_COLS;
    getmaxyx(main, MAX_ROWS, MAX_COLS);
    
    WINDOW * lWindow = subwin(main, MAX_ROWS - 3, MAX_COLS/2 - 2, 1,1);
    WINDOW * rWindow = subwin(main, MAX_ROWS - 3, MAX_COLS/2 - 4, 1,MAX_COLS/2+2);
    
    int rows, cols;
 

    int lRows = MAX_ROWS - 3;
    int lCols = MAX_COLS/2 - 2;

    int rRows = MAX_ROWS - 3;
    int rCols = MAX_COLS/2 - 4;

   

    if (type == Menu::SMonthlyCalendar) {
        
        cal = new MonthlyCalendar();
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