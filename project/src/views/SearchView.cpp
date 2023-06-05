#include<ncurses.h>
#include<cctype>
#include "../calendar.h"
#include "./EventView.h"

using namespace std;

bool startsWith(const string& str, const string& prefix) {
    if (str.length() < prefix.length()) {
        return false;  // String is shorter than the prefix, cannot start with it
    }

    for (size_t i = 0; i < prefix.length(); ++i) {
        if (tolower(str[i]) != tolower(prefix[i])) {
            return false; // Characters don't match
        }
    }

    return true;

}

vector<Event *> match(EventDictionary * storage, const string & needle) {
    vector<Event *> foundEvents;
    for(const Event * event : storage->getEvents()) {
        if(startsWith(event->getTitle(), needle)) {
            foundEvents.push_back(event->makeCopy());
        }
    }
    for(const Event * event : storage->getRepeatEvents()) {
        if(startsWith(event->getTitle(), needle)) {
            foundEvents.push_back(event->makeCopy());
        }
    }
    return foundEvents;
}

int search(WINDOW * win, EventDictionary * storage) {
    int COLS = getmaxx(win);
    int ROWS = getmaxy(win);
    WINDOW * inWindow = subwin(win, ROWS-5, COLS-3 , 4,2);
    wattron(win, A_BOLD);
    mvwprintw(win, 2, COLS/2 - 6, "Search Events");
    wattroff(win, A_BOLD);
    wrefresh(win);


    int ch = KEY_BACKSPACE;
    string input = "";
    vector<Event *> foundEvents;
    int selected = 0;
    // ESC to leave
    do {
        wattron(win, A_BOLD);
        mvwprintw(win, 2, COLS/2 - 6, "Search Events");
        wattroff(win, A_BOLD);
        wrefresh(win);
        switch(ch) {
            case KEY_BACKSPACE:
            case KEY_DC:
            case 127:
                if(!input.empty() && selected == 0)
                    input.pop_back();
                break;
            case KEY_ENTER:
            case '\n':
                if(selected) {
                    wclear(win);
                    wclear(inWindow);
                    int state = GetEventView(win, storage, foundEvents[selected-1]);
                    for(const Event * event : foundEvents) {
                        delete event;
                    }
                    box(win, 0,0);
                    return state;
                }
                break;
            case KEY_DOWN:
                selected++;
                if (selected > foundEvents.size())
                    selected = 0;
                break;

            case KEY_UP:
                selected--;
                if(selected < 0) {
                    selected = foundEvents.size();
                }
                break;
            default:    
                if(selected == 0)
                    input.push_back(static_cast<char>(ch));
                break;
        }
     
        wclear(inWindow);
        // The write page
        if(selected == 0)
            wattron(inWindow, COLOR_PAIR(3));

        wprintw(inWindow, "> ");
        wprintw(inWindow, "%s\n", input.c_str());
        wattroff(inWindow, COLOR_PAIR(3));


        wattron(inWindow, A_BOLD);
        wprintw(inWindow, "Results:\n");
        wattroff(inWindow, A_BOLD);


        for(const Event * event : foundEvents) {
            delete event;
        }

        foundEvents = match(storage, input);
        size_t i = 1;
        for(const Event * event : foundEvents) {
            if(selected == i)
                wattron(inWindow, COLOR_PAIR(3));
            event->renderInDays(inWindow, false);
            wattroff(inWindow, COLOR_PAIR(3));
            i++;
            // wprintw(inWindow, "\n");
        }
        wrefresh(inWindow);

    } while ((ch = getch()) != 27);

    wclear(inWindow);
    delwin(inWindow);
}

int GetSearchView(WINDOW * main, EventDictionary * storage) {
    int MAX_ROWS, MAX_COLS;
    getmaxyx(main, MAX_ROWS, MAX_COLS);

    int state = search(main, storage);
    wclear(main);
    return state;
}