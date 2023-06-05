#include "../state.h"
#include "../calendar.h"
#include "../util.h"



int triggerMonthlyView(WINDOW * lWindow, WINDOW * rWindow,  Calendar * cal) {
    Datetime showDate = getCurrentDateTime();
    
    int selected = 0;
    int monthDay = getDaysInMonth(showDate);
    int ch = 0;
    vector<Event *> events;

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
        bool showAll = true;
        if (selected)
            showAll = false;

        for(Event * event : events)
            delete event;

        events = cal->getEvents(showDate, showAll);
        cal->displayEvents(rWindow, events ,showAll);

        wrefresh(lWindow);
        wrefresh(rWindow);

        ch = getch();

        if(ch == 'q')
            break;
    }
    
    for(Event * event : events)
        delete event;
    wclear(lWindow);
    wclear(rWindow);
    delwin(lWindow);
    delwin(rWindow);
    return 1;
}

int triggerWeeklyView(WINDOW * lWindow, WINDOW * rWindow,  Calendar * cal) {
    wclear(lWindow);
    wclear(rWindow);

    Datetime showDate = getCurrentDateTime();
    showDate = startOfCurrentWeek(showDate);
    int selected = 0;
    int ch = 0;
    vector<Event *> events;
    while(true) {
        wrefresh(rWindow);
        wclear(lWindow);
        wclear(rWindow);
        
        if(selected == 0) {
            if(ch == KEY_LEFT || ch == KEY_SLEFT || ch == 260) {
                showDate = startOfPreviosWeek(showDate);
            }
            if(ch == KEY_RIGHT || ch == KEY_SRIGHT || ch == 261) {
                showDate = startOfNextWeek(showDate);
            }
        }
       
        if(ch == KEY_DOWN) {
            selected++;
            if(selected > 7)
                selected = 0;
            Datetime startWeek = startOfCurrentWeek(showDate);
            for(int i = 0; i < selected - 1; i++) {
                startWeek = findTomorrow(startWeek);
            }
            showDate = startWeek;
        }
        if(ch == KEY_UP) {
            selected--;
            if(selected < 0)
                selected = 7;
            Datetime startWeek = startOfCurrentWeek(showDate);
            for(int i = 0; i < selected - 1; i++) {
                startWeek = findTomorrow(startWeek);
            }
            showDate = startWeek;

        }

        
        cal->print(lWindow, showDate, selected);
        bool showAll = true;
        if (selected)
            showAll = false;

        for(Event * event : events)
            delete event;

        events = cal->getEvents(showDate, showAll);
        cal->displayEvents(rWindow, events ,showAll);

        wrefresh(lWindow);
        wrefresh(rWindow);

        ch = getch();

        if(ch == 'q') {
            break;
        }
        
    }
    for(Event * event : events)
        delete event;
    wclear(lWindow);
    wclear(rWindow);
    delwin(lWindow);
    delwin(rWindow);
    return 1;
}
int triggerDailyView(WINDOW * lWindow, WINDOW * rWindow,  Calendar * cal) {
    wclear(lWindow);
    wclear(rWindow);
    
    Datetime showDate = getCurrentDateTime();
    int ch = 0;
    vector<Event *> events;
    while(true) {
        wrefresh(rWindow);
        wclear(lWindow);
        wclear(rWindow);
        
        if(ch == KEY_LEFT || ch == KEY_SLEFT || ch == 260) {
            showDate = findYesterday(showDate);
        }
        if(ch == KEY_RIGHT || ch == KEY_SRIGHT || ch == 261) {
            showDate = findTomorrow(showDate);
        }
        // if(ch == KEY_DOWN) {
        //     selected++;
        //     if(selected > 7)
        //         selected = 0;
        // }
        // if(ch == KEY_UP) {
        //     selected--;
        //     if(selected < 0)
        //         selected = 7;
        // }
        
        cal->print(lWindow, showDate, 0);
        // wmove()
        for (Event * event : events)
            delete event;

        events = cal->getEvents(showDate, false);
       
        cal->displayEvents(rWindow, events);

        wrefresh(lWindow);
        wrefresh(rWindow);

        ch = getch();

        if(ch == 'q') {
            break;
        }
        
    }
    for (Event * event : events)
            delete event;
    wclear(lWindow);
    wclear(rWindow);
    delwin(lWindow);
    delwin(rWindow);
    return 1;
}

int GetCalendarView(int type, WINDOW * main, Calendar * cal) {
    int MAX_ROWS, MAX_COLS;
    getmaxyx(main, MAX_ROWS, MAX_COLS);
    
    // Dividing the screen in half. One for calendar, 1 for days
    WINDOW * lWindow = subwin(main, MAX_ROWS - 3, MAX_COLS/2 - 2, 1,1);
    WINDOW * rWindow = subwin(main, MAX_ROWS - 3, MAX_COLS/2 - 4, 1,MAX_COLS/2+2);
   

    if (type == Menu::SMonthlyCalendar) {
        
        int state = triggerMonthlyView(lWindow, rWindow, cal);
        return state;
    }

    if (type == Menu::SWeeklyCalendar) {
        int state = triggerWeeklyView(lWindow, rWindow, cal);
        return state;
    }

    if (type == Menu::SDailyCalendar) {
        int state = triggerDailyView(lWindow, rWindow, cal);
        return state;
    }

    return 0;
    
}