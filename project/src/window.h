#ifndef WINDOW_H
#define WINDOW_H
#include<ncurses.h>
#include<vector>
#include<utility>


class Window {
    public:
    Window() = delete;
    Window(int ROW , int COL );
    Window(int ROW , int COL, int x, int y, Window * parent = nullptr);
    
    ~Window();
    int getInput();
   
    std::pair<int,int> getSize();
    

    int & getX();
    int & getY();
    int & getRow();
    int & getCol();
    WINDOW * getWindow();
    void refresh();


    void printCharAt(int row, int col, char ch);
    void drawButton(int y, int x, const std::string& label, bool selected);
    void addWindow(int rows, int cols, int y, int x); 

    void writeHorizontalCenter(const char * str, int paddingUp = 2) {

    }

    std::string getWriteInput();
    private:
    int m_Row;
    int m_Col;
    int m_X;
    int m_Y;
    int m_Last_Write_Row;
    int m_Last_Write_Col;

    WINDOW * m_Win;
    std::vector<Window *> m_Objects;
};




#endif // WINDOW_H
