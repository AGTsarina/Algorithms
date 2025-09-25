#include <iostream>
#include <vector>
#include <string>
#include <algorithm>

using namespace std;

class SuffixTreeNode {
public:
    map<char, SuffixTreeNode*> children;
    vector<int> indices;
    
    void insertSuffix(const string& s, int index) {
        indices.push_back(index);
        if (!s.empty()) {
            char firstChar = s[0];
            if (children.find(firstChar) == children.end()) {
                children[firstChar] = new SuffixTreeNode();
            }
            children[firstChar]->insertSuffix(s.substr(1), index);
        }
    }
    
    vector<int> search(const string& pattern) {
        if (pattern.empty()) return indices;
        char firstChar = pattern[0];
        if (children.find(firstChar) != children.end()) {
            return children[firstChar]->search(pattern.substr(1));
        }
        return {};
    }
};

int main(){

}