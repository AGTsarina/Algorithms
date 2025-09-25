#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include <algorithm>
#include <map>
#include <set>
#include <cctype>
#include <sstream>
#include <iterator>

using namespace std;

vector<int> getSuffixArray(const string& text){
    int i=0, n = text.length();
    vector<pair<string, int>> words(n);    
       
    for(auto it = text.begin(); it != text.end(); it++, i++){         
        words[i].first = text.substr(i, n - i); 
        words[i].second = i;         
    }
    sort(words.begin(), words.end(), [](const auto& a, const auto& b) { return a.first < b.first; });
    vector<int> res(text.length());
    i=0;
    for(const auto & item: words) res[i++] = item.second;
    return res;
}

bool search(const string& pattern, const string& text, const vector<int>& suffixArray) {
    int left = 0, right = text.length() - 1;
    
    while (left <= right) {
        int mid = left + (right - left) / 2;
        int cmp = text.compare(suffixArray[mid], pattern.length(), pattern);
        
        if (cmp == 0) return true;
        if (cmp < 0) left = mid + 1;
        else right = mid - 1;
    }
    return false;
}



int main(){
    string pattern = "word";

    string text = "This is the text to search word. Let us find the word";  
    auto suffixArray = getSuffixArray(text);

    if (search(pattern, text, suffixArray)){
        cout << "Слово найдено\n";
    }
    else{
        cout << "Слово не найдено\n";
    }

}