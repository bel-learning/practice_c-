#ifndef CALENDAR_H
#define CALENDAR_H
#include <string>
#include <vector>
#include <unordered_map>
#include <memory>
#include "event.h"
#include "datetime.h"

// We'll have Daily Calendar, Weekly Calendar, Monthly Calendar and such.
// All of them will have Printing function which is where the 
// polymorphism is used.

/**
 * @file calendar.h
 *
 * @brief Declaration of Calendar and its derived classes.
 */

/**
 * @struct Interval
 *
 * @brief A container for start and date datetimes, representing interval.
 */

/**
 * @class EventDictionary
 *
 * @brief A container containing all events.
 */

class EventDictionary {
public:
    /**
     * @brief Default constructor for EventDictionary.
     */
    EventDictionary();

    /**
     * @brief Copy constructor for EventDictionary.
     *
     * @param ed The EventDictionary object to be copied.
     */
    EventDictionary(const EventDictionary& ed);

    /**
     * @brief Constructs an EventDictionary object from a pointer to an existing EventDictionary.
     *
     * @param ed A pointer to the EventDictionary object to be copied.
     */
    EventDictionary(const EventDictionary* ed);

    /**
     * @brief Destructor for EventDictionary.
     */
    ~EventDictionary();

    /**
     * @brief Assignment operator for EventDictionary.
     *
     * @param ed The EventDictionary object to be assigned.
     * @return The assigned EventDictionary object.
     */
    EventDictionary operator=(const EventDictionary& ed);

    /**
     * @brief Adds an event to the EventDictionary.
     *
     * @param event A pointer to the Event object to be added.
     */
    void addEvent(const Event* event);

    /**
     * @brief Deletes an event EventDictionary.
     *
     * @param event A pointer to Event to delete.
     */
    void deleteEvent(const Event* event);
    /**
     * @brief Finds event and calls their change time function.
     *
     */
    void changeEventTime(const Event * event, const Interval & intl);

    /**
     * @brief Adds a repeatable event to the EventDictionary.
     *
     * @param event A pointer to the Event object to be added as a repeatable event.
     */
    void addRepeatEvent(const Event* event);

    /**
     * @brief Gets the vector of all events in the EventDictionary.
     *
     * @return A constant reference to the vector of Event pointers.
     */
    const vector<Event*>& getEvents() const;

    /**
     * @brief Gets the vector of repeatable events in the EventDictionary.
     *
     * @return A constant reference to the vector of repeatable Event pointers.
     */
    const vector<Event*>& getRepeatEvents() const;

private:
    // std::unordered_map<std::string, Event *> names;
    // std::unordered_map<std::string, Event *> places;
    std::vector<Event*> m_RepeatEvents; /**< The vector of repeatable events in the EventDictionary. */
    std::vector<Event*> m_Events; /**< The vector of all events in the EventDictionary. */
};

/**
 * @class Calendar
 *
 * @brief Calendar is the base class in the hierarchy. It is an abstract class.
 */
class Calendar {
public:
    /**
     * @brief Default constructor for Calendar.
     */
    Calendar();

    /**
     * @brief Constructor for Calendar.
     *
     * @param time The initial time for the Calendar.
     */
    Calendar(const Datetime& time);

    /**
     * @brief Assignment operator for Calendar.
     *
     * @param cal The Calendar object to be assigned.
     * @return The assigned Calendar object.
     */
    Calendar& operator=(const Calendar& cal);

    /**
     * @brief Virtual destructor for Calendar.
     */
    virtual ~Calendar();

    /**
     * @brief Gets the events within a specified time range.
     *
     * @param start The starting time of the range.
     * @param showAll Flag indicating whether to show all events or only the ones within the range.
     * @return A vector of Event pointers within the specified time range.
     */
    virtual const std::vector<Event*> getEvents(const Datetime& start, bool showAll) const = 0;

    /**
     * @brief Prints the Calendar view on the specified window.
     *
     * @param win The window to print the Calendar view on.
     * @param date The current date.
     * @param selected The index of the selected event.
     */
    virtual void print(WINDOW* win, const Datetime& date, int selected) const = 0;

    /**
     * @brief Displays the specified events on the window.
     *
     * @param win The window to display the events on.
     * @param events A vector of Event pointers to be displayed.
     * @param showAll Flag indicating whether to show all events or only the ones within the range.
     */
    virtual void displayEvents(WINDOW* win, const vector<Event*>& events, bool showAll = false) const = 0;

    /**
     * @brief Sets the EventDictionary for the Calendar to refer to.
     *
     * @param ed A pointer to the EventDictionary to be referred to.
     */
    void referDictionary(EventDictionary* ed) {
        m_Dictionary = ed;
    }

protected:
    std::string m_Username; /**< The username associated with the Calendar. */
    Datetime m_CurrentTime; /**< The current time of the Calendar. */
    EventDictionary* m_Dictionary; /**< Pointer to the EventDictionary for the Calendar. */
};

/**
 * @brief WeeklyCalendar class represents a weekly calendar.
 *
 * It is a derived class of the Calendar class.
 */
class WeeklyCalendar : public Calendar {
    public:
        WeeklyCalendar();
        ~WeeklyCalendar();
        const std::vector<Event *> getEvents(const Datetime & start, bool showAll) const override;
        void display(WINDOW * win, const Datetime & date, int active);
        void displayEvents(WINDOW * win, const vector<Event *> & events, bool showAll = false) const override;
        
        void print(WINDOW * win,const Datetime & date, int selected) const override;
    private:
};
/**
 * @brief DailyCalendar class represents a daily calendar.
 *
 * It is a derived of the Calendar class.
 */
class DailyCalendar : public Calendar {
    public:
        DailyCalendar();
        ~DailyCalendar();

        void print(WINDOW * win,const Datetime & date, int selected) const override;
        void display(WINDOW * win, const Datetime & date, int active);
        void displayEvents(WINDOW * win, const vector<Event *> & events, bool showAll = false) const override;
        
        const std::vector<Event *> getEvents(const Datetime & start, bool showAll) const override;


    private:
};
/**
 * @brief MonthlyCalendar class represents a daily calendar.
 *
 * It is a derived of the Calendar class.
 */
class MonthlyCalendar : public Calendar {
    public:
        MonthlyCalendar();
    
        ~MonthlyCalendar();


        void print(WINDOW * win,const Datetime & date, int selected) const override;
        void display(WINDOW * win, const Datetime & date, int selected) const;
        void displayEvents(WINDOW * win, const vector<Event *> & events, bool showAll = false) const override;

    
        const std::vector<Event *> getEvents(const Datetime & start, bool showAll) const override;

    private:
};






#endif //CALENDAR_H
