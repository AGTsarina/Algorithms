#include <vector>
#include <string>
#include <iostream>
#include <queue>
#include <unordered_map>

using namespace std;

class AhoCorasick {
private:
    struct TrieNode {
        unordered_map<char, TrieNode*> children;
        TrieNode* fail = nullptr;
        vector<int> output; // индексы образцов, заканчивающихся здесь
        bool is_end = false;
    };

    TrieNode* root;
    vector<string> patterns;

public:
    AhoCorasick(const vector<string>& patterns) : patterns(patterns) {
        root = new TrieNode();
        buildTrie();
        buildFailureLinks();
    }

    void buildTrie() {
        for (int i = 0; i < patterns.size(); i++) {
            TrieNode* current = root;
            for (char c : patterns[i]) {
                if (current->children.find(c) == current->children.end()) {
                    current->children[c] = new TrieNode();
                }
                current = current->children[c];
            }
            current->is_end = true;
            current->output.push_back(i);
        }
    }

    void buildFailureLinks() {
        queue<TrieNode*> q;
        
        // Инициализация для корня
        for (auto& pair : root->children) {
            pair.second->fail = root;
            q.push(pair.second);
        }

        while (!q.empty()) {
            TrieNode* current = q.front();
            q.pop();

            for (auto& pair : current->children) {
                char c = pair.first;
                TrieNode* child = pair.second;
                
                TrieNode* failNode = current->fail;
                while (failNode != nullptr && 
                       failNode->children.find(c) == failNode->children.end()) {
                    failNode = failNode->fail;
                }
                
                child->fail = (failNode == nullptr) ? root : failNode->children[c];
                
                // Добавляем выходные ссылки из состояния неудачи
                child->output.insert(child->output.end(), 
                                   child->fail->output.begin(), 
                                   child->fail->output.end());
                
                q.push(child);
            }
        }
    }

    void search(const string& text) {
        TrieNode* current = root;
        
        for (int i = 0; i < text.length(); i++) {
            char c = text[i];
            
            // Переход по ссылкам неудачи, пока не найдем подходящий переход
            while (current != root && 
                   current->children.find(c) == current->children.end()) {
                current = current->fail;
            }
            
            if (current->children.find(c) != current->children.end()) {
                current = current->children[c];
            } else {
                current = root;
            }
            
            // Проверяем все образцы, заканчивающиеся в текущем состоянии
            for (int patternIndex : current->output) {
                cout << "Образец '" << patterns[patternIndex] 
                         << "' найден на позиции " << i - patterns[patternIndex].length() + 1 
                         << endl;
            }
        }
    }
};

// Использование
void AhoCorasickSearch(const string& text, const vector<string>& patterns) {
    AhoCorasick ac(patterns);
    ac.search(text);
}

int main(){
    string text = "function calculateSum(arr) {\nlet total = 0;\nfor (let i = 0; i < arr.length; i++) {\ntotal += arr[i]; \n} \nreturn total; \n}";
    vector<string> patterns({"function", "let", "for", "return", "if", "else", "while"});
    AhoCorasickSearch(text, patterns);
}