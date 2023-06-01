#include "event.h"
#include <string>

using namespace std;

Event::Event(const string & title, const string & description, Repeat repeat)
    : m_Title(title), m_Description(description), m_Repeat(repeat)
{}
Event::Event(const Event * event) : m_Title(event->m_Title), m_Description(event->m_Description), m_Repeat(event->m_Repeat) 
{};
Event::Event(const Event & event) : m_Title(event.m_Title), m_Description(event.m_Description), m_Repeat(event.m_Repeat) 
{};
Event::~Event() {}
Event Event::operator= (const Event & event) {
    if(*this == event) return *this;
    m_Title = event.m_Title;
    m_Description = event.m_Description;
    m_Repeat = event.m_Repeat;
    return *this;
}

bool operator == (const Event & event1, const Event & event2) {
    if(event1.m_Title != event2.m_Title) return false;
    if(event1.m_Description != event2.m_Description) return false;
    if(event1.m_Repeat != event2.m_Repeat) return false;
    return true;
}

void Event::display() {};

Event * Event::makeCopy() const {
    return new Event(this);
}

NormalEvent::NormalEvent(const Event & event, const string & location, Datetime start, Datetime end)
    : Event(event), m_Location(location), m_Start(start), m_End(end) {};

// NormalEvent::NormalEvent(const NormalEvent & event) 

void NormalEvent::display() {};

Event * NormalEvent::makeCopy() const {
    return new NormalEvent(Event(this), m_Location, m_Start, m_End);
};