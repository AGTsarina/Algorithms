#include <iostream>
#include <unordered_set>
#include <vector>

using namespace std;

int main() {
    vector<string> programmingWords = {
        "function", "variable", "class", "object", "method",
        "function", "variable", "algorithm", "class", "object"
    };
    
    unordered_set<string> uniqueWords;    
    
    for (const auto& word : programmingWords) {
        uniqueWords.insert(word);
    }
    
    
    cout << "Уникальные слова:" << endl;    
    for (const auto& word : uniqueWords) {
        cout << word << endl;
    }    
    
    string searchWord = "algorithm";
    if (uniqueWords.find(searchWord) != uniqueWords.end()) {
        cout << "\nСлово найдено" << endl;
    }
}