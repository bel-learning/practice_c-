#ifndef EVENT_H
#define EVENT_H
#include "datetime.h"
#include<string>

using namespace std;

class Event {
    // There will be different kinds of events

    public:
    enum Repeat {Daily, Weekly, BiWeekly, Monthly, Yearly};

    Event(const string & title, const string & description, Repeat repeat);
    // Copy constructor
    Event(const Event & event);
    // Assignment operator

    friend bool operator == (const Event & event1, const Event & event2);
    Event(const Event * event);
    virtual ~Event();
    virtual void display() = 0;
    virtual bool insideInterval(const Datetime & start, const Datetime & end) const = 0;
    virtual Event * makeCopy() const = 0;
    virtual string toHours() const = 0;
    virtual string toDays() const = 0;


    virtual bool isLowerThan(const Event * e) const = 0;
    virtual Datetime getCompareDate() const = 0;
    const string & getTitle() const;
    const string & getDescription() const;
    const Repeat & getRepeat() const;

    private:
    string m_Title;
    string m_Description;
    Repeat m_Repeat;
};
class NormalEvent : public Event {
    public:
    NormalEvent(const Event & event, const string & location, Datetime start, Datetime end);
    NormalEvent(const string & title, const string & description, Repeat repeat, const string & location, const Datetime & start, const Datetime & end);
    // Copy constructor
    NormalEvent(const NormalEvent & event);
    // Assignment operator
    NormalEvent operator = (const NormalEvent & event);

    ~NormalEvent() {};

    Event * makeCopy() const override;
    bool insideInterval(const Datetime & start, const Datetime & end) const override;
    string toHours() const override;
    string toDays() const override;

    void display() override;
    bool isLowerThan(const Event * e) const override;
    Datetime getCompareDate() const override;

    private:
    string m_Location;
    Datetime m_Start;
    Datetime m_End;
};

// class Task : public Event {
//     public:
//     Task(const Task & event);
//     // Assignment operator
//     Task operator = (const Task & event);
//     Event * makeCopy() const override;
//     bool insideInterval(const Datetime & start, const Datetime & end) const override;
//     void display() override;
//     private:
// };
// class Deadline : public Event {
//     public:
//     Event * makeCopy() const override;
//     void display() override;
//     private:
// };
// class Reminder : public Event {
//     public:
//     Reminder(const Reminder & event);
//     // Assignment operator
//     Reminder operator = (const Reminder & event);
//     bool insideInterval(const Datetime & start, const Datetime & end) const override;

//     Event * makeCopy() const override;
//     void display() override;
//     private:
//     Datetime m_End;
// };


#endif // EVENT_H
