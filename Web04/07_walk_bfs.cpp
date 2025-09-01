#include <iostream>
#include <vector>
#include <queue>
#include <unordered_set>

using namespace std;

size_t get_idx(const char &node){
    return node - 'A';
}

void bfs(char start, const vector<vector<char>>& graph) {
    int n = graph.size();
    vector<bool> visited(n, false);
    queue<char> q;
    
    q.push(start);
    visited[get_idx(start)] = true;
    
    while (!q.empty()) {
        int node = get_idx(q.front());
        cout << "Посещена вершина: " << q.front() << endl;
        q.pop();        
        
        for (const char &neighbor : graph[node]) {
            int neighbour_index = get_idx(neighbor);
            if (!visited[neighbour_index]) {
                visited[neighbour_index] = true;
                q.push(neighbor);
            }
        }
    }
}

int main() {
    // Граф в виде списка смежности
    vector<vector<char>> graph = {
        {'B', 'C', 'D'}, {'A', 'E', 'F'}, {'A'}, {'A', 'G'}, {'B'}, {'B'},{'D'}
    };
    
    cout << "BFS:" << endl;
    bfs('A', graph);
    
    return 0;
}