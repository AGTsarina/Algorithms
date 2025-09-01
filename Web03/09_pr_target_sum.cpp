#include <iostream>
#include <vector>
#include <unordered_map>
using namespace std;

pair<int, int> twoSum(const vector<int>& nums, int target) {
    unordered_map<int, int> numMap; // число -> индекс
    
    for (int i = 0; i < nums.size(); ++i) {
        int complement = target - nums[i];
        if (numMap.count(complement)) {
            return {numMap[complement], i};
        }
        numMap[nums[i]] = i;
    }
    return {-1, -1}; 
}

int main(){
    vector<int> nums = {2, 11, 4, 7, 17, 15};
    auto res = twoSum(nums, 9); 
    return 0;
}

