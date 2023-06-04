#ifndef IMPORTEXPORT_H
#define IMPORTEXPORT_H
#include<fstream>
#include "calendar.h"

void importEvents(EventDictionary * storage, std::ofstream & file);
void exportEvents(const EventDictionary * storage, std::ofstream & file);

#endif // IMPORTEXPORT_H