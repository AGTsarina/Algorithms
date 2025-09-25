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
    return '�' <= c && c <= '�';
}

// ������� ��� �������� ������ �� �����
string load_text(const string& filename) {
    ifstream file(filename);
    if (!file.is_open()) {
        throw runtime_error("Cannot open file: " + filename);
    }    
    // ������ ���� ���� � ������
    string content((istreambuf_iterator<char>(file)),
                        istreambuf_iterator<char>());
    return content;
}

// ������� ��� ������������� ������
string preprocess_text(const string& text) {
    // 0. �������� �������� �����
    string result = text;    
    // 1. �������� � ������� ��������
    transform(result.begin(), result.end(), result.begin(),
                  [](unsigned char c) { return tolower(c); });
    
    // 2. ������� ��� ��-��������� ������� (����� �������� � �����)
    result.erase(remove_if(result.begin(), result.end(),
        [](char c) { return !isalpha(c) && !isrusalpha(c) && !isspace(c) && c != '.'; }),
        result.end());
    
    // 3. ����������� ������� (�������� ������������� ������� �� ���������)
    auto double_spaces = unique(result.begin(), result.end(),
        [](char a, char b) { return isspace(a) && isspace(b); });
    result.erase(double_spaces, result.end());

    // 4. ����������� ����� ���������� (��������� ������ ����)
    auto double_punct = unique(result.begin(), result.end(),
        [](char a, char b) { return ispunct(a) && ispunct(b); });
    result.erase(double_punct, result.end());
    
    return result;
}

// ������� ��� ���������� ������ �� �����
vector<string> extract_words(const string& text) {
    vector<string> words;
    istringstream iss(text);
    copy(istream_iterator<string>(iss),
              istream_iterator<string>(),
              back_inserter(words));
    return words;
}

// ������� ��� ���������� ������ �� �����������
vector<string> extract_sentences(const string& text) {
    vector<string> sentences;
    istringstream iss(text);
    string sentence;
    
    while (getline(iss, sentence, '.')) {
        // ������� ��������� � �������� �������
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

// ������� ��� �������� ������� ����
map<string, int> count_word_frequency(const vector<string>& words) {
    map<string, int> frequency;
    // ��������� ������ ����� ����� ������ 2
    for (const auto& word : words) {
        if (word.length() > 2)
            frequency[word]++;
    }
    return frequency;
}

// ������� ��� ���������� ����� ������ ����
vector<pair<string, int>> get_most_frequent_words(
    const map<string, int>& frequency, int count) {
    
    vector<pair<string, int>> sorted_words(frequency.begin(), frequency.end());
    
    // ��������� �� ������� (�� ��������)
    sort(sorted_words.begin(), sorted_words.end(),
        [](const auto& a, const auto& b) { return a.second > b.second; });
    
    // ���������� ���-N ����
    if (sorted_words.size() > count) {
        sorted_words.resize(count);
    }
    
    return sorted_words;
}

// ������� ��� ���������� ���������� ���� � ������
set<string> find_unique_words(const vector<string>& words) {
    set<string> unique_words(words.begin(), words.end());
    return unique_words;
}

// �������� ������� �������
void analyze_author_style(const string& filename, const string& author_name) {
    out << "������ ����� ������: " << author_name << "\n\n";
    
    try {
        // 1. �������� � ������������� ������
        string text = load_text(filename);
        string processed_text = preprocess_text(text);
        
        // 2. ���������� ���� � �����������
        vector<string> words = extract_words(processed_text);
        vector<string> sentences = extract_sentences(processed_text);
        
        // 3. ������� ����������
        map<string, int> word_frequency = count_word_frequency(words);
        auto top_words = get_most_frequent_words(word_frequency, 10);
        auto unique_words = find_unique_words(words);
        
        // 4. ������ ������� ����� �����������
        double avg_sentence_length = 0;
        for (const auto& sentence : sentences) {
            vector<string> sentence_words = extract_words(sentence);
            avg_sentence_length += sentence_words.size();
        }
        avg_sentence_length /= sentences.size();
        
        // 5. ����� �����������
        out << "����� ���������� ����: " << words.size() << "\n";
        out << "���������� ���������� ����: " << unique_words.size() << "\n";
        out << "���������� �����������: " << sentences.size() << "\n";
        out << "������� ����� �����������: " << avg_sentence_length << " ����\n";
        
        out << "\n10 ����� ������ ����:\n";
        for (const auto& [word, count] : top_words) {
            out << word << ": " << count << "\n";
        }
        
        out << "\n" << string(50, '-') << "\n\n";
        
    } catch (const exception& e) {
        cerr << "������: " << e.what() << "\n";
    }
}

// ������� ��� ��������� ���� �������
void compare_authors(const string& file1, const string& name1,
                     const string& file2, const string& name2) {
    
    // ������ ������� ������
    analyze_author_style(file1, name1);
    
    // ������ ������� ������
    analyze_author_style(file2, name2);
    
    // �������������: ����� �������� ��������� ���������� ����
    // � ���������� ������������ ��������/�������� ������
}

int main() {
    // ���������� ����� ���� �������
    compare_authors("dostoevsky.txt", "�.�. �����������",
                    "chekhov.txt", "�.�. �����");
    
    return 0;
}