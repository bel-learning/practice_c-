#include<ncurses.h>
#include "../calendar.h"
#include<cctype>

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

void search(WINDOW * win, EventDictionary * storage) {
    int COLS = getmaxx(win);
    int ROWS = getmaxy(win);
    WINDOW * inWindow = subwin(win, ROWS-5, COLS-2 , 4,2);
    int ch;
    string input;
    vector<Event *> foundEvents;
    int selected = 0;
    // ESC to leave
    while ((ch = getch()) != 27) {
        switch(ch) {
            case KEY_BACKSPACE:
            case KEY_DC:
            case 127:
                if(!input.empty() && selected == 0)
                    input.pop_back();
            case KEY_ENTER:
            case '\n':
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

    }
    wclear(inWindow);
    delwin(inWindow);
}

int GetSearchView(WINDOW * main, EventDictionary * storage) {
    int MAX_ROWS, MAX_COLS;
    getmaxyx(main, MAX_ROWS, MAX_COLS);
    wattron(main, A_BOLD);
    mvwprintw(main, 2, MAX_COLS/2-6, "Search Events");
    wattroff(main, A_BOLD);
    refresh();

    search(main, storage);
    wclear(main);
    return 0;
}