#include<ncurses.h>
#include "../event.h"
#include "../util.h"
#include "../calendar.h"
#include<string>

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