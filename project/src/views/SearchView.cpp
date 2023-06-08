#include <ncurses.h>
#include <cctype>
#include "../calendar.h"
#include "./EventView.h"

using namespace std;

/**
 * @file SearchView.cpp
 *
 * @brief Implementation of functions in SearchView and its helper functions
 */

bool startsWith(const string &str, const string &prefix)
{
    if (str.length() < prefix.length())
    {
        return false; // String is shorter than the prefix, cannot start with it
    }

    for (size_t i = 0; i < prefix.length(); ++i)
    {
        if (tolower(str[i]) != tolower(prefix[i]))
        {
            return false; // Characters don't match
        }
    }

    return true;
}

/**
 * \brief Searches for events in the given EventDictionary that have titles starting with the specified needle.
 *
 * This function iterates through the events stored in the EventDictionary and its repeat events, and checks if the
 * title of each event starts with the specified needle string. It keeps track of the number of matched events and
 * limits the number of found events to a maximum of 15. For each matched event, a copy is created and added to the
 * vector of foundEvents.
 *
 * \param storage Pointer to the EventDictionary object containing the events to search through.
 * \param needle The string used as the search criterion. Only events with titles starting with this string are considered.
 *
 * \return A vector of pointers to Event objects that match the search criterion.
 */
vector<Event *> match(EventDictionary *storage, const string &needle)
{
    int count = 0;
    vector<Event *> foundEvents;
    for (const Event *event : storage->getEvents())
    {
        // Limiting events within 15
        if (startsWith(event->getTitle(), needle) && count < 15)
        {
            foundEvents.push_back(event->makeCopy());
            count++;
        }
    }
    for (const Event *event : storage->getRepeatEvents())
    {
        // Limiting events within 15
        if (startsWith(event->getTitle(), needle) && count < 15)
        {
            foundEvents.push_back(event->makeCopy());
            count++;
        }
    }
    return foundEvents;
}

/**
 * \brief Performs a search for events in the given EventDictionary.
 *
 * This function allows the user to search for events by typing a search query in a window. The search is performed
 * by matching the input against event titles using the match() function. The user can navigate through the search
 * results and select an event to view its details. The function uses the ncurses library for displaying the user
 * interface.
 *
 * \param win Pointer to the ncurses window where the search interface is displayed.
 * \param storage Pointer to the EventDictionary object containing the events to search through.
 *
 * \return An integer representing the state of the program after the search:
 *         - 0: The search operation completed successfully.
 *         - Any other value: An error or a specific state as defined by the program.
 */
int search(WINDOW *win, EventDictionary *storage)
{
    int COLS = getmaxx(win);
    int ROWS = getmaxy(win);
    WINDOW *inWindow = subwin(win, ROWS - 5, COLS - 3, 4, 2);
    wattron(win, A_BOLD);
    mvwprintw(win, 2, COLS / 2 - 6, "Search Events");
    wattroff(win, A_BOLD);
    wrefresh(win);

    int ch = KEY_BACKSPACE;
    string input = "";
    vector<Event *> foundEvents;
    int selected = 0;
    // ESC to leave
    do
    {
        wattron(win, A_BOLD);
        mvwprintw(win, 2, COLS / 2 - 6, "Search Events");
        wattroff(win, A_BOLD);
        wrefresh(win);
        switch (ch)
        {
        case KEY_BACKSPACE:
        case KEY_DC:
        case 127:
            if (!input.empty() && selected == 0 && input.size() < 80)
                input.pop_back();
            break;
        case KEY_ENTER:
        case '\n':
            if (selected)
            {
                wclear(win);
                wclear(inWindow);
                int state = GetEventView(win, storage, foundEvents[selected - 1]);
                for (const Event *event : foundEvents)
                {
                    delete event;
                }
                foundEvents = vector<Event *>();
                box(win, 0, 0);
                delwin(inWindow);
                return state;
            }
            break;
        case KEY_DOWN:
            selected++;
            if (selected > static_cast<int>(foundEvents.size()))
                selected = 0;
            break;

        case KEY_UP:
            selected--;
            if (selected < 0)
            {
                selected = static_cast<int>(foundEvents.size());
            }
            break;
        default:
            if (selected == 0)
                input.push_back(static_cast<char>(ch));
            break;
        }

        wclear(inWindow);
        // The write page
        if (selected == 0)
            wattron(inWindow, COLOR_PAIR(3));

        wprintw(inWindow, "> ");
        wprintw(inWindow, "%s\n", input.c_str());
        wattroff(inWindow, COLOR_PAIR(3));

        wattron(inWindow, A_BOLD);
        wprintw(inWindow, "Results:\n");
        wattroff(inWindow, A_BOLD);

        for (const Event *event : foundEvents)
        {
            delete event;
        }
        foundEvents = vector<Event *>();
        foundEvents = match(storage, input);
        size_t i = 1;
        for (const Event *event : foundEvents)
        {
            if (selected == static_cast<int>(i))
                wattron(inWindow, COLOR_PAIR(3));
            event->renderInDays(inWindow, false);
            wattroff(inWindow, COLOR_PAIR(3));
            i++;
        }
        wrefresh(inWindow);

    } while ((ch = getch()) != 27);

    for (const Event *event : foundEvents)
    {
        delete event;
    }
    foundEvents = vector<Event *>();

    wclear(inWindow);
    delwin(inWindow);
    return 0;
}

int GetSearchView(WINDOW *main, EventDictionary *storage)
{
    int state = search(main, storage);
    wclear(main);
    return state;
}