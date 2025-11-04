#include <iostream>
using namespace std;

int binarySearch(int *arr, int low, int high, int target) {
    // Базовый случай: элемент не найден
    if (low > high) {
        return -1;
    }
    
    // РАЗДЕЛЯЙ: Находим средний элемент
    int mid = low + (high - low) / 2;
    
    // ВЛАСТВУЙ: Рекурсивно ищем в соответствующей половине
    if (arr[mid] == target) {
        return mid;  // Элемент найден
    } else if (arr[mid] > target) {
        // Ищем в левой половине
        return binarySearch(arr, low, mid - 1, target);
    } else {
        // Ищем в правой половине
        return binarySearch(arr, mid + 1, high, target);
    }
    
    // ОБЪЕДИНЯЙ: Не требуется - возвращаем готовый результат
}

int binarySearchIterative(int *arr, int n, int target) {
    int low = 0;
    int high = n - 1;   
    
    while (low <= high) {        
        int mid = low + (high - low) / 2;
        
        if (arr[mid] == target) {
            return mid;
        } else if (arr[mid] > target) {
            high = mid - 1;
        } else {
            low = mid + 1;
        }
    }    
    return -1;
}

int main(){
    int *a = new int[10]{1,2,3,4,4,5,6,7,8,9};
    //int index = binarySearch(a, 0, 9, 14);
    int index = binarySearchIterative(a, 10, 2);
    if (index > -1) cout << "Индекс найденного элемента: " <<index << "\n";
    else cout << "Элемент не найден\n";    
}