#include <iostream>
#include <vector>
#include <algorithm>
using namespace std;

int knapsack(int W, vector<int>& weights, vector<int>& values) {
    int n = weights.size();
    vector<vector<int>> dp(n + 1, vector<int>(W + 1, 0));
    
    for (int i = 1; i <= n; i++) {
        for (int w = 1; w <= W; w++) {
            if (weights[i-1] <= w) {
                dp[i][w] = max(dp[i-1][w], 
                              values[i-1] + dp[i-1][w - weights[i-1]]);
            } else {
                dp[i][w] = dp[i-1][w];
            }
        }
    }
    
    // Восстановление выбранных предметов
    vector<int> selected;
    int w = W;
    for (int i = n; i > 0 && w > 0; i--) {
        if (dp[i][w] != dp[i-1][w]) {
            selected.push_back(i-1);
            w -= weights[i-1];
        }
    }
    
    cout << "Selected items: ";
    for (int item : selected) {
        cout << item << " ";
    }
    cout << endl;
    
    return dp[n][W];
}

int main() {
    vector<int> values = {60, 100, 120};
    vector<int> weights = {10, 20, 30};
    int W = 50;
    
    cout << "Maximum value: " << knapsack(W, weights, values) << endl;
    return 0;
}