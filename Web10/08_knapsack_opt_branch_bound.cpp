#include <iostream>
#include <vector>
#include <algorithm>
#include <functional>
using namespace std;

struct Item {
    int weight, value;
    double ratio;
};

class BranchBoundKnapsack {
private:
    vector<Item> items;
    int capacity;
    int best_value;
    
    int upperBound(int level, int current_weight, int current_value) {
        int remaining_capacity = capacity - current_weight;
        int bound = current_value;
        int i = level;
        
        while (i < items.size() && items[i].weight <= remaining_capacity) {
            remaining_capacity -= items[i].weight;
            bound += items[i].value;
            i++;
        }
        
        if (i < items.size()) {
            bound += remaining_capacity * items[i].ratio;
        }
        
        return bound;
    }
    
    void dfs(int level, int current_weight, int current_value) {
        if (current_weight > capacity) return;
        
        if (level == items.size()) {
            best_value = max(best_value, current_value);
            return;
        }
        
        // Проверка верхней границы
        if (upperBound(level, current_weight, current_value) <= best_value) {
            return;
        }
        
        // Включить текущий предмет
        dfs(level + 1, current_weight + items[level].weight, 
            current_value + items[level].value);
        
        // Исключить текущий предмет
        dfs(level + 1, current_weight, current_value);
    }
    
public:
    int solve(int W, vector<int>& weights, vector<int>& values) {
        capacity = W;
        best_value = 0;
        items.clear();
        
        // Сортируем предметы по убыванию отношения стоимость/вес
        for (int i = 0; i < weights.size(); i++) {
            items.push_back({weights[i], values[i], (double)values[i] / weights[i]});
        }
        
        sort(items.begin(), items.end(), [](const Item& a, const Item& b) {
            return a.ratio > b.ratio;
        });
        
        dfs(0, 0, 0);
        return best_value;
    }
};

// Пример использования
int main() {
    vector<int> values = {60, 100, 120};
    vector<int> weights = {10, 20, 30};
    int W = 50;

    BranchBoundKnapsack bbk;   
    
    cout << "Maximum value: " << bbk.solve(W, weights, values) << endl;
    return 0;
}