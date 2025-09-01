#include <iostream>
#include <unordered_map>
#include <string>

using namespace std;

bool isAnagram(const string& s, const string& t) {
    if (s.size() != t.size()) return false;
    
    unordered_map<char, int> countMap;
    
    for (char c : s) countMap[c]++;
    for (char c : t) {
        if (--countMap[c] < 0) return false;
    }
    
    return true;
}

int main(){
    string s1 = "aaaaaaaaaaa";
    string s2 = "aaa";
    cout << boolalpha << isAnagram(s1, s2); // true
}

// Пример вызова:
// cout << isAnagram("listen", "silent"); // true