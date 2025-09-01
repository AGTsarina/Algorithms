#include <iostream>
#include <vector>
#include <unordered_set>

using namespace std;

size_t get_idx(const char &node){
    return node - '0';
}

void dfs_recursive(char node, const vector<vector<char>>& graph, vector<bool>& visited) {
    visited[get_idx(node)] = true;
    cout << "Посещена вершина: " << node << endl;
    
    for (char neighbor : graph[get_idx(node)]) {
        if (!visited[get_idx(neighbor)]) {            
            dfs_recursive(neighbor, graph, visited);
        }
    }
}

void dfs(char start, const vector<vector<char>>& graph) {
    int n = graph.size();
    vector<bool> visited(n, false);
    dfs_recursive(start, graph, visited);
}


int main() {
    // Граф в виде списка смежности
    vector<vector<char>> graph = {
        {'1', '2'}, {'0', '3'}, {'0', '3'}, {'1', '2', '4'}, {'3'} 
    };
    
    cout << "DFS обход starting from vertex 0:" << endl;
    dfs('3', graph);
    
    return 0;
}