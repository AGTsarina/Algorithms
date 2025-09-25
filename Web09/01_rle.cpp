#include <iostream>
#include <string>
using namespace std;

// Функция для RLE-сжатия
string rle_compress(const string& input) {
    string compressed;
    char count = 1;
    char current = input[0];

    for (size_t i = 1; i <= input.size(); i++) {
        if (i < input.size() && input[i] == current) {
            count++;
        } else {
            // Добавляем символ и количество (в виде символа)
            compressed += current;
            compressed += count + ' '; // код печатных символов
            // Сброс для следующего символа
            if (i < input.size()) {
                current = input[i];
                count = 1;
            }
        }
    }
    return compressed;
}

// Функция для RLE-восстановления данных
string rle_decompress(const string& compressed) {
    string decompressed;
    for (size_t i = 0; i < compressed.size(); i += 2) {
        char symbol = compressed[i];
        int count = compressed[i + 1] - ' '; // Восстанавливаем число повторений
        decompressed.append(count, symbol);
    }
    return decompressed;
}

int main() {
    string original = "AAAABBBCCCDDDDEEEEE";
    string compressed = rle_compress(original);
    string decompressed = rle_decompress(compressed);

    cout << "Original: " << original << endl;
    cout << "Compressed: " << compressed << endl;
    cout << "Decompressed: " << decompressed << endl;
    cout << "Compression ratio: " << (float)compressed.size() / original.size() << endl;

    return 0;
}