#include <iostream>
#include <fstream>
#include <iomanip>
#include <vector>
#include <algorithm>

using namespace std;
void out_function(const double &x){
    cout <<fixed<< setw(5) << setprecision(2) <<x << " "; 
}

class out_class{
    ostream& out;
    public:
    out_class(ostream& out):out(out){}
    void operator()(const double &x){
        out <<fixed<< setw(5) << setprecision(2) <<x << " ";
    }
};

int main() {
    vector<double> v = {2.1, 7.53, 6.232, 4.8, 3.9};
    // Передача функции в качестве аргумента
    for_each(v.begin(), v.end(), out_function);
    cout << "\n";
    // Вывод части последовательности
    for_each(v.begin(), v.begin() + 3, out_function);
    cout << "\n";
    // Передача функтора в качестве аргумента
    for_each(v.begin(), v.end(), out_class(cout));
    cout << "\n";
    ofstream out("output.txt");
    for_each(v.begin(), v.end(), out_class(out));
    out << "\n";
    // Лямбда-выражение
    for_each(v.begin(), v.end(), [&out](const double &x){
        out << x << "\n";});  
    out << "\n"; 
}


