#ifndef EVENT_H
#define EVENT_H
#include "datetime.h"
#include<ncurses.h>
#include<string>
#include<sstream>
#include<iostream>
using namespace std;

class Event {
    // There will be different kinds of events

    public:
    enum Repeat {Daily, Weekly, BiWeekly, Monthly, Yearly, None};

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
    virtual void renderInHours(WINDOW * win) const = 0;
    virtual void renderInDays(WINDOW * win) const = 0;
    virtual stringstream toFile() const = 0;
    virtual Event * findNextRepeatable() const = 0;
    virtual Event * findPreviousRepeatable() const = 0;

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
    Event * findNextRepeatable() const override;
    Event * findPreviousRepeatable() const override;

    Event * makeCopy() const override;
    stringstream toFile() const  override;
    bool insideInterval(const Datetime & start, const Datetime & end) const override;
    void renderInHours(WINDOW * win) const override;
    void renderInDays(WINDOW * win) const override;

    void display() override;
    bool isLowerThan(const Event * e) const override;
    Datetime getCompareDate() const override;

    private:
    string m_Location;
    Datetime m_Start;
    Datetime m_End;
};

class Task : public Event {
    public:
    Task(const string & formatted);
    Task(const string & title, const string & description, Repeat repeat, const Datetime & start, const Datetime & end, bool finished);
    ~Task() {};
    
    Event * makeCopy() const override;
    bool insideInterval(const Datetime & start, const Datetime & end) const override;
    void renderInHours(WINDOW * win) const override;
    void renderInDays(WINDOW * win) const override;
    stringstream toFile() const  override;

    Event * findNextRepeatable() const override;
    Event * findPreviousRepeatable() const override;

    void display() override;
    bool isLowerThan(const Event * e) const override;
    Datetime getCompareDate() const override;

    private:
    Datetime m_Start;
    Datetime m_End;
    bool m_Finished;
};

class Deadline : public Event {
    public:
    Deadline(const string & formatted);
    Deadline(const string & title, const string & description, Repeat repeat, const Datetime & end);
    ~Deadline() {};
    
    Event * makeCopy() const override;
    bool insideInterval(const Datetime & start, const Datetime & end) const override;
    void renderInHours(WINDOW * win) const override;
    void renderInDays(WINDOW * win) const override;
    stringstream toFile() const  override;

    Event * findNextRepeatable() const override;
    Event * findPreviousRepeatable() const override;

    void display() override;
    bool isLowerThan(const Event * e) const override;
    Datetime getCompareDate() const override;

    private:
    Datetime m_End;
};



#endif // EVENT_H
