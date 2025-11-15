#include <vector>
#include <iostream>
using namespace std;

pair<int*, int*> partition(int* left, int* right){
  auto length = right - left;
  int middle = *(left + length / 2);
  int* forward = left, *backward = right;
  do{
    while(*forward < middle) forward++;
    while(*backward > middle) backward--;
    if (forward <= backward){
      swap(*forward, *backward);
      forward++; backward--;
    }
  }while(forward <= backward);
  return {forward, backward};
}

void quick_sort(int* left, int* right){
  // РАЗДЕЛЯЙ: Разбиение массива относительно опорного элемента
  auto [i, j] = partition(left, right);
  // ВЛАСТВУЙ: Рекурсивная сортировка подмассивов
  if (left < j){    
    quick_sort(left, j);    // Левая часть
  }
  if (i < right){    
    quick_sort(i, right);   // Правая часть    
  }
  // ОБЪЕДИНЯЙ: Не требуется - массив сортируется на месте
}

int main(){
    int *a = new int[10]{1,4,7,3,8,2,4,9,5,6};
    quick_sort(a, a + 10);
    for(int *p = a; p < a + 10; p++){
        cout << *p << " ";
    }
}

