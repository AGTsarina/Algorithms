#include <iostream>
#include <queue>
#include <unordered_map>
#include <vector>
#include <bitset>
#include <memory>

using namespace std;

// Узел дерева Хаффмана
struct HuffmanNode {
    char symbol;
    int frequency;
    shared_ptr<HuffmanNode> left;
    shared_ptr<HuffmanNode> right;
    
    HuffmanNode(char s, int f) : symbol(s), frequency(f), left(nullptr), right(nullptr) {}
    HuffmanNode(int f, shared_ptr<HuffmanNode> l, shared_ptr<HuffmanNode> r) 
        : symbol('\0'), frequency(f), left(l), right(r) {}
};

// Компаратор для приоритетной очереди
struct CompareNode {
    bool operator()(const shared_ptr<HuffmanNode>& a, const shared_ptr<HuffmanNode>& b) {
        return a->frequency > b->frequency;
    }
};

class HuffmanCoder {
private:
    shared_ptr<HuffmanNode> root;
    unordered_map<char, string> codes;
    unordered_map<string, char> reverse_codes;

    void buildTree(const unordered_map<char, int>& frequencies) {
        priority_queue<shared_ptr<HuffmanNode>, 
                      vector<shared_ptr<HuffmanNode>>, 
                      CompareNode> pq;
        
        // Создаем листья для каждого символа
        for (const auto& pair : frequencies) {
            pq.push(make_shared<HuffmanNode>(pair.first, pair.second));
        }
        
        // Строим дерево
        while (pq.size() > 1) {
            auto left = pq.top(); pq.pop();
            auto right = pq.top(); pq.pop();
            
            auto parent = make_shared<HuffmanNode>(
                left->frequency + right->frequency, left, right);
            pq.push(parent);
        }
        
        root = pq.top();
    }
    
    void generateCodes(shared_ptr<HuffmanNode> node, string code) {
        if (!node) return;
        // если нашли лист дерева
        if (!node->left && !node->right) {
            codes[node->symbol] = code;
            reverse_codes[code] = node->symbol;
            return;
        }
        
        generateCodes(node->left, code + "0");
        generateCodes(node->right, code + "1");
    }
    
public:
    void train(const string& text) {
        // Подсчет частот
        unordered_map<char, int> frequencies;
        for (char c : text) {
            frequencies[c]++;
        }
        
        buildTree(frequencies);
        generateCodes(root, "");
    }
    
    string encode(const string& text) {
        string encoded;
        for (char c : text) {
            if (codes.find(c) == codes.end()) {
                throw runtime_error("Symbol not in training data: " + string(1, c));
            }
            encoded += codes[c];
        }
        return encoded;
    }
    
    string decode(const string& encoded) {
        string decoded;
        string current_code;
        
        for (char bit : encoded) {
            current_code += bit;
            if (reverse_codes.find(current_code) != reverse_codes.end()) {
                decoded += reverse_codes[current_code];
                current_code = "";
            }
        }
        
        if (!current_code.empty()) {
            throw runtime_error("Invalid encoded string");
        }
        
        return decoded;
    }
    
    void printCodes() {
        cout << "Huffman Codes:" << endl;
        for (const auto& pair : codes) {
            cout << "'" << pair.first << "': " << pair.second << endl;
        }
    }
    
    double calculateCompressionRatio(const string& original) {
        string encoded = encode(original);
        double original_bits = original.length() * 8.0;
        double encoded_bits = encoded.length();
        return encoded_bits / original_bits;
    }
};

// Пример использования
int main() {
    string text = "abracadabrabrabra";
    
    HuffmanCoder coder;
    coder.train(text);
    
    cout << "Original text: " << text << endl;
    
    string encoded = coder.encode(text);
    cout << "Encoded: " << encoded << endl;
    
    string decoded = coder.decode(encoded);
    cout << "Decoded: " << decoded << endl;
    
    coder.printCodes();
    
    double ratio = coder.calculateCompressionRatio(text);
    cout << "Compression ratio: " << ratio << endl;
    
    return 0;
}