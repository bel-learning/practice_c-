#ifndef EVENTVIEW_H
#define EVENTVIEW_H
#include<ncurses.h>
#include "../event.h"
#include "../calendar.h"




int AddEventView(WINDOW * main, Calendar * cal, EventDictionary * storage);

int GetEventView(WINDOW * main, EventDictionary * storage, const Event * event);
// Just a helper function used in moving events
vector<Interval> findFreeIntervals(const Event * event, EventDictionary * storage);
#endif // EVENTVIEW_H