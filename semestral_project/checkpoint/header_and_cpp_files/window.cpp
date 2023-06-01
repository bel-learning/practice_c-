#include<ncurses.h>
#include<curses.h>
#include<utility>
#include "window.h"

using namespace std;

Window::Window(int ROW, int COL) : m_Row(ROW), m_Col(COL), m_X(0), m_Y(0) {
    initscr();
    cbreak();
    noecho();
    m_Win = newwin(ROW, COL, 0, 0);
    box(m_Win, 0,0);
    wrefresh(m_Win);
}
Window::~Window() {
    if(m_Win != nullptr)
        delwin(m_Win);    
    
}

int Window::getInput() {
    return getch();
}