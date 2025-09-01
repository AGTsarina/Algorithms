#include <iostream>
#include <fstream>
#include <iomanip>
#include <vector>
#include <set>
#include <algorithm>
#include <random>
using namespace std;

int main(){
    set<int> s1({0, 2, 6, 7, 8, 10}), s2({1, 5, 6, 7, 8, 9, 10});
 
    vector<int> res_merge, res_union, res_intersect,
               res_diff, res_sym;

    merge(s1.begin(), s1.end(), s2.begin(), s2.end(),
        back_inserter(res_merge)
    );  

    set_union(s1.begin(), s1.end(), s2.begin(), s2.end(),
        back_inserter(res_union)
    );  // 1, 1, 2, 3, 5, 5, 7  
  
    set_intersection(s1.begin(), s1.end(), s2.begin(), s2.end(),
        back_inserter(res_intersect)
    );  // 1, 3    

    set_difference(s1.begin(), s1.end(), s2.begin(), s2.end(),        
        back_inserter(res_diff)
    );  // 5, 5, 7    

    set_symmetric_difference(s1.begin(), s1.end(), s2.begin(), s2.end(),
        back_inserter(res_sym)
    );  // 1, 2, 5, 5, 7

    bool res = includes(s1.begin(), s1.end(), s2.begin(), s2.end());  // False
}
