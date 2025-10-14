#include <iostream>
#include <vector>
#include <string>

using namespace std;

vector<int> search(string text, string sample){
    vector<int> res;
    auto search_end = text.end() - sample.length();
    for(auto search_start = text.begin(); search_start < search_end; search_start++){
        auto p = sample.begin();
        for(auto t = search_start; p!= sample.end() && *t == *p; t++, p++);
        if (p == sample.end()) res.push_back(search_start - text.begin());
    }
    return res;
}

int main(){
    string text = "This is a text to search words. The words are lined one by one.";
    string word = "word";
    auto res = search(text, word);
    for(const auto & index: res){
        cout << index <<" ";
    }
    cout << endl;
}