#include <string>
#include <iostream>
#include <algorithm>
using namespace std;

int main() {
    string s1 = "Hello", s2 = "World";
    // Оператор +
    string s = s1 + " " + s2 + "!"; // "Hello World!"
    
    // Метод append()
    s.append(" ").append("Welcome").append("!"); // Hello World! Welcome!

     // Поиск подстроки
    size_t pos = s.find("World"); // Возвращает индекс первого вхождения (6)
    // Поиск любого символа из набора
    size_t vowel_pos = s.find_first_of("aeiou"); // Первая гласная буква (1)
    string word = s.substr(6, 5); // World

    s.insert(5, " dear"); // Hello dear World! Welcome!  
    s.replace(6, 4, "wonderful"); // Hello wonderful World! Welcome!
    s.erase(5, 10); // Hello World! Welcome!

    // Приведение к нижнему регистру
    transform(s.begin(), s.end(), s.begin(),
        [](unsigned char c){ return tolower(c); });

    // Удаление всех знаков препинания
    s.erase(remove_if(s.begin(), s.end(),
        [](char c){ return ispunct(c); }), s.end());

    // Поиск с использованием алгоритма
    auto it = search(s.begin(), s.end(),
        word.begin(), word.end());
    
    cout << s << " " <<pos<<" " << vowel_pos << " " << word << endl;
   
}