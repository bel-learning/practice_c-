
#include<ncurses.h>
#include "../calendar.h"


int GetExportView(WINDOW * win, EventDictionary * storage) {
    wprintw(win, "Import section");
    int ch = getch();
    return ch;
}
