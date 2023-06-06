#include <ncurses.h>
#include <fstream>
#include "../calendar.h"
#include "../util.h"

using namespace std;

/**
 * @file ImportView.cpp
 *
 * @brief Declaration of functions in ImportView and its utility functions
 */

Event *parseLine(const string &formatted)
{
    Event *newEvent = nullptr;
    std::istringstream iss(formatted);
    std::string token;
    std::getline(iss, token, ',');
    string type = token;

    std::getline(iss, token, ',');
    string title = token;

    std::getline(iss, token, ',');
    string description = token;

    std::getline(iss, token, ',');
    string repeatFormatted = token;
    Event::Repeat repeat = stringToRepeat(repeatFormatted);
    string leftOver;
    std::getline(iss, leftOver);

    if (type == "event")
    {
        newEvent = new NormalEvent(title, description, repeat, leftOver);
    }
    if (type == "task")
    {
        newEvent = new Task(title, description, repeat, leftOver);
    }
    if (type == "deadline")
    {
        newEvent = new Deadline(title, description, repeat, leftOver);
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
    if(filename == "exit") {
        wclear(sWin);
        wclear(win);
        delwin(sWin);
        return 0;
    }
    mvwprintw(win, MAX_ROWS / 2 - 1, MAX_COLS / 2 - 6,"%s", filename.c_str());
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
            if (event->getRepeat() == Event::Repeat::None)
            {
                storage->addEvent(event);
            }
            else
            {
                storage->addRepeatEvent(event);
            }
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

    return 1;
}