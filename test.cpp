#include<iostream>

using namespace std;

void ZeroInit(int * ptr) { ptr= new int(0);}
int main(int argc, char ** argv) {
    int * intPtr = nullptr;
    ZeroInit(intPtr);
    cout << *intPtr << endl;
}