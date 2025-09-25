#include <iostream>
#include <string>
#include <vector>
using namespace std;

struct Token {
    int offset; // Смещение назад
    int length; // Длина совпадения
    char nextChar; // Следующий символ
};

vector<Token> lz77_compress(const string& input, int window_size) {
    vector<Token> compressed;
    size_t i = 0;
    
    while (i < input.size()) {
        Token token = {0, 0, input[i]};
        int max_offset = min((int)i, window_size);
        
        // Поиск наилучшего совпадения в окне
        for (int offset = 1; offset <= max_offset; offset++) {
            int len = 0;
            while (i + len < input.size() && 
                   input[i + len] == input[i - offset + len % offset]) {
                len++;
            }
            if (len > token.length) {
                token.offset = offset;
                token.length = len;
                token.nextChar = input[i + len];
            }
        }
        
        compressed.push_back(token);
        i += token.length + 1; // Перемещаемся за обработанную фразу
    }
    
    return compressed;
}

string lz77_decompress(const vector<Token>& compressed) {
    string decompressed;
    
    for (const Token& token : compressed) {
        if (token.offset == 0) {
            decompressed += token.nextChar;
        } else {
            // Копируем фразу из уже распакованной части
            size_t start = decompressed.size() - token.offset;
            for (int i = 0; i < token.length; i++) {
                decompressed += decompressed[start + i % token.offset];
            }
            decompressed += token.nextChar;
        }
    }
    
    return decompressed;
}

int main() {
    string original = "ABRACADABRABRABRA";
    int window_size = 10;
    
    vector<Token> compressed = lz77_compress(original, window_size);
    string decompressed = lz77_decompress(compressed);
    
    cout << "Original: " << original << endl;
    cout << "Decompressed: " << decompressed << endl;
    cout << "Compression ratio: " << (float)(compressed.size() * 3) / original.size() << endl;
    
    return 0;
}