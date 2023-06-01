#ifndef EVENT_H
#define EVENT_H
#include "datetime.h"
#include<string>
enum Repeat {Daily, Weekly, BiWeekly, Monthly, Yearly};

using namespace std;

class Event {
    // There will be different kinds of events
    public:
    Event(const string & title, const string & description, Repeat repeat);
    // Copy constructor
    Event(const Event & event);
    // Assignment operator
    Event operator = (const Event & event);

    friend bool operator == (const Event & event1, const Event & event2);
    Event(const Event * event);
    ~Event();
    virtual void display();
    virtual Event * makeCopy() const;
    private:
    string m_Title;
    string m_Description;
    Repeat m_Repeat;
};
class NormalEvent : public Event {
    public:
    NormalEvent(const Event & event, const string & location, Datetime start, Datetime end);
    // Copy constructor
    NormalEvent(const NormalEvent & event);
    // Assignment operator
    NormalEvent operator = (const NormalEvent & event);

    Event * makeCopy() const override;
    void display() override;
    private:
    string m_Location;
    Datetime m_Start;
    Datetime m_End;
};

class Task : public Event {
    public:
    Task(const Task & event);
    // Assignment operator
    Task operator = (const Task & event);
    Event * makeCopy() const override;
    void display() override;
    private:
};
class Deadline : public Event {
    public:
    Event * makeCopy() const override;
    void display() override;
    private:
};
class Reminder : public Event {
    public:
    Reminder(const Reminder & event);
    // Assignment operator
    Reminder operator = (const Reminder & event);

    Event * makeCopy() const override;
    void display() override;
    private:
    Datetime m_End;
};


#endif // EVENT_H
