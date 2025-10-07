#include <iostream>
#include <vector>
#include <algorithm>
using namespace std;

struct Solution {
    int weight, value;
};

vector<Solution> generateSubsets(vector<int>& weights, vector<int>& values, int start, int end) {
    int n = end - start;
    vector<Solution> solutions;
    
    for (int mask = 0; mask < (1 << n); mask++) {
        int total_weight = 0, total_value = 0;
        for (int i = 0; i < n; i++) {
            if (mask & (1 << i)) {
                total_weight += weights[start + i];
                total_value += values[start + i];
            }
        }
        solutions.push_back({total_weight, total_value});
    }
    
    sort(solutions.begin(), solutions.end(), [](const Solution& a, const Solution& b) {
        return a.weight < b.weight || (a.weight == b.weight && a.value > b.value);
    });
    
    vector<Solution> filtered;
    int max_value = -1;
    for (auto& sol : solutions) {
        if (sol.value > max_value) {
            filtered.push_back(sol);
            max_value = sol.value;
        }
    }
    
    return filtered;
}

int meetInMiddle(int W, vector<int>& weights, vector<int>& values) {
    int n = weights.size();
    int mid = n / 2;
    
    // Генерируем подмножества для двух половин
    vector<Solution> left = generateSubsets(weights, values, 0, mid);
    vector<Solution> right = generateSubsets(weights, values, mid, n);
    
    // Сортируем правую часть по весу
    sort(right.begin(), right.end(), [](const Solution& a, const Solution& b) {
        return a.weight < b.weight;
    });
    
    // Префиксные максимумы для правой части
    vector<int> prefix_max(right.size());
    for (int i = 0; i < right.size(); i++) {
        prefix_max[i] = right[i].value;
        if (i > 0) prefix_max[i] = max(prefix_max[i], prefix_max[i-1]);
    }
    
    int best_value = 0;
    
    // Комбинируем решения
    for (auto& left_sol : left) {
        if (left_sol.weight > W) continue;
        
        int remaining = W - left_sol.weight;
        
        // Бинарный поиск в правой части
        int l = 0, r = right.size() - 1;
        int best_right = 0;
        
        while (l <= r) {
            int m = (l + r) / 2;
            if (right[m].weight <= remaining) {
                best_right = prefix_max[m];
                l = m + 1;
            } else {
                r = m - 1;
            }
        }
        
        best_value = max(best_value, left_sol.value + best_right);
    }
    
    return best_value;
}

int main(){
    vector<int> weights = {2, 5, 7, 3, 1, 4, 8, 6, 9, 3};
    vector<int> values =  {10, 15, 25, 8, 5, 12, 30, 18, 35, 9};
    int W = 20;
    
    cout << "Maximum value: " << meetInMiddle(W, weights, values) << endl;
    return 0;
}