#include <iostream>
#include <vector>
#include <queue>
#include <limits>
#include <algorithm>

using namespace std;

const int INF = numeric_limits<int>::max();

// Структура для представления ребра графа
struct Edge {
    int from;
    int to;
    int w;
    Edge(int f, int t, int w) :from(f), to(t), w(w) {}
};

bool bellmanFord(int n, vector<Edge>& edges, int start,
                 vector<int>& dist, vector<int>& prev) {    
    dist.assign(n, INF);
    prev.assign(n, -1);
    dist[start] = 0;
    
    for (int i = 0; i < n - 1; i++) {
        for (const Edge& e : edges) {
            if (dist[e.from] != INF && dist[e.from] + e.w < dist[e.to]) {
                dist[e.to] = dist[e.from] + e.w;
                prev[e.to] = e.from;
            }
        }
    }
    
    for (const Edge& e : edges) {
        if (dist[e.from] != INF && dist[e.from] + e.w < dist[e.to]) {
            return false; // "Граф содержит отрицательный цикл"
        }
    }
    return true;
}

std::vector<int> getPath(const std::vector<int>& prev, int target) {
    vector<int> path;
    for (int v = target; v != -1; v = prev[v]) {
        path.push_back(v);
    }
    reverse(path.begin(), path.end());
    return path;
}


int main() {
    // Создаем граф (5 вершин)
    int n = 5;
    vector<Edge> graph;
    
    // Заполняем граф
    graph.push_back(Edge(0, 1, 4)); // 0->1 (4)
    graph.push_back(Edge(0, 2, 2)); // 0->2 (2)
    graph.push_back(Edge(1, 2, -3)); // 1->2 (-3)
    graph.push_back(Edge(1, 4, 1)); // 1->4 (1)
    graph.push_back(Edge(2, 3, 7)); // 2->3 (7)
    graph.push_back(Edge(3, 1, -2)); // 3->1 (-2)    
    
    // Выполняем алгоритм Беллмана-Форда
    vector<int> dist, prev;
    bellmanFord(n, graph, 0, dist, prev);
    
    // Выводим результаты
    cout << "Кратчайшие расстояния от вершины 0:\n";
    int vertexName = 0;
    for (int i = 0; i < n; i++) {
        cout << "До " << vertexName++ << ": " << dist[i] << endl;
    }
    
    // Восстанавливаем и выводим путь до F
    cout << "\nПуть до 4: ";
    vector<int> path = getPath(prev, 4);
    for (int i = 0; i < path.size(); i++) {
        if (i > 0) cout << " → ";
        cout << path[i];
    }
    cout << " (длина: " << dist[4] << ")" << endl;
    
    return 0;
}

