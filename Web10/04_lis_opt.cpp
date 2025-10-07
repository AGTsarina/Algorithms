#include <iostream>
#include <vector>
#include <algorithm>
using namespace std;

int lis(vector<int>& nums) {
    if (nums.empty()) return 0;
    
    vector<int> tails;
    
    for (int x : nums) {
        // Бинарный поиск позиции для вставки
        auto it = lower_bound(tails.begin(), tails.end(), x);
        
        if (it == tails.end()) {
            // x больше всех элементов - добавляем в конец
            tails.push_back(x);
        } else {
            // Заменяем элемент на x
            *it = x;
        }
    }
    
    return tails.size();
}

int main() {
    vector<int> nums = {10, 9, 2, 5, 3, 7, 101, 18};
    cout << "Длина наибольшей возрастающей последовательности " << lis(nums) << endl;
    return 0;
}