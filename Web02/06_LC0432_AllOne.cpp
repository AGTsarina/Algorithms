#include <map>
#include <vector>
#include <string>
#include <algorithm>
using namespace std;

class AllOne {
    map<string, int> words;
    map<int, vector<string>> nums;
public:
    AllOne() {
        
    }

    void remove(string key){
        auto& vec = nums[words[key]];
        auto to_remove = find(vec.begin(), vec.end(), key);
        nums[words[key]].erase(to_remove);
        if (nums[words[key]].size() == 0) nums.erase(words[key]);
    }
    
    void inc(string key) {
        if (words.find(key) != words.end()){
            remove(key);
            words[key]++;
        }
        else words[key] = 1;
        nums[words[key]].push_back(key);
    }
    
    void dec(string key) {
        remove(key);
        words[key]--;
        if (!words[key]) words.erase(key);
        else nums[words[key]].push_back(key);
    }
    
    string getMaxKey() {
        if (nums.size() == 0) return "";
        return nums.rbegin()->second.front();
    }
    
    string getMinKey() {
        if (nums.size() == 0) return "";
        return nums.begin()->second[0];
    }
};