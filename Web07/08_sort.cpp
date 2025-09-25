#include <vector>
#include <string>
#include <algorithm>
#include <functional>
using namespace std;

void baseSortStrings(vector<string>& arr) {
    // Использует встроенную сортировку с компаратором
    sort(arr.begin(), arr.end(), [](const string& a, const string& b) {
        return a < b; // Лексикографическое сравнение
    });
}


void stringQuickSort(vector<string>& arr, int left, int right, int digit) {
    if (right <= left) return;

    int lt = left, gt = right;
    int pivot = (digit < arr[left].size()) ? (unsigned char)arr[left][digit] : -1;
    int i = left + 1;

    while (i <= gt) {
        int current = (digit < arr[i].size()) ? (unsigned char)arr[i][digit] : -1;
        
        if (current < pivot) {
            swap(arr[lt++], arr[i++]);
        } else if (current > pivot) {
            swap(arr[i], arr[gt--]);
        } else {
            i++;
        }
    }

    stringQuickSort(arr, left, lt - 1, digit);
    if (pivot >= 0) {
        stringQuickSort(arr, lt, gt, digit + 1);
    }
    stringQuickSort(arr, gt + 1, right, digit);
}

void stringQuickSortRec(vector<string>& arr) {
    stringQuickSort(arr, 0, arr.size() - 1, 0);
}

void radixSortMSD(vector<string>& arr, int low, int high, int digit) {
    if (high <= low + 1) return;

    const int RANGE = 256;
    vector<string> aux(high - low);
    vector<int> count(RANGE + 2, 0);

    // Подсчет количества символов в соответствующих разрядах
    for (int i = low; i < high; i++) {
        int c = (digit < arr[i].size()) ? (unsigned char)arr[i][digit] + 1 : 1;
        count[c]++;
    }

    // Нарастающая сумма
    for (int i = 1; i < RANGE + 2; i++) {
        count[i] += count[i - 1];
    }

    // Распределение индексов
    for (int i = low; i < high; i++) {
        int c = (digit < arr[i].size()) ? (unsigned char)arr[i][digit] + 1 : 1;
        aux[count[c - 1]++] = arr[i];
    }

    // Обратное копирование в исходный массив
    for (int i = low; i < high; i++) {
        arr[i] = aux[i - low];
    }

    // Рекурсивная сортировка массивов
    for (int r = 0; r < RANGE; r++) {
        radixSortMSD(arr, low + count[r], low + count[r + 1], digit + 1);
    }
}

void radixSortMSDRec(vector<string>& arr) {
    if (arr.empty()) return;
    radixSortMSD(arr, 0, arr.size(), 0);
}

void countingSort(vector<string>& arr, int digit) {
    const int RANGE = 256; // ASCII characters
    vector<string> output(arr.size());
    vector<int> count(RANGE + 1, 0);

    // Подсчет количества символов в соответствующих разрядах
    for (const auto& s : arr) {
        int charIndex = (digit < s.size()) ? (unsigned char)s[digit] : 0;
        count[charIndex + 1]++;
    }

    // Нарастающая сумма
    for (int i = 1; i <= RANGE; i++) {
        count[i] += count[i - 1];
    }

    // Результирующий массив
    for (int i = arr.size() - 1; i >= 0; i--) {
        int charIndex = (digit < arr[i].size()) ? (unsigned char)arr[i][digit] : 0;
        output[count[charIndex + 1] - 1] = arr[i];
        count[charIndex + 1]--;
    }

    arr = output;
}

void radixSortLSD(vector<string>& arr) {
    if (arr.empty()) return;
    
    // Find maximum length
    size_t max_len = 0;
    for (const auto& s : arr) {
        max_len = max(max_len, s.size());
    }

    // Sort from least significant digit to most
    for (int digit = max_len - 1; digit >= 0; digit--) {
        countingSort(arr, digit);
    }
}


int main(){
    vector<int> a = {1,2,3,4};
    const vector<function<void(vector<string>& arr)>> sortMethods({baseSortStrings, stringQuickSortRec, radixSortLSD, radixSortMSDRec});    
    vector<string> words = {"class", "classic", "classification", "classifier", "classroom",
    "object", "objective", "objection", "object-oriented",
    "function", "functional", "functionality", "function call",
    "variable", "variance", "variation", "variant",
    "algorithm", "algorithmic", "algorithmically",
    "program", "programmer", "programming", "programmatic",
    "code", "coder", "coding", "codebase", "code review"};    
    sortMethods[3](words);
    return 0;
}