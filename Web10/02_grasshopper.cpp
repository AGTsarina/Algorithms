#include <iostream>
#include <vector>
using namespace std;

long long waysToReach(int n) {
    if (n <= 0) return 0;
    if (n == 1) return 1;
    if (n == 2) return 1;
    if (n == 3) return 2;

    long long a = 1, b = 1, c = 2, d;
    for (int i = 4; i <= n; i++) {
        d = a + b + c;
        a = b;
        b = c;
        c = d;
    }
    return d;
}

int main() {
    int n;
    cout << "Введите n: ";
    cin >> n;
    cout << "Количество вариантов " << n << ": " << waysToReach(n) << endl;
    return 0;
}