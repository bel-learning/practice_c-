#include<iostream>
#include<string>
#include<vector>
using namespace std;

class CComponent {
public:
    // int m_Size;
    // int m_Pos;
    int m_Size;
    int m_Pos;
    CComponent() = delete;
    CComponent(int size, int pos) {
        m_Size = size;
        m_Pos = pos;
    }
    virtual void printName() const {}

};
class CButton : public CComponent {
public:
    string m_Name;

    CButton(int size, int pos, string name) : CComponent(size, pos), m_Name(name) {    };
    void printName() const override {
        cout << m_Name << endl;
    }
private:
} ;

class CLabel : public CComponent {
public:
    string m_Name;

    CLabel(int size, int pos, string name) : CComponent(size, pos), m_Name(name) { };
    void printName() const override {
        cout << m_Name << endl;
    }
private:
};

int main() {
    CButton a(1,2,"button1");
    CLabel b(1,3,"label1");
    vector<CComponent *> objects;
    objects.push_back(&a);
    objects.push_back(&b);

    objects[0]->printName();
    vector<CComponent> obj;
    obj.push_back(a);
    obj[0].printName();
}