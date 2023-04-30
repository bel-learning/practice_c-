#include<iostream>
using namespace std;

class Cnum {
    public:
    Cnum (int a) {m_X = a;}
    Cnum operator + (const Cnum & x) {
        m_X += x.m_X;
        return *this;
    }
    void print(void) const
 {
    cout << m_X << endl;
 }    private:
    int m_X;
};
int main() {
    Cnum a(6);
    Cnum b = a + 6;
    b.print();
}