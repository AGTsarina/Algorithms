#include <iostream>
#include <list>
#include <deque>
#include <chrono>
#include <functional>

using namespace std;

const int ELEMENTS_COUNT = 1000000;

void get_execution_time(function<void(void)> f){
    auto start = chrono::steady_clock::now(); 
    f();       
    auto end = chrono::steady_clock::now();

    chrono::duration<double> elapsed = end - start;
    cout << "Время выполнения: " << elapsed.count() << " секунд\n";
}

template<typename Container>
void assess_container(Container & container){
    get_execution_time([&container](){        
        for (int i = 0; i < ELEMENTS_COUNT; ++i) {
            container.push_back(i);
        }});
}

int main(){
    
    vector<int> vec;
    vector<int> vec_reserve;
    vec_reserve.reserve(ELEMENTS_COUNT); 
    list<int> lst;
    deque<int> deque;   
    
    assess_container(vec);  // Время выполнения: 0.0128581 секунд
  
    assess_container(vec_reserve); // Время выполнения: 0.00908317 секунд

    assess_container(lst);  // Время выполнения: 0.0970343 секунд

    assess_container(deque);  // Время выполнения: 0.00989199 секунд
}

