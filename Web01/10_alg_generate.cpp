#include <iostream>
#include <fstream>
#include <iomanip>
#include <vector>
#include <set>
#include <algorithm>
#include <random>
using namespace std;

int main() {    
    vector<int> v1(10), v2(10); 
    random_device rd;     
    mt19937 engine(rd());    
    uniform_int_distribution<int> int_dist(0, 10);

    generate(v1.begin(), v1.end(), 
        [&]() {return int_dist(engine);});
    generate(v2.begin(), v2.end(), 
        [&]() {return int_dist(engine);});

    set<int> s1(v1.begin(), v1.end()), s2(v2.begin(), v2.end());     
    
}
