#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include <algorithm>
#include <map>
#include <set>
#include <cctype>
#include <sstream>
#include <iterator>

using namespace std;

ofstream out("result.txt");

bool isrusalpha(char c){
    return 'а' <= c && c <= 'я';
}

// Функция для загрузки текста из файла
string load_text(const string& filename) {
    ifstream file(filename);
    if (!file.is_open()) {
        throw runtime_error("Cannot open file: " + filename);
    }    
    // Читаем весь файл в строку
    string content((istreambuf_iterator<char>(file)),
                        istreambuf_iterator<char>());
    return content;
}

// Функция для предобработки текста
string preprocess_text(const string& text) {
    // 0. Копируем исходный текст
    string result = text;    
    // 1. Приводим к нижнему регистру
    transform(result.begin(), result.end(), result.begin(),
                  [](unsigned char c) { return tolower(c); });
    
    // 2. Удаляем все не-буквенные символы (кроме пробелов и точек)
    result.erase(remove_if(result.begin(), result.end(),
        [](char c) { return !isalpha(c) && !isrusalpha(c) && !isspace(c) && c != '.'; }),
        result.end());
    
    // 3. Нормализуем пробелы (заменяем множественные пробелы на одинарные)
    auto double_spaces = unique(result.begin(), result.end(),
        [](char a, char b) { return isspace(a) && isspace(b); });
    result.erase(double_spaces, result.end());

    // 4. Нормализуем знаки препинания (оставляем только один)
    auto double_punct = unique(result.begin(), result.end(),
        [](char a, char b) { return ispunct(a) && ispunct(b); });
    result.erase(double_punct, result.end());
    
    return result;
}

// Функция для разделения текста на слова
vector<string> extract_words(const string& text) {
    vector<string> words;
    istringstream iss(text);
    copy(istream_iterator<string>(iss),
              istream_iterator<string>(),
              back_inserter(words));
    return words;
}

// Функция для разделения текста на предложения
vector<string> extract_sentences(const string& text) {
    vector<string> sentences;
    istringstream iss(text);
    string sentence;
    
    while (getline(iss, sentence, '.')) {
        // Удаляем начальные и конечные пробелы
        sentence.erase(sentence.begin(), 
                      find_if(sentence.begin(), sentence.end(),
                      [](int ch) { return !isspace(ch); }));
        sentence.erase(find_if(sentence.rbegin(), sentence.rend(),
                      [](int ch) { return !isspace(ch); }).base(),
                      sentence.end());
        
        if (!sentence.empty()) {
            sentences.push_back(sentence);
        }
    }
    
    return sentences;
}

// Функция для подсчета частоты слов
map<string, int> count_word_frequency(const vector<string>& words) {
    map<string, int> frequency;
    // учитываем только слова длины больше 2
    for (const auto& word : words) {
        if (word.length() > 2)
            frequency[word]++;
    }
    return frequency;
}

// Функция для нахождения самых частых слов
vector<pair<string, int>> get_most_frequent_words(
    const map<string, int>& frequency, int count) {
    
    vector<pair<string, int>> sorted_words(frequency.begin(), frequency.end());
    
    // Сортируем по частоте (по убыванию)
    sort(sorted_words.begin(), sorted_words.end(),
        [](const auto& a, const auto& b) { return a.second > b.second; });
    
    // Возвращаем топ-N слов
    if (sorted_words.size() > count) {
        sorted_words.resize(count);
    }
    
    return sorted_words;
}

// Функция для нахождения уникальных слов в тексте
set<string> find_unique_words(const vector<string>& words) {
    set<string> unique_words(words.begin(), words.end());
    return unique_words;
}

// Основная функция анализа
void analyze_author_style(const string& filename, const string& author_name) {
    out << "Анализ стиля автора: " << author_name << "\n\n";
    
    try {
        // 1. Загрузка и предобработка текста
        string text = load_text(filename);
        string processed_text = preprocess_text(text);
        
        // 2. Извлечение слов и предложений
        vector<string> words = extract_words(processed_text);
        vector<string> sentences = extract_sentences(processed_text);
        
        // 3. Подсчет статистики
        map<string, int> word_frequency = count_word_frequency(words);
        auto top_words = get_most_frequent_words(word_frequency, 10);
        auto unique_words = find_unique_words(words);
        
        // 4. Расчет средней длины предложения
        double avg_sentence_length = 0;
        for (const auto& sentence : sentences) {
            vector<string> sentence_words = extract_words(sentence);
            avg_sentence_length += sentence_words.size();
        }
        avg_sentence_length /= sentences.size();
        
        // 5. Вывод результатов
        out << "Общее количество слов: " << words.size() << "\n";
        out << "Количество уникальных слов: " << unique_words.size() << "\n";
        out << "Количество предложений: " << sentences.size() << "\n";
        out << "Средняя длина предложения: " << avg_sentence_length << " слов\n";
        
        out << "\n10 самых частых слов:\n";
        for (const auto& [word, count] : top_words) {
            out << word << ": " << count << "\n";
        }
        
        out << "\n" << string(50, '-') << "\n\n";
        
    } catch (const exception& e) {
        cerr << "Ошибка: " << e.what() << "\n";
    }
}

// Функция для сравнения двух авторов
void compare_authors(const string& file1, const string& name1,
                     const string& file2, const string& name2) {
    
    // Анализ первого автора
    analyze_author_style(file1, name1);
    
    // Анализ второго автора
    analyze_author_style(file2, name2);
    
    // Дополнительно: можно добавить сравнение уникальных слов
    // и вычисление коэффициента сходства/различия стилей
}

int main() {
    // Сравниваем стили двух авторов
    compare_authors("dostoevsky.txt", "Ф.М. Достоевский",
                    "chekhov.txt", "А.П. Чехов");
    
    return 0;
}