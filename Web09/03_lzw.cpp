#include <iostream>
#include <string>
#include <vector>
#include <unordered_map>
using namespace std;

vector<int> lzw_compress(const string& input) {
    unordered_map<string, int> dictionary;
    vector<int> compressed;
    
    // Инициализация словаря базовыми символами
    for (int i = 0; i < 256; i++) {
        dictionary[string(1, char(i))] = i;
    }
    
    string current;
    int dict_size = 256;
    
    for (char c : input) {
        string next = current + c;
        if (dictionary.find(next) != dictionary.end()) {
            current = next;
        } else {
            // Выводим код для current
            compressed.push_back(dictionary[current]);
            // Добавляем новую комбинацию в словарь
            dictionary[next] = dict_size++;
            current = string(1, c);
        }
    }
    
    if (!current.empty()) {
        compressed.push_back(dictionary[current]);
    }
    
    return compressed;
}

string lzw_decompress(const vector<int>& compressed) {
    // Инициализация словаря
    unordered_map<int, string> dictionary;
    for (int i = 0; i < 256; i++) {
        dictionary[i] = string(1, char(i));
    }
    
    int dict_size = 256;
    string decompressed;
    
    if (compressed.empty()) return decompressed;
    
    // Обрабатываем первый код
    int old_code = compressed[0];
    string current = dictionary[old_code];
    decompressed = current;
    
    // Обрабатываем остальные коды
    for (size_t i = 1; i < compressed.size(); i++) {
        int new_code = compressed[i];
        string entry;
        
        // Проверяем наличие кода в словаре
        if (dictionary.find(new_code) != dictionary.end()) {
            entry = dictionary[new_code];
        } else if (new_code == dict_size) {
            // Специальный случай для кода, который должен быть добавлен
            entry = current + current[0];
        } else {
            throw invalid_argument("Invalid compressed code: " + to_string(new_code));
        }
        
        decompressed += entry;
        
        // Добавляем новую комбинацию в словарь
        dictionary[dict_size++] = current + entry[0];        
        
        current = entry;
    }
    
    return decompressed;
}



int main() {
    string original = "ABRACADABRABRABRA";
    int window_size = 10;
    
    vector<int> compressed = lzw_compress(original);
    string decompressed = lzw_decompress(compressed);
    
    cout << "Original: " << original << endl;
    cout << "Decompressed: " << decompressed << endl;
    cout << "Compression ratio: " << (float)(compressed.size()) / original.size() << endl;
    
    return 0;
}