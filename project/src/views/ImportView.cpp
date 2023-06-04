#include<ncurses.h>
#include "../calendar.h"

int GetImportView(WINDOW * win, EventDictionary * storage) {
    wprintw(win, "Import section");
    int ch = getch();
    return ch;
}