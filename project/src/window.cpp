#include<ncurses.h>
#include<utility>
#include<string>
#include<iostream>
#include<vector>
#include "window.h"

using namespace std;

Window::Window(int ROW , int COL, int x, int y, Window * parent) : m_Row(ROW), m_Col(COL), m_X(x), m_Y(y) {
    // m_Win = newwin(m_Row, m_Col, m_X, m_Y);
    if(parent != nullptr) {
        m_Win = subwin(parent->getWindow(), m_Row, m_Col, m_X, m_Y);
        keypad(m_Win, TRUE);
    }
    else 
        m_Win = initscr();
    // keypad(m_Win, TRUE);
    box(m_Win, 0,0);

    raw();
    noecho();
    start_color();
    cbreak();
    // mvwprintw(m_Win, 20, 20, "hello pls ");
    // curs_set(0);
    
    refresh();
    wrefresh(m_Win);
}
Window::~Window() {
    if(m_Win != nullptr)
        delwin(m_Win);    
}

int Window::getInput() {
    return getch();
}

void Window::printCharAt(int row, int col, char ch) {
    mvwaddch(this->m_Win, row, col, ch);
    wrefresh(m_Win);
}

void Window::drawButton(int y, int x, const std::string& label, bool selected) {
    if (selected) {
        wattron(this->m_Win, A_REVERSE);   // Highlight the button if selected
    }
  
    mvwprintw(this->m_Win,y, x, "[%s]", label.c_str());
    wattroff(this->m_Win,A_REVERSE);  // Turn off highlighting
}

void Window::refresh() {
    wrefresh(this->m_Win);
}
int & Window::getX() {
    return m_X;
}
int & Window::getY() {
    return m_Y;
}
int & Window::getRow() {
    return m_Row;
}
int & Window::getCol() {
    return m_Col;
}
WINDOW * Window::getWindow() {
    return m_Win;
}


// string Window::getWriteInput() {
//     string buffer;
//     for(size_t i = 0; i < 100; i++) {
//         buffer.append("");
//     }
//     while (true) {
//         // Get user input
//         int ch = getch();
//         // Handle backspace and delete
//         if (ch == KEY_BACKSPACE || ch == KEY_DC || ch == 127 ) {
//             if (pos > 0) {
//                 pos--;
//                 buffer[pos] = ' ';
//                 move(0, pos);
//                 printw(" ");
//                 move(0, pos); // Move cursor back
//                 refresh();
//             }
//         }
//         // Handle Enter key to exit loop
//         else if (ch == '\n')
//             break;
//         // Handle regular input
//         else {
//             buffer[pos] = ch;
//             pos++;
//             printw("%c", ch);
//             refresh();
//         }
//     }
//     return buffer;
// }