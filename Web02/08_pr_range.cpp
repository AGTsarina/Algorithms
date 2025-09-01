#include <map>
#include <string>
using namespace std;

using Dictionary = multimap<int, string>;
using Iterator = Dictionary::iterator;
using Range = pair<Iterator, Iterator>;


Iterator lower_bound(Dictionary& mmap, const int& key) {
    auto it = mmap.begin();
    auto end = mmap.end();    
    // Проходим по дереву, пока не найдем первый элемент >= key
    while (it != end) {
        if (it->first >= key) return it;
        ++it;
    }
    return end;
}

Iterator upper_bound(Dictionary& mmap, const int& key) {
    auto it = mmap.begin();
    auto end = mmap.end();    
    // Проходим по дереву, пока не найдем первый элемент > key
    while (it != end) {
        if (key < it->first) return it;
        ++it;
    }
    return end;
}

Range find_range(Dictionary& mmap, const int& key_min, const int& key_max) {
    // Находим первый элемент с ключом >= минимального (нижняя граница)
    auto lower = mmap.lower_bound(key_min);
    
    // Находим первый элемент с ключом > максимального (верхняя граница)
    auto upper = mmap.upper_bound(key_max);
    
    return make_pair(lower, upper);
}



