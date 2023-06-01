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

class EventDictionary {
    public:
    EventDictionary() {};
    private:
    std::unordered_map<std::string, std::shared_ptr<Event> > names;
    std::unordered_map<std::string, std::shared_ptr<Event> > places;

};

class Calendar {
    // Calendar is biggest in hierarchy. Abstract Class
    public:
        Calendar() ;
        Calendar(const Datetime & time) ;
        Calendar & operator = (const Calendar & cal);
        virtual ~Calendar() {};
        virtual const std::vector<std::shared_ptr<Event> > & getEvents();
        virtual void print() const;
        bool addEvent(const Event & e);
        
    private:
        std::string username;
        std::vector<std::shared_ptr<Event> > events;
        Datetime currentTime;
        // For searching capabilities
        EventDictionary dictionary;
};


class WeeklyCalendar : public Calendar {
    public:
        WeeklyCalendar();
        WeeklyCalendar(const Datetime & time) ;

        void print() const override;
    private:
};

class DailyCalendar : public Calendar {
    public:
        DailyCalendar();
        void print() const override;

    private:
};

class MonthlyCalendar : public Calendar {
    public:
        MonthlyCalendar();
        void print() const override;

    private:
};







#endif //CALENDAR_H
