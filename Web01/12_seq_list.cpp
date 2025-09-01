#include <iostream>
#include <list>
#include <chrono>
#include <functional>

using namespace std;

void get_execution_time(function<void(void)> f, int n){
    auto start = chrono::steady_clock::now(); 
    for(int i=0; i<n; i++){
        f();
    }    
    auto end = chrono::steady_clock::now();

    chrono::duration<double> elapsed = end - start;
    cout << "Время выполнения: " << elapsed.count() << " секунд\n";
}
int main(){
    list<int> lst;   
    int m = 10000;
    for(int i=0; i<m; i++){
        lst.push_back(i);
    }
    int n = 1000;
    get_execution_time([lst](){
        int S = 0;
        for (auto it = lst.begin(); it != lst.end(); it=next(it)){
            S += *it;
        }}, n);
    get_execution_time([lst](){
        int S = 0;
        for (auto it = lst.begin(); it != lst.end(); ++it){
            S += *it;
        }}, n);
    get_execution_time([lst](){
        int S = 0;
        for(auto &x: lst){
            S += x;
        }}, n);    
    return 0;
}
