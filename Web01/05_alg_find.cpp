#include <iostream>
#include <fstream>
#include <iomanip>
#include <vector>
#include <set>
#include <algorithm>

using namespace std;

int main() {
    set<int> s = {5, 8, 13, 2, 3, 4, 7};
    vector<int> v = {5, 8, 13, 2, 3, 4, 7};

    // Возвращает итератор на значение 5 в упорядоченном множестве
    auto iter_value_5 = find(s.begin(), s.end(), 5);
    // Возвращает итератор на v.end() в векторе, 
    // так как искомого элемента нет
    auto iter_no_value = find(v.begin(), v.end(), 100);

    // Находит первый чётный элемент в упорядоченном множестве
    auto iter_value_even = find_if(s.begin(), s.end(), [](const int&x){return x % 2 == 0;});
    // Находит первый нечётный элемент в векторе
    auto iter_value_odd = find_if_not(v.begin(), v.end(), [](const int&x){return x % 2 == 0;});
    return 0;
}
