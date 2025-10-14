#include <vector>
#include <string>
#include <iostream>
#include <unordered_map>

using namespace std;

unordered_map<char, int> buildBadCharTable(const string& pattern) {
    unordered_map<char, int> badChar;
    int m = pattern.length();
    
    for (int i = 0; i < m - 1; i++) {
        badChar[pattern[i]] = m - 1 - i;
    }
    
    return badChar;
}

vector<int> BoyerMoore(const string& text, const string& pattern) {
    vector<int> res;
    int n = text.length();
    int m = pattern.length();
    
    if (m == 0) return res;
    
    unordered_map<char, int> badChar = buildBadCharTable(pattern);
    int i = 0;
    
    while (i <= n - m) {
        int j = m - 1;
        
        // Сравнение справа налево
        while (j >= 0 && pattern[j] == text[i + j]) {
            j--;
        }
        
        if (j < 0) {
            res.push_back(i);
            i += (i + m < n) ? m - badChar[text[i + m]] : 1;
        } else {
            char bad_char = text[i + j];
            int shift = badChar.find(bad_char) != badChar.end() ? 
                       max(1, j - (m - 1 - badChar[bad_char])) : 
                       max(1, j + 1);
            i += shift;
        }
    }
    return res;
}

int main(){
    string text = "This is a text to search words. The words are lined one by one.";
    string word = "word";
    auto res = BoyerMoore(text, word);
    for(const auto & index: res){
        cout << index <<" ";
    }
    cout << endl;
}