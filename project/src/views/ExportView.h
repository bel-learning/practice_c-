#ifndef EXPORTVIEW_H
#define EXPORTVIEW_H
#include <ncurses.h>
#include "../calendar.h"

/**
 * @file ExportView.h
 *
 * @brief Declaration of functions in ExportView
 */

int GetExportView(WINDOW *win, EventDictionary *storage);

#endif // EXPORTVIEW_H
