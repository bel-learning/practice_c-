
#include<ncurses.h>
#include "../calendar.h"
#include "../util.h"
#include<string>
#include<fstream>
using namespace std;


int GetExportView(WINDOW * win, EventDictionary * storage) {
    int MAX_ROWS, MAX_COLS;
    getmaxyx(win, MAX_ROWS, MAX_COLS);
    wattron(win, A_UNDERLINE);
    mvwprintw(win, MAX_ROWS-2, MAX_COLS-24, "WRITE EXIT TO CANCEL:");
    wattroff(win, A_UNDERLINE);

    WINDOW * sWin = subwin(win, MAX_ROWS - 4, MAX_COLS - 4, 1, 2);
    refresh();

    string filename = promptInput(sWin, "<Filename>.cal (It'll be in the examples directory): ");
        mvwprintw(win, MAX_ROWS/2-1, MAX_COLS/2-6, filename.c_str());
    wrefresh(win);
    filename = "./examples/" + filename;
    ofstream file(filename, std::ios::trunc);   

    if(!file.fail()) {
        for(const Event * event : storage->getEvents()) {
            file << event->toFile().str() << endl;
        }
        for(const Event * event : storage->getRepeatEvents()) {
            file << event->toFile().str() << endl;
        }
        
        mvwprintw(win, MAX_ROWS/2, MAX_COLS/2-6, "Success");
        wrefresh(win);
        file.close();
    }
    else {
        mvwprintw(win, MAX_ROWS/2, MAX_COLS/2-6, "File exists");
        wrefresh(win);
        file.close();
    }

    napms(1000);
    wclear(sWin);
    wclear(win);
    delwin(sWin);

    return 1;
}
