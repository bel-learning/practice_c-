#include <ncurses.h>
#include <fstream>
#include "../validation.h"
#include "../calendar.h"
#include "../util.h"

using namespace std;

/**
 * @file ImportView.cpp
 *
 * @brief Declaration of functions in ImportView and its utility functions
 */

// Line to event
Event *parseLine(const string &formatted)
{
    Event *newEvent = nullptr;
    int occur = 0;
    for (const auto &c : formatted)
    {
        if (c == ',')
            occur++;
    }

    std::istringstream iss(formatted);
    std::string token;
    std::getline(iss, token, ',');
    string type = token;
    if (!isValidType(type))
        return nullptr;
    std::getline(iss, token, ',');
    string title = token;
    if (!isValidTitle(title))
        return nullptr;
    std::getline(iss, token, ',');
    string description = token;
    if (!isValidDescription(description))
        return nullptr;
    std::getline(iss, token, ',');
    string repeatFormatted = token;
    if (!isValidRepeat(repeatFormatted))
        return nullptr;
    Event::Repeat repeat = stringToRepeat(repeatFormatted);

    if (type == "event")
    {
        std::getline(iss, token, ',');
        string location = token;
        if (!isValidLocation(location))
            return nullptr;
        std::getline(iss, token, ',');
        string startDateFormatted = token;
        if (!isValidDate(startDateFormatted))
            return nullptr;
        Datetime startTime(startDateFormatted);

        std::getline(iss, token, ',');
        string endDateFormatted = token;
        if (!isValidDate(endDateFormatted))
            return nullptr;
        Datetime endTime(endDateFormatted);

        newEvent = new NormalEvent(title, description, repeat, location, startTime, endTime);
    }
    if (type == "task")
    {
        std::getline(iss, token, ',');
        string startDateFormatted = token;
        if (!isValidDate(startDateFormatted))
            return nullptr;
        Datetime startTime(startDateFormatted);

        std::getline(iss, token, ',');
        string endDateFormatted = token;
        if (!isValidDate(endDateFormatted))
            return nullptr;
        Datetime endTime(endDateFormatted);

        std::getline(iss, token, ',');
        string strDone = token;
        if (!isValidBool(strDone))
            return nullptr;
        bool finished = strDone[0] == '0' ? false : true;
        newEvent = new Task(title, description, repeat, startTime, endTime, finished);
    }
    if (type == "deadline")
    {
        std::getline(iss, token, ',');
        string endDateFormatted = token;
        if (!isValidDate(endDateFormatted))
            return nullptr;
        Datetime endTime(endDateFormatted);

        newEvent = new Deadline(title, description, repeat, endTime);
    }
    return newEvent;
}

int GetImportView(WINDOW *win, EventDictionary *storage)
{
    int MAX_ROWS, MAX_COLS;
    getmaxyx(win, MAX_ROWS, MAX_COLS);
    wattron(win, A_UNDERLINE);
    mvwprintw(win, MAX_ROWS - 2, MAX_COLS - 22, "ESC TO CANCEL:");
    wattroff(win, A_UNDERLINE);

    WINDOW *sWin = subwin(win, MAX_ROWS - 4, MAX_COLS - 4, 1, 2);
    refresh();

    string filename = promptInput(sWin, "<Filename>.cal (File to import(must be in examples/ directory)):");
    if (filename == "exit")
    {
        wclear(sWin);
        wclear(win);
        delwin(sWin);
        return 1;
    }
    mvwprintw(win, MAX_ROWS / 2 - 1, MAX_COLS / 2 - 6, "%s", filename.c_str());
    wrefresh(win);
    filename = "./examples/" + filename;
    ifstream file(filename);

    if (!file.fail())
    {
        string line;
        while (getline(file, line))
        {
            Event *event = parseLine(line);
            if (event == nullptr)
                continue;
            if (storage->duplicateExists(event)) {
                delete event;
                event = nullptr;
                continue;
            }
            if (event->getRepeat() == Event::Repeat::None)
                storage->addEvent(event);
            else
                storage->addRepeatEvent(event);
        }

        mvwprintw(win, MAX_ROWS / 2, MAX_COLS / 2 - 6, "Success");
        wrefresh(win);
        file.close();
    }
    else
    {
        mvwprintw(win, MAX_ROWS / 2, MAX_COLS / 2 - 6, "File doesn't exists");
        wrefresh(win);
        file.close();
    }

    napms(1000);
    wclear(sWin);
    wclear(win);
    delwin(sWin);

    return 0;
}