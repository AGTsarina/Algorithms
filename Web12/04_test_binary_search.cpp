#include <iostream>
#include <vector>
#include <chrono>
#include <cmath>

class BinarySearchAnalyzer {
private:
    static int binarySearchRecursive(const std::vector<int>& arr, int low, int high, 
                                   int target, int& comparisons, int& recursiveCalls) {
        recursiveCalls++;
        
        // Базовый случай
        if (low > high) {
            return -1;
        }
        
        int mid = low + (high - low) / 2;
        comparisons++;
        
        if (arr[mid] == target) {
            return mid;
        } else if (arr[mid] > target) {
            return binarySearchRecursive(arr, low, mid - 1, target, comparisons, recursiveCalls);
        } else {
            return binarySearchRecursive(arr, mid + 1, high, target, comparisons, recursiveCalls);
        }
    }

public:
    static void analyzeSearch(const std::vector<int>& data, int target, const std::string& caseName) {
        int comparisons = 0;
        int recursiveCalls = 0;
        
        auto start = std::chrono::high_resolution_clock::now();
        int result = binarySearchRecursive(data, 0, data.size() - 1, target, comparisons, recursiveCalls);
        auto end = std::chrono::high_resolution_clock::now();
        
        auto duration = std::chrono::duration_cast<std::chrono::nanoseconds>(end - start);
        
        std::cout << "Случай: " << caseName << " (n=" << data.size() << ")" << std::endl;
        std::cout << "  Целевой элемент: " << target << std::endl;
        std::cout << "  Результат: " << (result != -1 ? "Найден" : "Не найден") << std::endl;
        std::cout << "  Сравнения: " << comparisons << std::endl;
        std::cout << "  Рекурсивные вызовы: " << recursiveCalls << std::endl;
        std::cout << "  Ожидаемое O(log n): " << log2(data.size()) << std::endl;
        std::cout << "  Время: " << duration.count() << " нс" << std::endl;
        std::cout << std::endl;
    }
    
    static void demonstrateCases() {
        // Создаем отсортированный массив
        std::vector<int> sortedArray;
        for (int i = 0; i < 1000; i++) {
            sortedArray.push_back(i * 2); // Четные числа: 0, 2, 4, ..., 1998
        }
        
        // Анализируем разные случаи
        analyzeSearch(sortedArray, 248, "Средний случай");   // Где-то в массиве
        analyzeSearch(sortedArray, 998, "Лучший случай");      // Центральный элемент
        analyzeSearch(sortedArray, 1998, "Худший случай");   // Последний элемент
        analyzeSearch(sortedArray, 501, "Не найден");        // Нечетное число
    }
};

int main(){
    BinarySearchAnalyzer::demonstrateCases();
}