#ifndef EVENT_H
#define EVENT_H
#include "datetime.h"
#include<string>
class Event {
    // There will be different kinds of events
    public:
    Event();
    ~Event();
    virtual void print();
    private:
    Datetime start;
    Datetime end;
    std::string title;
    std::string location;
};

class Task : public Event {
    public:
    private:
};
class Deadline : public Event {
    public:
    private:
};
class Reminder : public Event {
    public:
    private:
};


#endif // EVENT_H
