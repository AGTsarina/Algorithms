#include <iostream>
#include <vector>
#include <string>
#include <bitset>
#include <string>
#include <cstdint>

class SimpleFeistelCipher {
private:
    // Простая функция раунда F (можно модифицировать)
    uint8_t feistel_function(uint8_t data, uint8_t key) {
        // Простейшие операции для демонстрации
        return ((data << 3) | (data >> 5)) ^ key; // Циклический сдвиг + XOR
    }
    
    // Генерация раундовых ключей из основного ключа
    std::vector<uint8_t> generate_round_keys(uint8_t main_key, int rounds) {
        std::vector<uint8_t> round_keys;
        for (int i = 0; i < rounds; i++) {
            round_keys.push_back(main_key ^ i); // Простая генерация
        }
        return round_keys;
    }

public:
    // Шифрование 16-битного блока
    uint16_t encrypt(uint16_t block, uint8_t key, int rounds = 4) {
        auto round_keys = generate_round_keys(key, rounds);
        
        uint8_t left = (block >> 8) & 0xFF;  // Старший байт
        uint8_t right = block & 0xFF;        // Младший байт
        
        for (int i = 0; i < rounds; i++) {
            uint8_t temp = right;
            right = left ^ feistel_function(right, round_keys[i]);
            left = temp;
        }
        
        return (left << 8) | right;
    }
    
    // Расшифрование 16-битного блока
    uint16_t decrypt(uint16_t block, uint8_t key, int rounds = 4) {
        auto round_keys = generate_round_keys(key, rounds);
        
        uint8_t left = (block >> 8) & 0xFF;
        uint8_t right = block & 0xFF;
        
        // Обратный порядок ключей для расшифрования
        for (int i = rounds - 1; i >= 0; i--) {
            uint8_t temp = left;
            left = right ^ feistel_function(left, round_keys[i]);
            right = temp;
        }
        
        return (left << 8) | right;
    }
};

int main(){
    SimpleFeistelCipher sfc;
    uint8_t key = 0x1f;
    std::string open_text = "Text to cifr!";
    
    const uint16_t * blocks = reinterpret_cast<const uint16_t*>(open_text.c_str());
    int len = open_text.length() / 2 + 1;
    uint16_t * encrypted_blocks = new uint16_t[len];
    uint16_t * decrypted_blocks = new uint16_t[len];
    
    for(int i=0; i<len; i++){
        encrypted_blocks[i] = sfc.encrypt(blocks[i], key, 10);
    }

    for(int i=0; i<len; i++){
        decrypted_blocks[i] = sfc.decrypt(encrypted_blocks[i], key, 10);
    }
    std::string original(reinterpret_cast<const char*>(blocks));
    std::string encrypted(reinterpret_cast<char*>(encrypted_blocks));
    std::string result(reinterpret_cast<char*>(decrypted_blocks));

    std::cout << "Original: " << original<< std::endl;
    std::cout << "Encrypted: " << encrypted<< std::endl;
    std::cout << "Decrypted: " << result<< std::endl;


}