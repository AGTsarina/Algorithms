#include <iostream>
#include <map>
#include <vector>
#include <string>
#include <algorithm>

using namespace std;

// Класс узла суффиксного дерева
class SuffixTreeNode {
public:
    // Дочерние узлы: ключ - символ, значение - указатель на узел
    map<char, SuffixTreeNode*> children;
    
    // Начальный и конечный индексы подстроки в исходном тексте, которую представляет это ребро
    int start;
    int end;
    
    // Индексы начала всех суффиксов, которые проходят через этот узел
    vector<int> indices;

    // Конструктор: инициализирует начальный и конечный индексы
    SuffixTreeNode(int start = -1, int end = -1) : start(start), end(end) {}
    
    // Метод для вычисления длины подстроки на ребре
    int length() const {
        return end - start + 1;
    }
};

// Основной класс суффиксного дерева
class SuffixTree {
private:
    SuffixTreeNode* root;  // Корневой узел дерева
    string text;           // Исходный текст с добавленным терминальным символом '$'

    // Рекурсивная функция для добавления суффикса в дерево
    void addSuffix(SuffixTreeNode* node, int suffixIndex, int charIndex) {
        // Базовый случай: дошли до конца текста
        if (charIndex >= text.size()) return;

        // Текущий символ для обработки
        char currentChar = text[charIndex];
        
        // Проверяем, есть ли у текущего узла ребенок с таким символом
        if (node->children.find(currentChar) == node->children.end()) {
            // Создаем новый узел для этого символа
            // Ребро будет содержать подстроку от charIndex до конца текста
            SuffixTreeNode* newNode = new SuffixTreeNode(charIndex, text.size() - 1);
            
            // Добавляем индекс начала суффикса в новый узел
            newNode->indices.push_back(suffixIndex);
            
            // Связываем новый узел с текущим
            node->children[currentChar] = newNode;
        } else {
            // Если ребенок с таким символом уже существует
            SuffixTreeNode* child = node->children[currentChar];
            
            // Добавляем индекс суффикса в существующий узел
            child->indices.push_back(suffixIndex);
            
            // Рекурсивно продолжаем добавлять оставшуюся часть суффикса
            addSuffix(child, suffixIndex, charIndex + 1);
        }
    }

public:
    // Конструктор: инициализирует дерево для заданной строки
    SuffixTree(const string& input) : text(input + "$") {
        root = new SuffixTreeNode();
        buildTree();
    }

    // Построение всего суффиксного дерева
    void buildTree() {
        // Добавляем все суффиксы в дерево (от каждого символа до конца)
        for (int i = 0; i < text.size(); i++) {
            addSuffix(root, i, i);
        }
    }

    // Рекурсивная функция для печати дерева в древовидной форме
    void printTree(SuffixTreeNode* node, string prefix = "") {
        // Если узел не указан, начинаем с корня
        if (node == nullptr) node = root;
        
        // Обходим всех детей текущего узла
        for (auto& child : node->children) {
            // Получаем метку ребра (подстроку от start до end)
            string edgeLabel = text.substr(child.second->start, 
                                          child.second->end - child.second->start + 1);
            
            // Выводим метку ребра с отступом
            cout << prefix << "└── " << edgeLabel;
            
            // Если у узла есть индексы суффиксов, выводим их
            if (!child.second->indices.empty()) {
                cout << " [";
                for (int idx : child.second->indices) {
                    cout << idx << " ";
                }
                cout << "]";
            }
            cout << "\n";
            
            // Рекурсивно печатаем поддерево с увеличенным отступом
            printTree(child.second, prefix + "    ");
        }
    }

    // Публичная функция для поиска подстроки в тексте
    vector<int> search(const string& pattern) {
        // Если паттерн пустой, возвращаем пустой результат
        if (pattern.empty()) return {};
        
        // Запускаем рекурсивный поиск, начиная с корня
        return searchPattern(root, pattern, 0, 0);
    }

private:    
    // Рекурсивная функция для поиска паттерна в дереве
    vector<int> searchPattern(SuffixTreeNode* node, const string& pattern, 
                             int pos, int patternStart) {
        vector<int> result; // Вектор для хранения найденных позиций
        
        // Базовый случай: весь паттерн успешно найден
        if (pos >= pattern.size()) {           
            return {patternStart}; // Возвращаем начальную позицию паттерна
        }
        
        // Текущий символ паттерна для поиска
        char currentChar = pattern[pos];
        
        // Обходим всех детей текущего узла
        for (auto& child : node->children) {
            // Если найден ребенок с нужным символом
            if (child.first == currentChar) {
                SuffixTreeNode* childNode = child.second;
                
                // Проверяем все суффиксы, проходящие через этот узел
                for (int index: childNode->indices){
                    // Пропускаем, если от индекса до конца ребра недостаточно символов для паттерна
                    if (childNode->end - index < pattern.length()) 
                        continue;                    
                    
                    // Извлекаем метку ребра начиная с текущего индекса
                    string edgeLabel = text.substr(index, childNode->length());
                    
                    // Определяем длину для сравнения (минимум из оставшегося паттерна и длины ребра)
                    int compareLength = min((int)pattern.size() - pos, childNode->length());
                    
                    // Извлекаем соответствующие части паттерна и метки ребра
                    string patternPart = pattern.substr(pos, compareLength);
                    string edgePart = edgeLabel.substr(0, compareLength);
                    
                    // Сравниваем части паттерна и метки ребра
                    if (patternPart == edgePart) {
                        // Если сравнение успешно и мы дошли до конца паттерна
                        if (pos + compareLength >= pattern.size()) {                            
                            // Добавляем индекс начала найденного вхождения
                            result.push_back(index);
                        }                         
                    }
                }
            }
        }        
        
        // Сортируем результат для удобства чтения
        sort(result.begin(), result.end());               
        return result;
    }
};

int main() {
    // Тестирование на примере слова "suffics"
    string word = "suffics";
    SuffixTree tree(word);

    // Выводим структуру суффиксного дерева
    cout << "Суффиксное дерево для слова: " << word << "\n";
    tree.printTree(nullptr);
    cout << "\n";
    
    // Тестируем поиск различных подстрок
    vector<string> patterns = {"s", "x", "fi", "cs", "suff", "ics", "ff", "ic"};
    
    for (const auto& pattern : patterns) {
        // Ищем все вхождения паттерна
        auto positions = tree.search(pattern);
        
        // Выводим результаты поиска
        cout << "Подстрока \"" << pattern << "\" найдена в позициях: ";
        for (int pos : positions) {
            cout << pos << " ";
        }
        cout << "\n";
    }

    return 0;
}