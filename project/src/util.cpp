#include <chrono>
#include "datetime.h"
#include "util.h"
#include <ctime>
#include <string>
#include <iostream>
#include <fstream>
#include <ncurses.h>

/**
 * @file util.cpp
 *
 * @brief Implementation of Helper functions commonly used in the program.
 */

Datetime getCurrentDateTime()
{
    auto now = chrono::system_clock::now();
    time_t currentTime = chrono::system_clock::to_time_t(now);
    tm *tmTime = localtime(&currentTime);
    return Datetime(tmTime);
};

string getInput(WINDOW *win)
{
    int COLS = getmaxx(win);
    WINDOW *inWindow = subwin(win, 4, COLS - 2, 3, 2);
    int ch;
    string input;

    // WHILE ENTER OR ESC
    while ((ch = getch()) != '\n')
    {
        if (ch == KEY_BACKSPACE || ch == KEY_DC || ch == 127)
        {
            if (!input.empty())
                input.pop_back();
        }
        else if (ch == KEY_ENTER || ch == '\n')
        {
            break;
        }
        // ESC
        else if (ch == 27)
        {
            input = "exit";
            break;
        }
        else
            input.push_back(static_cast<char>(ch));
        wclear(inWindow);
        wprintw(inWindow, "> ");
        wprintw(inWindow, "%s", input.c_str());
        wrefresh(inWindow);
    }
    wclear(inWindow);
    delwin(inWindow);
    return input;
}

std::string getDayOfWeek(int index)
{
    switch (index)
    {
    case 0:
        return "Sunday";
    case 1:
        return "Monday";
    case 2:
        return "Tuesday";
    case 3:
        return "Wednesday";
    case 4:
        return "Thursday";
    case 5:
        return "Friday";
    case 6:
        return "Saturday";
    default:
        return "Invalid day";
    }
}

int dayIndexInWeek(const Datetime &time)
{
    int year = time.year;
    int month = time.month;
    int day = time.day;

    static int t[] = {0, 3, 2, 5, 0, 3, 5, 1,
                      4, 6, 2, 4};
    year -= month < 3;
    // Common way to calculate weekday index of given day in date
    return (year + year / 4 - year / 100 +
            year / 400 + t[month - 1] + day) %
           7;
}
std::string getMonthName(int monthNum)
{
    std::string months[] = {"Jan", "Feb", "Mar",
                            "Apr", "May", "Jun",
                            "Jul", "Aug", "Sep",
                            "Oct", "Nov", "Dec"};

    return (months[monthNum - 1]);
}

int getDaysInMonth(const Datetime &date)
{
    int numDays = 31;
    int month = date.month;
    int year = date.year;
    if (month == 4 || month == 6 || month == 9 || month == 11)
        numDays = 30; // April, June, September, November

    if (month == 2)
    {
        if ((year % 4 == 0 && year % 100 != 0) || year % 400 == 0)
            numDays = 29; // February leap year
        else
            numDays = 28; // February non-leap year
    }
    return numDays;
}

void writeToDebug(const string &out, ofstream &file)
{
    file << out << endl;
    return;
}

void writeToDebug(const string &out)
{
    ofstream file("../debugger.txt");
    file << out << endl;
    return;
}
void writeToDebug(const char &out)
{
    ofstream file("../debugger.txt");

    file << out << endl;
    return;
}
void writeToDebug(const int &out)
{
    ofstream file("../debugger.txt");

    file << out << endl;
    return;
}
Datetime increaseMonth(Datetime &date)
{
    if (date.month == 12)
    {
        date.year++;
        date.month = 1;
    }
    else
        date.month++;
    return date;
}

Datetime decreaseMonth(Datetime &date)
{
    if (date.month == 1)
    {
        date.year--;
        date.month = 12;
    }
    else
        date.month--;
    return date;
}
Datetime findYesterday(const Datetime &dt)
{
    int year = dt.year;
    int month = dt.month;
    int day = dt.day;
    day--;
    if (day == 0)
    {
        // Decrease the month by 1
        month--;

        // Check if the month becomes zero
        if (month == 0)
        {
            // Decrease the year by 1
            year--;

            // Set the month to December
            month = 12;
        }

        // Get the number of days in the previous month
        int daysInPreviousMonth = getDaysInMonth(Datetime(year, month, day, 0, 0, 0));

        // Set the day to the last day of the previous month
        day = daysInPreviousMonth;
    }

    // Create a new Datetime object with the updated values
    Datetime yesterday(year, month, day, dt.hour, dt.minute, dt.second);
    return yesterday;
}
Datetime findTomorrow(const Datetime &dt)
{
    int year = dt.year;
    int month = dt.month;
    int day = dt.day;

    // Get the number of days in the current month
    int daysInCurrentMonth = getDaysInMonth(Datetime(year, month, day, 0, 0, 0));

    // Increase the day by 1
    day++;

    // Check if the day exceeds the number of days in the current month
    if (day > daysInCurrentMonth)
    {
        // Reset the day to 1
        day = 1;

        // Increase the month by 1
        month++;

        // Check if the month exceeds 12
        if (month > 12)
        {
            // Reset the month to 1
            month = 1;

            // Increase the year by 1
            year++;
        }
    }
    // Create a new Datetime object with the updated values
    Datetime tomorrow(year, month, day, dt.hour, dt.minute, dt.second);
    return tomorrow;
}

Datetime calculateEndtime(const Datetime &start, long long durationMinute)
{
    long long startInSeconds = start.toSeconds();
    long long durationInSeconds = durationMinute * 60;
    return secondsToDatetime(startInSeconds + durationInSeconds);
}
Datetime secondsToDatetime(long long totalSeconds)
{
    Datetime dt;

    // Convert seconds to years
    dt.year = totalSeconds / (365 * 24 * 60 * 60);
    totalSeconds %= (365 * 24 * 60 * 60);

    // Convert seconds to months
    dt.month = totalSeconds / (30 * 24 * 60 * 60);
    totalSeconds %= (30 * 24 * 60 * 60);

    // Convert seconds to days
    dt.day = totalSeconds / (24 * 60 * 60);
    totalSeconds %= (24 * 60 * 60);

    // Convert seconds to hours
    dt.hour = totalSeconds / (60 * 60);
    totalSeconds %= (60 * 60);

    // Convert seconds to minutes
    dt.minute = totalSeconds / 60;
    totalSeconds %= 60;

    // Remaining seconds
    dt.second = totalSeconds;

    return dt;
}

Event::Repeat stringToRepeat(const string &formatted)
{
    if (formatted == "none" || formatted == "")
        return Event::Repeat::None;
    if (formatted == "weekly")
        return Event::Repeat::Weekly;
    if (formatted == "biweekly")
        return Event::Repeat::BiWeekly;
    if (formatted == "monthly")
        return Event::Repeat::Monthly;
    if (formatted == "daily")
        return Event::Repeat::Daily;
    if (formatted == "yearly")
        return Event::Repeat::Yearly;
    return Event::Repeat::Yearly;
}

string repeatToString(const Event::Repeat &repeat)
{
    switch (repeat)
    {
    case Event::Repeat::Yearly:
        return "yearly";
    case Event::Repeat::Monthly:
        return "monthly";
    case Event::Repeat::BiWeekly:
        return "biweekly";
    case Event::Repeat::Weekly:
        return "weekly";
    case Event::Repeat::Daily:
        return "daily";
    default:
        return "none";
    }
}

Datetime startOfCurrentWeek(const Datetime &dt)
{
    Datetime current = dt;
    Datetime start(current);
    start.hour = 0;
    start.minute = 0;
    start.second = 0;

    // If not monday
    while (dayIndexInWeek(start) != 1)
    {
        start = findYesterday(start);
    }

    return start;
}
Datetime endOfCurrentWeek(const Datetime &dt)
{
    Datetime current = dt;
    current = startOfNextWeek(current);
    current = findYesterday(current);
    current.hour = 23;
    current.minute = 59;
    current.second = 59;
    return current;
}

Datetime startOfNextWeek(const Datetime &dt)
{
    Datetime res = startOfCurrentWeek(dt);
    res = findTomorrow(res);
    while (dayIndexInWeek(res) != 1)
    {
        res = findTomorrow(res);
    }
    return res;
}
Datetime startOfPreviosWeek(const Datetime &dt)
{
    Datetime res = startOfCurrentWeek(dt);
    res = findYesterday(res);
    return res;
}

long long repeatToSeconds(const Event::Repeat &repeat)
{
    switch (repeat)
    {
    case Event::Repeat::Yearly:
        return 365 * 24 * 60 * 60;
    case Event::Repeat::Monthly:
        return 30 * 24 * 60 * 60;
    case Event::Repeat::BiWeekly:
        return 14 * 24 * 60 * 60;
    case Event::Repeat::Weekly:
        return 7 * 24 * 60 * 60;
    case Event::Repeat::Daily:
        return 24 * 60 * 60;
    default:
        return 0;
    }
}

string promptInput(WINDOW *win, string question)
{
    wattron(win, A_BOLD);
    mvwprintw(win, 0, 0, "%s", question.c_str());
    wattroff(win, A_BOLD);

    wrefresh(win);
    wclear(win);
    return getInput(win);
}