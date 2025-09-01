#include <iostream>
#include <list>
#include <deque>
#include <vector>
#include <algorithm>

using namespace std;
const int ELEMENTS_COUNT=100000;


int main(){
    {
        vector<int> vec(ELEMENTS_COUNT);
        generate(vec.begin(), vec.end(), []() { static int n; return n++; });        
        
        auto mid = vec.begin() + vec.size() / 2;
        vec.erase(mid);
    }
    {
        list<int> lst;
        for (int i = 0; i < ELEMENTS_COUNT; ++i) {
            lst.push_back(i);
        }  
        auto mid = lst.begin();
        advance(mid, lst.size() / 2);
        lst.erase(mid);
    }
}