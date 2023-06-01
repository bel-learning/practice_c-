#ifndef WINDOW_H
#define WINDOW_H
#include<ncurses.h>
#include<curses.h>
#include<utility>


class Window {
    public:
    Window() = delete;
    Window(int ROW, int COL) ;
    ~Window();
    int getInput();
   
    std::pair<int,int> getSize();
    

    int & getX() {
        return m_X;
    }
    int & getY() {
        return m_Y;
    }
    int & getRow() {
        return m_Row;
    }
    int & getCol() {
        return m_Col;
    }
    
    WINDOW * getWindow() {
        return m_Win;
    }
    private:
    int m_Row;
    int m_Col;
    int m_X;
    int m_Y;
    WINDOW * m_Win;
};


#endif // WINDOW_H
