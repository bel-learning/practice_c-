#ifndef CALENDAR_H
#define CALENDAR_H
#include <string>
#include <vector>
#include <unordered_map>
#include <memory>
#include "event.h"
#include "datetime.h"
#include "window.h"


// We'll have Daily Calendar, Weekly Calendar, Monthly Calendar and such.
// All of them will have Printing function which is where the 
// polymorphism is used.

class EventDictionary {
    public:
    EventDictionary() {};
    private:
    // std::unordered_map<std::string, Event * > names;
    // std::unordered_map<std::string, Event * > places;

};

class Calendar {
    // Calendar is biggest in hierarchy. Abstract Class
    public:
        Calendar() ;
        Calendar(const Datetime & time) ;
        Calendar & operator = (const Calendar & cal);
        virtual ~Calendar();
        virtual const std::vector<Event *> & getEvents() const;
        virtual void print(WINDOW * win, const Datetime & date, int selected) const;
        bool addEvent(const Event * e);
        
    protected:
        std::string m_Username;
        std::vector<Event *> m_Events;
        Datetime m_CurrentTime;
        // For searching capabilities
        EventDictionary m_Dictionary;
};


class WeeklyCalendar : public Calendar {
    public:
        WeeklyCalendar();
        ~WeeklyCalendar();
        const std::vector<Event *> & getEvents() const override;
        void display(WINDOW * win, const Datetime & date, int active);
        
        void print(WINDOW * win,const Datetime & date, int selected) const override;
    private:
};

class DailyCalendar : public Calendar {
    public:
        DailyCalendar();
        ~DailyCalendar();

        void print(WINDOW * win,const Datetime & date, int selected) const override;
        void display(WINDOW * win, const Datetime & date, int active);
        
        const std::vector<Event *> & getEvents() const override;


    private:
};

class MonthlyCalendar : public Calendar {
    public:
        MonthlyCalendar();
        ~MonthlyCalendar();


        void print(WINDOW * win,const Datetime & date, int selected) const override;
        void display(WINDOW * win, const Datetime & date, int selected) const;

    
        const std::vector<Event *> & getEvents() const override;

    private:
};






#endif //CALENDAR_H
