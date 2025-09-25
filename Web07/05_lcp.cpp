#include <iostream>
#include <vector>
#include <string>
#include <algorithm>

std::vector<int> buildLCP(const std::string& text, const std::vector<int>& suffixArray) {
    int n = text.size();
    std::vector<int> lcp(n, 0);
    std::vector<int> rank(n, 0);
    
    // Создаем массив рангов (обратный к суффиксному массиву)
    for (int i = 0; i < n; i++) {
        rank[suffixArray[i]] = i;
    }
    
    int k = 0;
    for (int i = 0; i < n; i++) {
        if (rank[i] == n - 1) {
            k = 0;
            continue;
        }
        
        // j - следующий суффикс в суффиксном массиве
        int j = suffixArray[rank[i] + 1];
        
        // Увеличиваем k, пока символы совпадают
        while (i + k < n && j + k < n && text[i + k] == text[j + k]) {
            k++;
        }
        
        lcp[rank[i]] = k;
        
        // Уменьшаем k для следующей итерации
        if (k > 0) k--;
    }
    
    return lcp;
}

// Пример построения суффиксного массива (упрощенная версия)
std::vector<int> buildSuffixArray(const std::string& text) {
    int n = text.size();
    std::vector<int> sa(n);
    std::vector<std::pair<std::string, int>> suffixes;
    
    // Генерируем все суффиксы с их индексами
    for (int i = 0; i < n; i++) {
        suffixes.push_back({text.substr(i), i});
    }
    
    // Сортируем суффиксы лексикографически
    std::sort(suffixes.begin(), suffixes.end());
    
    // Извлекаем индексы начала суффиксов
    for (int i = 0; i < n; i++) {
        sa[i] = suffixes[i].second;
    }
    
    return sa;
}

int main() {
    std::string text = "banana";
    
    // Добавляем терминальный символ
    text += '$';
    
    // Строим суффиксный массив
    std::vector<int> sa = buildSuffixArray(text);
    
    // Строим LCP-массив
    std::vector<int> lcp = buildLCP(text, sa);
    
    // Выводим результаты
    std::cout << "Суффиксный массив и LCP для строки: " << text << "\n\n";
    
    for (int i = 0; i < sa.size(); i++) {
        std::cout << "SA[" << i << "] = " << sa[i] << " \t";
        std::cout << "LCP[" << i << "] = " << lcp[i] << " \t";
        std::cout << text.substr(sa[i]) << "\n";
    }
    
    return 0;
}