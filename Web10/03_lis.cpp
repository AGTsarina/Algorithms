#include <iostream>
#include <vector>
#include <algorithm>
using namespace std;

int lis(vector<int>& arr) {
    int n = arr.size();
    vector<int> dp(n, 1);
    int maxLen = 1;
    for (int i = 1; i < n; i++) {
        for (int j = 0; j < i; j++) {
            if (arr[j] < arr[i]) {
                dp[i] = max(dp[i], dp[j] + 1);
            }
        }
        maxLen = max(maxLen, dp[i]);
    }
    return maxLen;
}

int main() {
    vector<int> arr = {10, 9, 2, 5, 3, 7, 101, 18};
    cout << "Длина наибольшей возрастающей последовательности " << lis(arr) << endl;
    return 0;
}