#include<ncurses.h>
#include<string>
#include "../event.h"
#include "../util.h"
#include "../calendar.h"
#include "EventView.h"

using namespace std;

// Event * createNewEven


int AddEventView(WINDOW * main, Calendar * cal, EventDictionary * storage) {
    int MAX_ROWS, MAX_COLS;
    getmaxyx(main, MAX_ROWS, MAX_COLS);
    wattron(main, A_UNDERLINE);
    mvwprintw(main, MAX_ROWS-2, MAX_COLS-24, "WRITE EXIT TO CANCEL:");
    wattroff(main, A_UNDERLINE);
    Event * event = nullptr;

    WINDOW * bWindow = subwin(main, MAX_ROWS - 4, MAX_COLS - 4, 1, 2);
    refresh();
    string type = promptInput(bWindow, "Type of event (deadline, task, event):");
    if(type == "exit") {
        clear();
        delwin(bWindow);
        return 0;
    }
    string title = promptInput(bWindow, "The title:");
    if(title == "exit") {
        clear();
        delwin(bWindow);

        return 0;
    }
    string description = promptInput(bWindow, "Description:");
    if(description == "exit") {
        clear();
        delwin(bWindow);

        return 0;
    }
    
    string repeatFormatted = promptInput(bWindow, "Repeat (daily, weekly, biweekly, monthly, yearly, or none):");
     if(repeatFormatted == "exit") {
        clear();
        delwin(bWindow);
        return 0;
    }
    Event::Repeat repeat = stringToRepeat(repeatFormatted);
    
    if(type == "event" || type == "") {
        string location = promptInput(bWindow, "Location:");
        if(location == "exit") {
            clear();
            delwin(bWindow);
            return 0;
        }
        string timeStart = promptInput(bWindow, "Start time (YYYY-MM-DD-hh-mm), (MM-DD-hh-mm), (DD-hh-mm), (hh-mm):");
        if(timeStart == "exit") {
            clear();
            delwin(bWindow);
            return 0;
        }
        string duration = promptInput(bWindow, "Duration (30, in mins):");
        if(duration == "exit") {
            clear();
            delwin(bWindow);
            return 0;
        }
        Datetime start(timeStart);
        event = new NormalEvent(title, description, repeat, location, start, calculateEndtime(start, static_cast<long long>(stoi(duration))));

    }

    if(type == "task") {
        string timeStart = promptInput(bWindow, "Start time (YYYY-MM-DD-hh-mm), (MM-DD-hh-mm), (DD-hh-mm), (hh-mm):");
        if(timeStart == "exit") {
            clear();
            delwin(bWindow);
            return 0;
        }
        string duration = promptInput(bWindow, "Duration (30, in mins):");
        if(duration == "exit") {
            clear();
            delwin(bWindow);
            return 0;
        }

        string finished;
        do {
            finished = promptInput(bWindow, "Finished or not (1 or 0)*:");
        }
        while(finished.empty());
        if(finished == "exit") {
            clear();
            delwin(bWindow);
            return 0;
        }
        bool finishedBool = finished[0] == '1' ? true : false;
        Datetime start(timeStart);
        event = new Task(title, description, repeat, start, calculateEndtime(start, static_cast<long long>(stoi(duration))), finishedBool);
    }
    if(type == "deadline") {
        string timeEnd = promptInput(bWindow, "End time (YYYY-MM-DD-hh-mm), (MM-DD-hh-mm), (DD-hh-mm), (hh-mm):");
        if(timeEnd == "exit") {
            clear();
            delwin(bWindow);
            return 0;
        }
        event = new Deadline(title, description, repeat, timeEnd);
    }

    if(repeat != Event::Repeat::None)
        storage->addRepeatEvent(event);
    else 
        storage->addEvent(event);
    delwin(bWindow);
    return 1;
}

void drawButton(const std::string& label,int y, int x,  bool selected, WINDOW * win) {
    if (selected) {
        wattron(win, A_REVERSE);   // Highlight the button if selected
    }
  
    mvwprintw(win, y,x,"[%s]\n", label.c_str());
    wattroff(win,A_REVERSE);  // Turn off highlighting
    
    wrefresh(win);

}

Interval chooseInterval(WINDOW * win, EventDictionary * storage, const Event * event, const vector<Interval> & intls) {
    int selected = 0;
    for(size_t i = 0; i < intls.size(); i++) {
        string s;
        s.append(to_string(intls[i].start.hour) + ":" + to_string(intls[i].start.minute) + "-");
        s.append(to_string(intls[i].end.hour) + ":" + to_string(intls[i].end.minute));

        drawButton(s, i+2, 2, selected == i, win);
    }
    int ch;
    // ESC KEY
    while((ch = getch()) != 27) {
        switch(ch) {
            case KEY_DOWN:
                selected++;
                if(selected > intls.size()-1)
                    selected = 0;
                break;
            case KEY_UP:
                selected--;
                if(selected < 0)
                    selected = intls.size()-1;
                break;
            case KEY_ENTER:
            case 10:
                // Do something
                return intls[selected];
                break;
        }
        for(size_t i = 0; i < intls.size(); i++) {
            string s;
            s.append(to_string(intls[i].start.hour) + ":" + to_string(intls[i].start.minute) + "-");
            s.append(to_string(intls[i].end.hour) + ":" + to_string(intls[i].end.minute));

            drawButton(s, i+2, 2, selected == i, win);
        }
        // drawButton(to_string(ch).c_str(), 10+2, 2, selected == 12, win);

    }
    // Interval;
    Interval defIntl;
    defIntl.start = Datetime(0,0,0,0,0,0);
    return defIntl;
}

int GetEventView(WINDOW * win, EventDictionary * storage, const Event * event) {
    wclear(win);
    box(win,0,0);
    int MAX_ROWS, MAX_COLS;
    getmaxyx(win, MAX_ROWS, MAX_COLS);

    wattron(win, A_BOLD);
    mvwprintw(win, 2, MAX_COLS/2 - 6, "Event View");
    wattroff(win, A_BOLD);

    WINDOW * lWin = subwin(win, MAX_ROWS -5 , MAX_COLS/2 - 2, 4, 2);
    WINDOW * rWin = subwin(win, MAX_ROWS -5 , MAX_COLS/2 - 2, 4, MAX_COLS/2);
   

    wrefresh(rWin);
    wrefresh(lWin);

    event->renderFormatted(lWin);

    int ch;
    int selected = 0;
    int currentRow, currentCol;
    getyx(lWin, currentRow, currentCol);

    // Initializing
    mvwprintw(lWin,currentRow+1, 0, "\nOptions:");
    drawButton("EDIT",currentRow+2,0 , selected == 0, lWin);
    drawButton("MOVE EVENT",currentRow+3, 0, selected == 1, lWin);
    drawButton("DELETE",currentRow+4, 0, selected == 2, lWin);
    wrefresh(lWin);

    // While not ESC
    while ((ch = getch()) != 27) {
        switch(ch) {
            case KEY_DOWN:
                selected++;
                if(selected>2)
                    selected = 0;
                break;
            case KEY_UP:
                selected--;
                if(selected<0)
                    selected = 2;
                break;
            case KEY_ENTER:
            case 10:
                if(selected == 2) {
                    storage->deleteEvent(event);                    
                    wprintw(rWin, "Deleting...\n");
                    wrefresh(rWin);

                    napms(1000);
                    wclear(rWin);
                    wclear(lWin);
                    wclear(win);
                    delwin(lWin);
                    delwin(rWin);
                    return 2;
                }
                if(selected == 1) {
                    vector<Interval> intls = findFreeIntervals(event, storage);
                    Interval chosenIntl = chooseInterval(rWin, storage, event, intls);
                    if(chosenIntl.start.year == 0) {
                        break;
                    }
                    napms(1000);
                    storage->changeEventTime(event, chosenIntl);
                    wclear(rWin);
                    wclear(lWin);
                    wclear(win);
                    delwin(lWin);
                    delwin(rWin);
                    return 1;
                }
                break;
        }
        mvwprintw(lWin,currentRow+1, 0, "\nOptions:");
        drawButton("EDIT",currentRow+2,0 , selected == 0, lWin);
        drawButton("MOVE EVENT",currentRow+3, 0, selected == 1, lWin);
        drawButton("DELETE",currentRow+4, 0, selected == 2, lWin);
        wrefresh(lWin);
        
    }

    wclear(rWin);
    wclear(lWin);
    wclear(win);
    delwin(lWin);
    delwin(rWin);

    return 0;
}

bool intervalsOverlapping(const Interval& a, const Interval& b) {
    // Check if the intervals overlap by comparing their start and end values
    if (a.start <= b.end && b.start <= a.end) {
        // Intervals overlap
        return true;
    } else {
        // Intervals do not overlap
        return false;
    }
}


vector<Interval> findFreeIntervals(const Event * event, EventDictionary * storage) {
    long long diff = event->getDifference();
    // Try first 10 movable
    vector<Interval> res;
    for(int i = 1; i <= 10; i++) {
        Interval possibleIntl = event->proposeNextInterval(i);
        // for(const Event * event : storage->getEvents()) {
        // }
        res.push_back(possibleIntl);
    }
    return res;
}