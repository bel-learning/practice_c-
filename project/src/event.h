#ifndef EVENT_H
#define EVENT_H
#include "datetime.h"
#include <ncurses.h>
#include <string>
#include <sstream>
#include <iostream>

using namespace std;

struct Interval
{
    Datetime start;
    Datetime end;
};
/**
 * @file event.h
 *
 * @brief Defines the Event class and its derived classes, NormalEvent, Task, and Deadline.
 */

/**
 * @class Event
 *
 * @brief Represents an event, which can be of different kinds. An Abstract class
 */
class Event
{
    // There will be different kinds of events

public:
    /**
     * @enum Repeat
     *
     * @brief Represents the repeat options for an event.
     */
    enum Repeat
    {
        Daily,
        Weekly,
        BiWeekly,
        Monthly,
        Yearly,
        None
    };

    Event(const string &title, const string &description, Repeat repeat);
    // Copy constructor
    Event(const Event &event);
    Event(const string &formatted);
    // Assignment operator

    friend bool operator==(const Event &event1, const Event &event2);
    Event(const Event *event);
    virtual ~Event();
    virtual void display() = 0;
    /**
     * @brief Checks if the event falls within the given interval.
     *
     * @param start The start of the interval.
     * @param end The end of the interval.
     * @return True if the event is inside the interval, false otherwise.
     */
    virtual bool insideInterval(const Datetime &start, const Datetime &end) const = 0;
    /**
     * @brief Creates a copy of the Event object.
     *
     * @return A pointer to the copied Event object.
     */
    virtual Event *makeCopy() const = 0;
    /**
     * @brief Renders the event in hours format.
     *
     * @param win The window to render the event in.
     * @param colored Flag indicating if the rendering should be colored.
     */
    virtual void renderInHours(WINDOW *win, bool colored = true) const = 0;

    /**
     * @brief Finds how much space event occupies.
     *
     */
    virtual long long getDifference() const = 0;

    /**
     * @brief Renders the event in days format.
     *
     * @param win The window to render the event in.
     * @param colored Flag indicating if the rendering should be colored.
     */
    virtual void renderInDays(WINDOW *win, bool colored = true) const = 0;
    /**
     * @brief Converts the event to a formatted string for file output.
     *
     * @return A stringstream containing the formatted event information.
     */
    virtual stringstream toFile() const = 0;
    /**
     * @brief Creates next repeatable event.
     *
     * @return A pointer to the next repeatable event.
     */
    virtual Event *findNextRepeatable() const = 0;
    /**
     * @brief Creates previous repeatable event.
     *
     * @return A pointer to the next repeatable event.
     */
    virtual Event *findPreviousRepeatable() const = 0;
    /**
     * @brief Renders the event in a formatted manner.
     *
     * @param win The window to render the event in.
     */
    virtual void renderFormatted(WINDOW *win) const = 0;
    /**
     * @brief Renders the event in a formatted manner.
     *
     * @param win The window to render the event in.
     */
    virtual Interval proposeNextInterval(int move) const = 0;

    /**
     * @brief Checks if the event is lower than the given event.
     *
     * @param e The Event object to compare against.
     * @return True if the event is lower than the given event, false otherwise.
     */
    virtual bool isLowerThan(const Event *e) const = 0;

    /**
     * @brief Gets the compare date for the event.
     *
     * @return The compare date of the event.
     */
    virtual Datetime getCompareDate() const = 0;

    /**
     * @brief Gets the title of the event.
     *
     * @return A constant reference to the title of the event.
     */
    const string &getTitle() const;

    /**
     * @brief Gets the description of the event.
     *
     * @return A constant reference to the description of the event.
     */
    const string &getDescription() const;

    /**
     * @brief Gets the repeat option of the event.
     *
     * @return A constant reference to the repeat option of the event.
     */
    const Repeat &getRepeat() const;
    /**
     * @brief Changes time for different events.
     *
     */
    virtual void changeTime(const Interval &intl) = 0;

private:
    string m_Title;       /**< The title of the event. */
    string m_Description; /**< The description of the event. */
    Repeat m_Repeat;      /**< The repeat option of the event. */
};
/**
 * @class NormalEvent
 *
 * @brief A derived class of Event. Has start and end as datetime
 */
class NormalEvent : public Event
{
public:
    NormalEvent(const Event &event, const string &location, Datetime start, Datetime end);
    NormalEvent(const string &title, const string &description, Repeat repeat, const string &location, const Datetime &start, const Datetime &end);
    // Copy constructor
    NormalEvent(const NormalEvent &event);
    // Assignment operator
    NormalEvent operator=(const NormalEvent &event);

    long long getDifference() const override;

    ~NormalEvent(){};
    Event *findNextRepeatable() const override;
    Event *findPreviousRepeatable() const override;
    void renderFormatted(WINDOW *win) const override;
    Event *makeCopy() const override;
    stringstream toFile() const override;
    bool insideInterval(const Datetime &start, const Datetime &end) const override;
    void renderInHours(WINDOW *win, bool colored = true) const override;
    void renderInDays(WINDOW *win, bool colored = true) const override;
    Interval proposeNextInterval(int move) const override;
    void changeTime(const Interval &intl) override;

    void display() override;
    bool isLowerThan(const Event *e) const override;
    Datetime getCompareDate() const override;

private:
    string m_Location;
    Datetime m_Start;
    Datetime m_End;
};
/**
 * @class Task
 *
 * @brief A derived class of Event. Difference is it has finished or not bool.
 */
class Task : public Event
{
public:
    Task(const string &title, const string &description, Repeat repeat, const Datetime &start, const Datetime &end, bool finished);

    ~Task(){};

    Event *makeCopy() const override;
    bool insideInterval(const Datetime &start, const Datetime &end) const override;
    void renderInHours(WINDOW *win, bool colored = true) const override;
    void renderInDays(WINDOW *win, bool colored = true) const override;
    stringstream toFile() const override;
    Task formattedToTask(const string &formatted) const;
    void renderFormatted(WINDOW *win) const override;
    long long getDifference() const override;
    Interval proposeNextInterval(int move) const override;
    void changeTime(const Interval &intl) override;

    Event *findNextRepeatable() const override;
    Event *findPreviousRepeatable() const override;

    void display() override;
    bool isLowerThan(const Event *e) const override;
    Datetime getCompareDate() const override;

private:
    Datetime m_Start;
    Datetime m_End;
    bool m_Finished;
};
/**
 * @class Deadline
 *
 * @brief A derived class of Event. HAs end as datetime
 */
class Deadline : public Event
{
public:
    Deadline(const string &title, const string &description, Repeat repeat, const Datetime &end);

    ~Deadline(){};

    Event *makeCopy() const override;
    bool insideInterval(const Datetime &start, const Datetime &end) const override;
    void renderInHours(WINDOW *win, bool colored = true) const override;
    void renderInDays(WINDOW *win, bool colored = true) const override;
    stringstream toFile() const override;
    void renderFormatted(WINDOW *win) const override;
    Interval proposeNextInterval(int move) const override;

    Event *findNextRepeatable() const override;
    Event *findPreviousRepeatable() const override;
    long long getDifference() const override;
    void changeTime(const Interval &intl) override;

    void display() override;
    bool isLowerThan(const Event *e) const override;
    Datetime getCompareDate() const override;

private:
    Datetime m_End;
};

#endif // EVENT_H
