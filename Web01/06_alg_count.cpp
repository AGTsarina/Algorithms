#include <iostream>
#include <fstream>
#include <iomanip>
#include <vector>
#include <set>
#include <algorithm>

using namespace std;

int main() {
    set<int> s = {5, 8, 13, 2, 3, 4, 7};
    vector<int> v = {5, 8, 13, 5, 7, 2, 3, 4, 2, 2, 4, 7};

    // Считает количество 5 в упорядоченном множестве
    size_t number_5 = count(s.begin(), s.end(), 5);
    // Считает количество 2 в векторе
    size_t number_2 = count(v.begin(), v.end(), 2);

    // Считает количество чётных элементов в упорядоченном множестве
    size_t number_even = count_if(s.begin(), s.end(), [](const int&x){return x % 2 == 0;});
    // Считает количество нечётных элементов в векторе
    size_t number_odd = count_if(v.begin(), v.end(), [](const int&x){return x % 2;});
    return 0;
}
