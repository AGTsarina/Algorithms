#include <iostream>
#include <unordered_map>
#include <vector>
#include <cmath>
#include <iomanip>
#include <algorithm>

using namespace std;

class ArithmeticCoder {
private:
    unordered_map<char, pair<double, double>> probability_ranges;
    int precision_bits;
    
    void calculateRanges(const string& text) {
        // Подсчет частот
        unordered_map<char, int> frequencies;
        for (char c : text) {
            frequencies[c]++;
        }
        
        // Вычисление вероятностей и интервалов
        double total = text.length();
        double low = 0.0;

        vector<pair<char, int>> fr(frequencies.begin(), frequencies.end());
        sort(fr.begin(), fr.end(), [](const auto &a, const auto& b){
            if (a.second > b.second) return true;
            if (a.second < b.second) return false;
            return a.first < b.first;}); 
        
        for (const auto& pair : fr) {
            double probability = pair.second / total;
            probability_ranges[pair.first] = {low, low + probability};
            low += probability;
        }
    }
    
    double binaryToDecimal(const string& binary) {
        double result = 0.0;
        double factor = 0.5;
        
        for (char bit : binary) {
            if (bit == '1') {
                result += factor;
            }
            factor *= 0.5;
        }
        
        return result;
    }
    
    string decimalToBinary(double number, int bits) {
        string binary;
        number -= floor(number); // Берем дробную часть
        
        for (int i = 0; i < bits; i++) {
            number *= 2;
            if (number >= 1.0) {
                binary += '1';
                number -= 1.0;
            } else {
                binary += '0';
            }
        }
        
        return binary;
    }
    
public:
    ArithmeticCoder(int precision = 32) : precision_bits(precision) {}
    
    string encode(const string& text) {
        calculateRanges(text);
        
        double low = 0.0;
        double high = 1.0;
        double range = 1.0;
        
        for (char c : text) {
            if (probability_ranges.find(c) == probability_ranges.end()) {
                throw runtime_error("Symbol not found: " + string(1, c));
            }
            
            double char_low = probability_ranges[c].first;
            double char_high = probability_ranges[c].second;
            
            double new_low = low + range * char_low;
            double new_high = low + range * char_high;
            
            low = new_low;
            high = new_high;
            range = high - low;
            
            cout << "After '" << c << "': [" << low << ", " << high << "]" << endl;
        }
        
        // Выбираем число внутри финального интервала
        double result = (low + high) / 2.0;
        
        // Конвертируем в двоичное представление
        return decimalToBinary(result, precision_bits);
    }
    
    string decode(const string& encoded, int original_length) {
        if (probability_ranges.empty()) {
            throw runtime_error("Probability ranges not initialized");
        }
        
        double value = binaryToDecimal(encoded);
        string decoded;
        
        double low = 0.0;
        double high = 1.0;
        double range = 1.0;
        
        for (int i = 0; i < original_length; i++) {
            // Находим символ, соответствующий текущему значению
            double current = (value - low) / range;
            
            char found_char = '\0';
            for (const auto& pair : probability_ranges) {
                double char_low = pair.second.first;
                double char_high = pair.second.second;
                
                if (current >= char_low && current < char_high) {
                    found_char = pair.first;
                    
                    // Обновляем интервал
                    double new_low = low + range * char_low;
                    double new_high = low + range * char_high;
                    
                    low = new_low;
                    high = new_high;
                    range = high - low;
                    
                    break;
                }
            }
            
            if (found_char == '\0') {
                throw runtime_error("Decoding error");
            }
            
            decoded += found_char;
        }
        
        return decoded;
    }
    
    void printProbabilityRanges() {
        cout << "Probability Ranges:" << endl;
        for (const auto& pair : probability_ranges) {
            cout << "'" << pair.first << "': [" << pair.second.first 
                 << ", " << pair.second.second << "]" << endl;
        }
    }
};

// Пример использования
int main() {
    string text = "abracadabrabrabra";
    
    ArithmeticCoder coder;
    
    cout << "Original text: " << text << endl;
    cout << "Encoding process:" << endl;
    
    string encoded = coder.encode(text);
    cout << "Encoded binary: " << encoded << endl;
    
    coder.printProbabilityRanges();
    
    string decoded = coder.decode(encoded, text.length());
    cout << "Decoded text: " << decoded << endl;
    
    return 0;
}