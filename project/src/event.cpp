#include "event.h"
#include <string>
#include <sstream>
#include <iostream>
#include <iomanip>

using namespace std;

Event::Event(const string & title, const string & description, Repeat repeat)
    : m_Title(title), m_Description(description), m_Repeat(repeat)
{}
Event::Event(const Event * event) : m_Title(event->m_Title), m_Description(event->m_Description), m_Repeat(event->m_Repeat) 
{};
Event::Event(const Event & event) : m_Title(event.m_Title), m_Description(event.m_Description), m_Repeat(event.m_Repeat) 
{};
Event::~Event() {}
// Event Event::operator= (const Event & event) {
//     if(*this == event) return *this;
//     m_Title = event.m_Title;
//     m_Description = event.m_Description;
//     m_Repeat = event.m_Repeat;
//     return *this;
// }

const string & Event::getTitle() const {
    return m_Title;
}
const string & Event::getDescription() const {
    return m_Description;
}
const Event::Repeat & Event::getRepeat() const {
    return m_Repeat;
}

bool operator == (const Event & event1, const Event & event2) {
    if(event1.m_Title != event2.m_Title) return false;
    if(event1.m_Description != event2.m_Description) return false;
    if(event1.m_Repeat != event2.m_Repeat) return false;
    return true;
}

// void Event::display() {};


NormalEvent::NormalEvent(const Event & event, const string & location, Datetime start, Datetime end)
    : Event(event), m_Location(location), m_Start(start), m_End(end) {};

NormalEvent::NormalEvent(const string & title, const string & description, Repeat repeat, const string & location, const Datetime & start, const Datetime & end) 
    : Event(title, description, repeat), m_Location(location), m_Start(start), m_End(end) {};

// NormalEvent::NormalEvent(const NormalEvent & event) 
bool NormalEvent::insideInterval(const Datetime & start, const Datetime & end) const {
    if(m_Start >= start && m_End <= end)
        return true;
    return false;
}

void NormalEvent::display() {};

Event * NormalEvent::makeCopy() const {
    return new NormalEvent(this->getTitle(), this->getDescription(), this->getRepeat(), m_Location, m_Start, m_End);
};

string NormalEvent::toString() const {
    // str.append()
    stringstream ss;
    ss << std::setfill('0');  // Set fill character to '0' if the hour/minute is single digit
    ss << std::setw(2) << m_Start.hour << ":" << std::setw(2) << m_Start.minute << "-"
       << std::setw(2) << m_End.hour << ":" << std::setw(2) << m_End.minute;
    // str.append(m_Start.toString() + "\n");
    // str.append(m_End.toString() + "\n");
    ss << " " << getTitle();
    return ss.str();
};

// bool NormalEvent::insideInterval(const Datetime & start, const Datetime & end) const {
//     return true;
// }


// void NormalEvent::display() {};

// Event * NormalEvent::makeCopy() const {
//     return new NormalEvent(this->getTitle(), this->getDescription(), this->getRepeat(), m_Location, m_Start, m_End);
// };

// bool NormalEvent::insideInterval(const Datetime & start, const Datetime & end) const {
//    return true
// }


// void NormalEvent::display() {};

// Event * NormalEvent::makeCopy() const {
//     return new NormalEvent(this->getTitle(), this->getDescription(), this->getRepeat(), m_Location, m_Start, m_End);
// };