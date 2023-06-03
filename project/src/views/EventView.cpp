#include<ncurses.h>
#include "../event.h"
#include "../util.h"
#include "../calendar.h"
#include<string>

using namespace std;

string getInput(WINDOW * win) {
    int COLS = getmaxy(win);
    WINDOW * inWindow = subwin(win, 1, COLS, 3,2);
    int ch;
    string input;
    
    while ((ch = getch()) != '\n') {
        if(ch == KEY_BACKSPACE || ch == KEY_DC || ch == 127 ) {
            if(!input.empty())
                input.pop_back();
        }
        else if(ch == KEY_ENTER || ch == '\n') {
            break;
        }
        // ESC
        else if(ch == 27) {
            input = "exit";
            break;
        }
        else 
            input.push_back(static_cast<char>(ch));
        wclear(inWindow);
        wprintw(inWindow, "> ");
        wprintw(inWindow, input.c_str());
        wrefresh(inWindow);
    }
    wclear(inWindow);
    delwin(inWindow);
    return input;
}
string promptInput(WINDOW * win, string question) {
    wattron(win, A_BOLD);
    mvwprintw(win,0,0, question.c_str());
    wattroff(win, A_BOLD);

    wrefresh(win);
    wclear(win);
    return getInput(win);
}
// Event * createNewEven

int AddEventView(WINDOW * main, Calendar * cal, EventDictionary & storage) {

    int MAX_ROWS, MAX_COLS;
    getmaxyx(main, MAX_ROWS, MAX_COLS);
    wattron(main, A_UNDERLINE);
    mvwprintw(main, MAX_ROWS-2, MAX_COLS-15, "ESC TO CANCEL:");
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
    if(title == "exit") {
        clear();
        delwin(bWindow);

        return 0;
    }

   
    
    
    string repeatFormatted = promptInput(bWindow, "Repeat (daily, weekly, biweekly, monthly, yearly):");
     if(title == "exit") {
        clear();
        delwin(bWindow);
        return 0;
    }
    Event::Repeat repeat = stringToRepeat(repeatFormatted);
    if(type == "event" || type == "") {
        string location = promptInput(bWindow, "Location:");
        if(title == "exit") {
            clear();
            delwin(bWindow);
            return 0;
        }
        string timeStart = promptInput(bWindow, "Start time (YYYY-MM-DD-hh-mm), (MM-DD-hh-mm), (DD-hh-mm):");
        if(timeStart == "exit") {
            clear();
            delwin(bWindow);
            return 0;
        }
        string duration = promptInput(bWindow, "Duration (30, in mins):");
        if(timeStart == "exit") {
            clear();
            delwin(bWindow);
            return 0;
        }
        Datetime start(timeStart);
        event = new NormalEvent(title, description, repeat, location, start, calculateEndtime(start, static_cast<long>(stoi(duration))));
        
        storage.addEvent(event);
        
    }
    


    delwin(bWindow);
    return 1;
}