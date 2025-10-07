#include <vector>
using namespace std;

// Наивная рекурсия - НЕЭФФЕКТИВНО
long fib_naive(long n){
    if (n <= 1) return n;
    return fib_naive(n-1) + fib_naive(n-2);
}
// ДП с запоминанием
long fib_dp(long n, vector<long> &memo){
    if (memo[n] > 0)
        return memo[n];
    if (n <= 1){
        memo[n] = n;
        return n;
    }
    memo[n] = fib_dp(n-1, memo) + fib_dp(n-2, memo);
    return memo[n];
}

long fib_bu(long n){
    if (n <= 1)
        return n;
    vector<long> dp(n + 1, 0);
    dp[1] = 1;
    for (long i = 2; i < n + 1; i++)
        dp[i] = dp[i-1] + dp[i-2];
    return dp[n];
}

long fib_mem_opt(long n){
    if (n <= 1)
        return n;
    long prev0 = 0, prev1 = 1;
    long res;    
    for (long i = 3; i < n + 1; i++){
        res = prev0 + prev1;
        prev0 = prev1;
        prev1 = res;
    }
    return res;
}

int main(){
    int value = 7;
    long res_naive = fib_naive(value);
    vector<long> memo(value + 1, 0);
    long res_dp = fib_dp(value, memo);
    long res_bu = fib_bu(value);
    return 0;
}
