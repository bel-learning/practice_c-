#include <string>
#include <vector>
#include <unordered_map>
#include <memory>
#include "event.h"
#include "calendar.h"
#include "datetime.h"
#include "util.h"
#include "window.h"

Calendar::Calendar() {};
Calendar::Calendar(const Datetime & time) : currentTime(time) {}

void Calendar::print() const {
    

}

