#include<iostream>
#include<string>
#include<algorithm>

using namespace std;


class AnagramChecker {
public:
    static bool AreAnagrams(string a, string b) {
        size_t len1 = a.length();
        size_t len2 = b.length();
        if(len1 != len2) return false;

        sort(a.begin(), a.end());
        sort(b.begin(), b.end());
        
        for(size_t i = 0; i < len1; i++) {
            if(a[i] != b[i]) return false;
        }
        return true;
    }
};

int main() {
    AnagramChecker ac;
    string a = "ab";
    string b = "ba";
    cout << ac.AreAnagrams(a,b) << endl;
}