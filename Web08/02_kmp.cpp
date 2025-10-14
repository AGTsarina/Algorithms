#include <vector>
#include <string>
#include <iostream>
#include <vector>

using namespace std;

vector<int> computePrefixFunction(const string& sample) {
    int m = sample.length();
    vector<int> pi(m, 0);
    int k = 0;
    
    for (int q = 1; q < m; q++) {
        while (k > 0 && sample[k] != sample[q]) {
            k = pi[k - 1];
        }
        if (sample[k] == sample[q]) {
            k++;
        }
        pi[q] = k;
    }
    return pi;
}

vector<int> KMP(const string& text, const string& sample) {
    vector<int> res;
    int n = text.length();
    int m = sample.length();
    
    if (m == 0) return res;
    
    vector<int> pi = computePrefixFunction(sample);
    int q = 0; // количество совпавших символов
    
    for (int i = 0; i < n; i++) {
        while (q > 0 && sample[q] != text[i]) {
            q = pi[q - 1];
        }
        if (sample[q] == text[i]) {
            q++;
        }
        if (q == m) {
            res.push_back(i - m + 1);
            q = pi[q - 1];
        }
    }
    return res;
}

int main(){
    string text = "This is a text to search words. The words are lined one by one.";
    string word = "word";
    auto res = KMP(text, word);
    for(const auto & index: res){
        cout << index <<" ";
    }
    cout << endl;
}