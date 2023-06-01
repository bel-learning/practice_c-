#include <string>
#include <vector>
#include <unordered_map>
#include <memory>
#include "event.h"
#include "calendar.h"
#include "datetime.h"
#include "util.h"
#include "window.h"

using namespace std;

Calendar::Calendar() {
    m_CurrentTime = getCurrentDateTime();
};
Calendar::Calendar(const Datetime & time) : m_CurrentTime(time) {}
Calendar::~Calendar() {}
bool Calendar::addEvent(const Event * e) {
    m_Events.push_back(e->makeCopy());
}


void Calendar::print(WINDOW * win, const Datetime & date, int selected) const {
}   

const vector<Event * > & Calendar::getEvents() const {

}

DailyCalendar::DailyCalendar() : Calendar() {};
DailyCalendar::~DailyCalendar() {};


void DailyCalendar::print(WINDOW * win, const Datetime & date, int selected) const {

}

const vector<Event * > & DailyCalendar::getEvents() const {

}



WeeklyCalendar::WeeklyCalendar() : Calendar() {};
WeeklyCalendar::~WeeklyCalendar() {};


void WeeklyCalendar::print(WINDOW * win, const Datetime & date, int selected) const {

}
const vector<Event * > & WeeklyCalendar::getEvents() const {

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
        wattron(win, A_REVERSE);
        mvwprintw(win,2 + paddingTop, MAX_COLS/2-12, "< Calendar - %d/%d >", month, year);
    }
    else {
        wattroff(win, A_REVERSE);
        mvwprintw(win,2 + paddingTop, MAX_COLS/2-10, "Calendar - %d/%d", month, year);
    }
    wattroff(win, A_REVERSE);

    mvwprintw(win,4 + paddingTop , 4, " Sun Mon Tue Wed Thu Fri Sat");

    Datetime tmp = showDate;
    tmp.day = 1;
    int startDay = dayIndexInWeek(tmp);

    int row = 6;
    int col = startDay * 4;      
    int paddingLeft = 4;

    for (int i = 1; i <= numDays; ++i) {
        if(selected == i) {
            wattron(win, A_REVERSE);
        }
        mvwprintw(win, row + paddingTop, col + paddingLeft, "%3d", i);
        wattroff(win, A_REVERSE);
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

const vector<Event * > & MonthlyCalendar::getEvents() const {

}
