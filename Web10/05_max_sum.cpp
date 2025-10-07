#include <iostream>
#include <vector>
#include <algorithm>
using namespace std;

int maxSubarraySum(vector<int>& arr) {
    int n = arr.size();
    if (n == 0) return 0;
    
    int max_ending_here = arr[0]; // вместо динамической таблицы
    int max_so_far = arr[0];      // максимум в динамической таблице
    
    for (int i = 1; i < n; i++) {
        max_ending_here = max(arr[i], max_ending_here + arr[i]);
        max_so_far = max(max_so_far, max_ending_here);
    }
    
    return max_so_far;
}

// Расширенная версия, которая возвращает и индексы
vector<int> maxSubarraySumWithIndices(vector<int>& arr) {
    int n = arr.size();
    if (n == 0) return {0, 0, 0};
    
    int max_ending_here = arr[0];
    int max_so_far = arr[0];
    int start = 0, end = 0, temp_start = 0;
    
    for (int i = 1; i < n; i++) {
        if (arr[i] > max_ending_here + arr[i]) {
            max_ending_here = arr[i];
            temp_start = i;
        } else {
            max_ending_here += arr[i];
        }
        
        if (max_ending_here > max_so_far) {
            max_so_far = max_ending_here;
            start = temp_start;
            end = i;
        }
    }
    
    return {max_so_far, start, end};
}

// Пример использования
int main() {
    vector<int> arr = {-2, 1, -3, 4, -1, 2, 1, -5, 4};
    int result = maxSubarraySum(arr);
    cout << "Maximum subarray sum: " << result << endl;
    
    auto detailed = maxSubarraySumWithIndices(arr);
    cout << "Sum: " << detailed[0] << ", from index " << detailed[1] 
         << " to " << detailed[2] << endl;
    return 0;
}