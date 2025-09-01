#include <iostream>
#include <vector>
#include <unordered_set>

using namespace std;

vector<int> findIntersection(const vector<int>& nums1, const vector<int>& nums2) {
    // Создаем хеш-таблицу (множество) из элементов первого массива
    unordered_set<int> set(nums1.begin(), nums1.end());
    
    // Вектор для хранения результата
    vector<int> intersection;
    
    // Проходим по второму массиву
    for (int num : nums2) {
        // Если элемент найден в хеш-таблице
        if (set.find(num) != set.end()) {
            // Добавляем в результат
            intersection.push_back(num);
            // Удаляем из хеш-таблицы, чтобы избежать дубликатов
            set.erase(num);
        }
    }    
    return intersection;
}

int main() {    
    vector<int> arr1 = {1, 2, 3, 4, 5, 3, 2};
    vector<int> arr2 = {3, 4, 5, 6, 7, 3, 2, 3, 2};
    
    vector<int> result = findIntersection(arr1, arr2);
    
    cout << "Пересечение массивов: ";
    for (int num : result) {
        cout << num << " ";
    }
    cout << endl;
    
    return 0;
}