#include <string>
#include <vector>
#include <unordered_map>
#include <memory>
#include <algorithm>
#include "event.h"
#include "calendar.h"
#include "datetime.h"
#include "util.h"

/**
 * @file calendar.cpp
 *
 * @brief Implementation of Calendar and its derived classes.
 */

using namespace std;

Calendar::Calendar() {
    m_CurrentTime = getCurrentDateTime();
};
Calendar::Calendar(const Datetime & time) : m_CurrentTime(time) {}
Calendar::~Calendar() {}


DailyCalendar::DailyCalendar() : Calendar() {};
DailyCalendar::~DailyCalendar() {};


void DailyCalendar::print(WINDOW * win, const Datetime & date, int selected) const {
    wclear(win);
    int MAX_COLS = getmaxx(win);

    wattron(win, COLOR_PAIR(3));
    mvwprintw(win, 2 , MAX_COLS / 2 - 10, "<Daily Calendar>");
    wattroff(win, COLOR_PAIR(3));

    mvwprintw(win, 2 + 2, MAX_COLS / 2 - 10, "Today is %d %s", date.day, getMonthName(date.month).c_str());

    wrefresh(win);
}
void DailyCalendar::displayEvents(WINDOW * win, const vector<Event *> & events, bool showAll) const {
    wclear(win);
    int middle = getmaxx(win) / 2;
    wattron(win, A_BOLD);
    mvwprintw(win, 2, middle-6, "Events\n\n");
    wattroff(win, A_BOLD);

    if(events.size() == 0) {
        wprintw(win, "No events\n");
    }
    for(const Event * e : events) {
        e->renderInHours(win, true);
    }
    wrefresh(win);
}

const vector<Event * > DailyCalendar::getEvents(const Datetime & start, bool showAll) const {
    vector<Event *> dailyEvents;
    // Getting start of day
    Datetime startDate(start);
    startDate.hour = 0;
    startDate.minute = 0;
    startDate.second = 0;

    // Getting end of day
    Datetime endDate(start);
    endDate.hour = 23;
    endDate.minute = 59;    
    endDate.second = 59;

    // Finding the repeatable events are within the interval
    for(const Event * e : m_Dictionary->getRepeatEvents()) {
        if(e->getCompareDate() < endDate) {
            if(e->insideInterval(startDate, endDate)) {
                dailyEvents.push_back(e->makeCopy());
            }
            Event * newEvent = e->findNextRepeatable();
            Event * toDelete = newEvent;
            while(newEvent->getCompareDate() < endDate) {
                if(newEvent->insideInterval(startDate, endDate)) {
                    dailyEvents.push_back(newEvent->makeCopy());
                }
                newEvent = newEvent->findNextRepeatable();
                delete toDelete;
                toDelete = newEvent;
            }
            delete toDelete;
        }
        else {
            if(e->insideInterval(startDate, endDate)) {
                dailyEvents.push_back(e->makeCopy());
            }
            Event * newEvent = e->findPreviousRepeatable();
            Event * toDelete = newEvent;
            while(newEvent->getCompareDate() > startDate) {
                if(newEvent->insideInterval(startDate, endDate)) {
                    dailyEvents.push_back(newEvent->makeCopy());
                }
                newEvent = newEvent->findPreviousRepeatable();
                delete toDelete;
                toDelete = newEvent;
            }
            delete toDelete;
            
        }
    }

    for(const Event * e : m_Dictionary->getEvents()) {
        if(e->insideInterval(startDate, endDate)) {
            dailyEvents.push_back(e->makeCopy());
        }
    }
    vector<Event *> firstFifteenEvents;
    size_t len = dailyEvents.size();

    if(len > 15)
        len = 15;

    for(size_t i = 0; i < len; i++) {
        firstFifteenEvents.push_back(dailyEvents[i]->makeCopy());
    }
    for(Event * event : dailyEvents) {
        delete event;  
    }
    return firstFifteenEvents;
    return dailyEvents;
}



WeeklyCalendar::WeeklyCalendar() : Calendar() {};
WeeklyCalendar::~WeeklyCalendar() {};


void WeeklyCalendar::print(WINDOW * win, const Datetime & date, int selected) const {
    wclear(win);
    Datetime start = startOfCurrentWeek(date);
    Datetime end = start;
    // Six days from monday will be sunday hihi
    for(int i = 0; i < 6; i++)
        end = findTomorrow(end);

    int MAX_ROWS, MAX_COLS;
    getmaxyx(win, MAX_ROWS, MAX_COLS);

    int paddingTop = 2;

    wattron(win, A_BOLD);
    mvwprintw(win, 2 , MAX_COLS / 2 - 10, "Weekly Calendar");
    wattroff(win, A_BOLD);

    if (selected == 0) {
        wattron(win, COLOR_PAIR(3));
        mvwprintw(win, 2 + paddingTop, MAX_COLS/2 - 11, "<%02d %s-%02d %s>\n", start.day, getMonthName(start.month).c_str(), end.day, getMonthName(end.month).c_str());
        wattroff(win, COLOR_PAIR(3));
    }
    else {
        mvwprintw(win, 2 + paddingTop, MAX_COLS/2 - 10, "%02d %s-%02d %s\n", start.day, getMonthName(start.month).c_str(), end.day, getMonthName(end.month).c_str());
    }
    int iRows = 1;
    int weekDayIndex = 0;
    paddingTop += 1;
    Datetime iterateDate(start);
    for(int i = 1; i <= 7; i++) {
        weekDayIndex++;
        if(weekDayIndex > 6)
            weekDayIndex = 0;
        if(selected == i)
            wattron(win, COLOR_PAIR(3));
        if(2 + paddingTop + iRows * i >= MAX_ROWS-1)
            break;
        mvwprintw(win, 2 + paddingTop + iRows * i, MAX_COLS/2 - 10, "%02d %s\n", iterateDate.day ,getDayOfWeek(weekDayIndex).c_str());
        iterateDate = findTomorrow(iterateDate);
        wattroff(win, COLOR_PAIR(3));
    }
    wattroff(win, COLOR_PAIR(3));

    wrefresh(win);

}

void WeeklyCalendar::displayEvents(WINDOW * win, const vector<Event *> & events, bool showAll) const {
    wclear(win);
    int middle = getmaxx(win) / 2;
    wattron(win, A_BOLD);
    mvwprintw(win, 2, middle-6, "Events\n\n");
    wattroff(win, A_BOLD);

    if(events.size() == 0) {
        wprintw(win, "No events\n");
    }
    for(const Event * e : events) {
        if(showAll)
            e->renderInDays(win, true);
        else    
            e->renderInHours(win, true);

        // wprintw(win, "%s\n", e->ge);
    }
    wrefresh(win);
}


const vector<Event * > WeeklyCalendar::getEvents(const Datetime & start, bool showAll) const {
    // Special case to notify get everything in that week limited to 15 tho.
    vector<Event *> eventsInInterval;

    Datetime startDate(start);
    Datetime endDate(start);

    // Special case to notify get everything in that week limited to 15 tho.
    if(showAll) {
        // Get all the events in the month
        startDate = startOfCurrentWeek(start);
        endDate = endOfCurrentWeek(start);
    }
    // Getting start of the first day
    startDate.hour = 0;
    startDate.minute = 0;
    startDate.second = 0;

    // Getting end of day the last day
    endDate.hour = 23;
    endDate.minute = 59;
    endDate.second = 59;    
    for(const Event * e : m_Dictionary->getRepeatEvents()) {
        if(e->getCompareDate() < endDate) {
            if(e->insideInterval(startDate, endDate)) {
                eventsInInterval.push_back(e->makeCopy());
            }
            Event * newEvent = e->findNextRepeatable();
            Event * toDelete = newEvent;
            while(newEvent->getCompareDate() < endDate) {
                if(newEvent->insideInterval(startDate, endDate)) {
                    eventsInInterval.push_back(newEvent->makeCopy());
                }
                newEvent = newEvent->findNextRepeatable();
                delete toDelete;
                toDelete = newEvent;
            }
            delete toDelete;
        }
        else {
            if(e->insideInterval(startDate, endDate)) {
                eventsInInterval.push_back(e->makeCopy());
            }
            Event * newEvent = e->findPreviousRepeatable();
            Event * toDelete = newEvent;
            while(newEvent->getCompareDate() > startDate) {
                if(newEvent->insideInterval(startDate, endDate)) {
                    eventsInInterval.push_back(newEvent->makeCopy());
                }
                newEvent = newEvent->findPreviousRepeatable();
                delete toDelete;
                toDelete = newEvent;
            }
            delete toDelete;
            
        }
    }
    for(const Event * e : m_Dictionary->getEvents()) {
        if(e->insideInterval(startDate, endDate)) {
            eventsInInterval.push_back(e->makeCopy());
        }
    }
    sort(eventsInInterval.begin(), eventsInInterval.end(), [] (const Event * a, const Event * b) {
        return a->isLowerThan(b);
    });
    vector<Event *> firstFifteenEvents;
    size_t len = eventsInInterval.size();

    if(len > 15)
        len = 15;

    for(size_t i = 0; i < len; i++) {
        firstFifteenEvents.push_back(eventsInInterval[i]->makeCopy());
    }
    for(Event * event : eventsInInterval) {
        delete event;  
    }
    return firstFifteenEvents;
}


MonthlyCalendar::MonthlyCalendar() : Calendar() {};
MonthlyCalendar::~MonthlyCalendar() {};

void MonthlyCalendar::display(WINDOW * win, const Datetime & date, int selected) const {
    wclear(win);
    Datetime showDate = date;
    int month = showDate.month;
    int year = showDate.year;

    int numDays = getDaysInMonth(showDate);

    int MAX_ROWS, MAX_COLS;
    getmaxyx(win, MAX_ROWS, MAX_COLS);

    int paddingTop = 2;

    wattron(win, A_BOLD);
    mvwprintw(win, 2 , MAX_COLS / 2 - 10, "Monthly Calendar");
    wattroff(win, A_BOLD);


    if (selected == 0) {
        wattron(win, COLOR_PAIR(3));
        mvwprintw(win,2 + paddingTop, MAX_COLS/2-12, "< Calendar - %d/%d >", month, year);
    }
    else {
        wattroff(win, COLOR_PAIR(3));
        mvwprintw(win,2 + paddingTop, MAX_COLS/2-10, "Calendar - %d/%d", month, year);
    }
    wattroff(win, COLOR_PAIR(3));

    mvwprintw(win,4 + paddingTop , 4, " Sun Mon Tue Wed Thu Fri Sat");

    Datetime tmp = showDate;
    tmp.day = 1;
    int startDay = dayIndexInWeek(tmp);

    int row = 6;
    int col = startDay * 4;      
    int paddingLeft = 4;

    for (int i = 1; i <= numDays; ++i) {
        if(selected == i) {
            wattron(win, COLOR_PAIR(3));
        }
        mvwprintw(win, row + paddingTop, col + paddingLeft, "%3d", i);
        wattroff(win, COLOR_PAIR(3));
        col += 4;

        if ((i + startDay) % 7 == 0) {
            row++;
            col = 0;
        }
    }

    wrefresh(win);
}

void MonthlyCalendar::print(WINDOW * win, const Datetime & date, int selected) const {
    display(win, date, selected);
}
void MonthlyCalendar::displayEvents(WINDOW * win, const vector<Event *> & events, bool showAll) const {
    wclear(win);
    int middle = getmaxx(win) / 2;
    wattron(win, A_BOLD);
    mvwprintw(win, 2, middle-6, "Events\n\n");
    wattroff(win, A_BOLD);

    if(events.size() == 0) {
        wprintw(win, "No events\n");
    }
    for(const Event * e : events) {
        if(showAll)
            e->renderInDays(win, true);
        else    
            e->renderInHours(win, true);

        // wprintw(win, "%s\n", e->ge);
    }
    wrefresh(win);
}

const vector<Event * > MonthlyCalendar::getEvents(const Datetime & start, bool showAll) const {
    vector<Event *> eventsInInterval;

    Datetime startDate(start);
    Datetime endDate(start);

    // Special case to notify get everything in that week limited to 15 tho.
    if(showAll) {
        // Get all the events in the month
        startDate.day = 1;
        endDate.day = getDaysInMonth(start);
    }
   
    startDate.hour = 0;
    startDate.minute = 0;
    startDate.second = 0;
    // Getting end of day
    
    endDate.hour = 23;
    endDate.minute = 59;
    endDate.second = 59;    

    for(const Event * e : m_Dictionary->getRepeatEvents()) {
        if(e->getCompareDate() < endDate) {
            if(e->insideInterval(startDate, endDate)) {
                eventsInInterval.push_back(e->makeCopy());
            }
            Event * newEvent = e->findNextRepeatable();
            Event * toDelete = newEvent;
            while(newEvent->getCompareDate() < endDate) {
                if(newEvent->insideInterval(startDate, endDate)) {
                    eventsInInterval.push_back(newEvent->makeCopy());
                }
                newEvent = newEvent->findNextRepeatable();
                delete toDelete;
                toDelete = newEvent;
            }
            delete toDelete;
        }
        else {
            if(e->insideInterval(startDate, endDate)) {
                eventsInInterval.push_back(e->makeCopy());
            }
            Event * newEvent = e->findPreviousRepeatable();
            Event * toDelete = newEvent;
            while(newEvent->getCompareDate() > startDate) {
                if(newEvent->insideInterval(startDate, endDate)) {
                    eventsInInterval.push_back(newEvent->makeCopy());
                }
                newEvent = newEvent->findPreviousRepeatable();
                delete toDelete;
                toDelete = newEvent;
            }
            delete toDelete;
            
        }
    }

    for(const Event * e : m_Dictionary->getEvents()) {
        if(e->insideInterval(startDate, endDate)) {
            eventsInInterval.push_back(e->makeCopy());
        }
    }
    sort(eventsInInterval.begin(), eventsInInterval.end(), [] (const Event * a, const Event * b) {
        return a->isLowerThan(b);
    });
    vector<Event *> firstFifteenEvents;
    size_t len = eventsInInterval.size();

    if(len > 15)
        len = 15;

    for(size_t i = 0; i < len; i++) {
        firstFifteenEvents.push_back(eventsInInterval[i]->makeCopy());
    }

    for(Event * event : eventsInInterval) {
        delete event;  
    }

    return firstFifteenEvents;
}


EventDictionary::EventDictionary() {};

EventDictionary::EventDictionary(const EventDictionary * ed) : m_Events(ed->m_Events) {};

EventDictionary::EventDictionary(const EventDictionary & ed) : m_Events(ed.m_Events) {};

EventDictionary EventDictionary::operator= (const EventDictionary & ed) {
    if(this == &ed) return *this;
    for(Event * e : m_Events)
        delete e;
    m_Events = ed.m_Events;
    return *this;
};
EventDictionary::~EventDictionary(){
    for(Event * e : m_Events) {
        delete e;
    }
    for(Event * e : m_RepeatEvents) {
        delete e;
    }
};

void EventDictionary::addEvent(const Event * event) {
    m_Events.push_back(event->makeCopy());
    delete event;
}
void EventDictionary::addRepeatEvent(const Event * event) {
    m_RepeatEvents.push_back(event->makeCopy());
    delete event;
}


const vector<Event *> & EventDictionary::getEvents() const {
    return m_Events;
}
const vector<Event *> & EventDictionary::getRepeatEvents() const {
    return m_RepeatEvents;
}


void EventDictionary::deleteEvent(const Event * event) {
    int i = 0;
    for(Event * e : m_Events) {
        if(e->getTitle() == event->getTitle()
        && e->getDescription() == event->getDescription()
        && e->getRepeat() == event->getRepeat()
        ) {
            delete e;
            m_Events.erase(m_Events.begin() + i);
            return;
        }
        i++;
    }
    i = 0;
    for(Event * e : m_RepeatEvents) {
        if(e->getTitle() == event->getTitle()
        && e->getDescription() == event->getDescription()
        && e->getRepeat() == event->getRepeat()
        ) {
            delete e;
            m_RepeatEvents.erase(m_RepeatEvents.begin() + i);
            return;
        }
        i++;
    }
};

void EventDictionary::changeEventTime(const Event * event, const Interval & intl) {
    for(Event * e : m_Events) {
        if(e->getTitle() == event->getTitle()
        && e->getDescription() == event->getDescription()
        && e->getRepeat() == event->getRepeat()
        ) {
            e->changeTime(intl);            
            return;
        }
    }
    for(Event * e : m_RepeatEvents) {
        if(e->getTitle() == event->getTitle()
        && e->getDescription() == event->getDescription()
        && e->getRepeat() == event->getRepeat()
        ) {
            e->changeTime(intl);            
            return;
        }
    }
}