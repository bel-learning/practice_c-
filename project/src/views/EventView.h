#ifndef EVENTVIEW_H
#define EVENTVIEW_H
#include<ncurses.h>
#include "../event.h"
#include "../calendar.h"

int AddEventView(WINDOW * main, Calendar * cal, EventDictionary * storage);

#endif // EVENTVIEW_H