#include <iostream>
#include <vector>
#include <stack>

using namespace std;

size_t get_idx(const char &node){
    return node - 'A';
}

void dfs_iterative(int start, const vector<vector<char>>& graph) {
    int n = graph.size();
    vector<bool> visited(n, false);
    stack<char> s;
    
    s.push(start);
    visited[get_idx(start)] = true;
    
    while (!s.empty()) {
        size_t node = get_idx(s.top());
        cout << "Посещена вершина: " << s.top() << endl;        
        s.pop();        
        
        // Обрабатываем соседей в обратном порядке для сохранения порядка обхода
        for (auto it = graph[node].rbegin(); it != graph[node].rend(); ++it) {
            auto neighbour = get_idx(*it);
            if (!visited[neighbour]) {
                visited[neighbour] = true;
                s.push(*it);
            }
        }
    }
}

// Пример использования
int main() {
    vector<vector<char>> graph = {
        {'B', 'C', 'D'}, {'A', 'E', 'F'}, {'A'}, {'A', 'G'}, {'B'}, {'B'},{'D'} 
    };
    
    cout << "Итеративный DFS обход starting from vertex 0:" << endl;
    dfs_iterative('A', graph);
    
    return 0;
}