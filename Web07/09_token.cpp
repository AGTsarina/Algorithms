#include <iostream>
#include <string>
#include <vector>
#include <algorithm>
#include <sstream>
#include <unordered_map>
#include <unordered_set>

using namespace std;

// Функция токенизации текста с учетом стоп-слов и нормализации
vector<pair<string, int>> tokenizeText(
    const string& text, 
    const unordered_set<string>& stopWords = {}) 
{
    string processedText = text;
    
    // 1. Нормализация: приведение к нижнему регистру
    transform(processedText.begin(), processedText.end(), processedText.begin(),
                   [](unsigned char c) { return tolower(c); });

    // 2. Удаление стоп-символов (пунктуация, цифры и специальные символы)
    auto isStopChar = [](unsigned char c) {
        return ispunct(c) || isdigit(c) || iscntrl(c);
    };
    processedText.erase(remove_if(processedText.begin(), processedText.end(), isStopChar),
                        processedText.end());

    // 3. Сегментация на слова с помощью stringstream
    vector<string> tokens;
    istringstream iss(processedText);
    string token;
    while (iss >> token) {
        // 4. Удаление стоп-слов
        if (stopWords.find(token) == stopWords.end()) {
            tokens.push_back(token);
        }
    }

    // 5. Нормализация токенов (дополнительная обработка при необходимости)
    // Здесь можно добавить стемминг ( нахождения основы слова) или лемматизацию (приведение слова к его базовой, начальной форме)

    // 6. Индексация токенов
    unordered_map<string, int> tokenIndexMap;
    vector<pair<string, int>> indexedTokens;
    int currentIndex = 0;

    for (const auto& t : tokens) {
        if (tokenIndexMap.find(t) == tokenIndexMap.end()) {
            tokenIndexMap[t] = currentIndex++;
        }
        indexedTokens.emplace_back(t, tokenIndexMap[t]);
    }

    return indexedTokens;
}

int main() {
    // Пример использования
    string text = "Hello, World! This is a test. 123\n"
                       "Another line with words: cat, dog, bird!";

    // Подготовка стоп-слов
    unordered_set<string> stopWords = {"a", "an", "the", "is"};

    // Токенизация текста
    auto result = tokenizeText(text, stopWords);

    // Вывод результатов
    cout << "Original text:\n" << text << "\n\nTokens with indices:\n";
    for (const auto& [token, index] : result) {
        cout << "Token: '" << token << "' -> Index: " << index << endl;
    }

    return 0;
}