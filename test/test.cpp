#include<iostream>
#include<vector>
#include<string>


using namespace std;

int main() {
    vector<string> data;
    data.push_back("dada");
    data.push_back("dada1");    
    data.push_back("dada2");    
    data.push_back("dada3");    
    data.push_back("dada3");    
    for(const auto & x: data) {
        cout << x << endl;
    }
}