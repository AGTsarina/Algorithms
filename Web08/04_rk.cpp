#include <string>
#include <iostream>
#include <vector>
#include <cmath>

using namespace std;

const int BASE = 256;
const int PRIME = 101;

long calculateHash(const string& str, int start, int end) {
    long hash = 0;
    for (int i = start; i < end; i++) {
        hash = (hash * BASE + str[i]) % PRIME;
    }
    return hash;
}

long updateHash(long oldHash, char oldChar, char newChar, int patternLen) {
    long hash = (oldHash - oldChar * (long)(pow(BASE, patternLen - 1))) % PRIME;
    if (hash < 0) hash += PRIME;
    hash = (hash * BASE + newChar) % PRIME;
    return hash;
}

vector<int> RabinKarp(const string& text, const string& pattern) {
    vector<int> res;
    int n = text.length();
    int m = pattern.length();
    
    if (m == 0 || n < m) return res;
    
    long patternHash = calculateHash(pattern, 0, m);
    long textHash = calculateHash(text, 0, m);
    
    for (int i = 0; i <= n - m; i++) {
        if (patternHash == textHash) {
            // Проверка на коллизию
            bool match = true;
            for (int j = 0; j < m; j++) {
                if (text[i + j] != pattern[j]) {
                    match = false;
                    break;
                }
            }
            if (match) {
                res.push_back(i);                
            }
        }
        
        // Пересчет хеша для следующего окна
        if (i < n - m) {
            textHash = updateHash(textHash, text[i], text[i + m], m);
        }
    }
    return res;
}

int main(){
    string text = "This is a text to search words. The words are lined one by one.";
    string word = "word";
    auto res = RabinKarp(text, word);
    for(const auto & index: res){
        cout << index <<" ";
    }
    cout << endl;
}