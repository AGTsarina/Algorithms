#include <iostream>
#include <cmath>

using namespace std;

int hash_int(int key){
    return key;
}
const int table_size = 1000;
int hash_mod(int key){
    return key % table_size;
}

size_t hash_mult(int k) {
    const double A = (sqrt(5.0) - 1.0) / 2.0; 
    cout <<k * A << " " << fmod(k * A, 1.0) << endl;
    return table_size * fmod(k * A, 1.0); // Возвращает дробную часть от k*A
}

size_t hash_polynom(const string& s) {
    const int p = 31; // Малое простое число для ASCII-строк
    size_t hash_value = 0;
    size_t p_pow = 1;
    for (char c : s) {
        hash_value = (hash_value + (c - 'a' + 1) * p_pow);
        p_pow = p_pow * p;
    }
    return hash_value;
}

uint32_t hash_fnv1a(const string& s) {
    const uint32_t prime = 0x01000193; // 16777619
    uint32_t hash = 0x811C9DC5;        // Начальное значение (offset basis)

    for (char c : s) {
        hash ^= static_cast<uint8_t>(c);
        hash *= prime;
    }
    return hash;
}

int main(){
    int k = 125;
    cout << hash_mult(k)<< endl;
}
