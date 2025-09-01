#include <iostream>
#include <unordered_map>
#include <string>
#include <vector>

using namespace std;

int main() {
    vector<string> programmingWords = {
        "function", "variable", "class", "object", "method",
        "function", "variable", "algorithm", "class", "object"        
    };    
    unordered_map<string, int> wordCount;    
    
    for (const auto& word : programmingWords) {
        wordCount[word]++;
    }    
    
    cout << "Частота слов:" << endl;    
    for (const auto& pair : wordCount) {
        cout << pair.first << ": " << pair.second << " раз(а)" << endl;
    }
}