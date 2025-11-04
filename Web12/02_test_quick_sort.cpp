#include <iostream>
#include <vector>
#include <random>
#include <chrono>
using namespace std;

class QuickSortAnalyzer {
private:
    static int partition(vector<int>& arr, int low, int high, int& comparisons) {
        int pivot = arr[high];
        int i = low - 1;
        
        for (int j = low; j < high; j++) {
            comparisons++;
            if (arr[j] <= pivot) {
                i++;
                swap(arr[i], arr[j]);
            }
        }
        swap(arr[i + 1], arr[high]);
        return i + 1;
    }
    
    static void quickSortRecursive(vector<int>& arr, int low, int high, int& comparisons, int& recursiveCalls) {
        recursiveCalls++;
        if (low < high) {
            int pi = partition(arr, low, high, comparisons);
            quickSortRecursive(arr, low, pi - 1, comparisons, recursiveCalls);
            quickSortRecursive(arr, pi + 1, high, comparisons, recursiveCalls);
        }
    }

public:
    static void analyzeSort(const vector<int>& data, const string& caseName) {
        vector<int> arr = data;
        int comparisons = 0;
        int recursiveCalls = 0;
        
        auto start = chrono::high_resolution_clock::now();
        quickSortRecursive(arr, 0, arr.size() - 1, comparisons, recursiveCalls);
        auto end = chrono::high_resolution_clock::now();
        
        auto duration = chrono::duration_cast<chrono::microseconds>(end - start);
        
        cout << "Случай: " << caseName << " (n=" << data.size() << ")" << endl;
        cout << "  Сравнения: " << comparisons << endl;
        cout << "  Рекурсивные вызовы: " << recursiveCalls << endl;
        cout << "  Время: " << duration.count() << " мкс" << endl;
        cout << "  O(n log n) = " << data.size() * log2(data.size()) << endl;
        cout << "  O(n²) = " << data.size() * data.size() << endl;
        cout << endl;
    }
};

void demonstrateQuickSortCases() {
    std::vector<int> bestCase = {5, 3, 7, 1, 9, 2, 6, 4, 8}; // Сбалансированное разбиение
    std::vector<int> worstCase = {1, 2, 3, 4, 5, 6, 7, 8, 9}; // Уже отсортирован
    std::vector<int> averageCase = {3, 7, 1, 9, 2, 8, 5, 4, 6}; // Случайный порядок
    
    QuickSortAnalyzer::analyzeSort(bestCase, "Лучший");
    QuickSortAnalyzer::analyzeSort(worstCase, "Худший"); 
    QuickSortAnalyzer::analyzeSort(averageCase, "Средний");
}

int main(){
    demonstrateQuickSortCases();
}