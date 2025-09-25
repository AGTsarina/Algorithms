#include <iostream>
#include <vector>
#include <queue>
#include <limits>
#include <algorithm>

using namespace std;

const int INF = numeric_limits<int>::max();

// Структура для представления ребра графа
struct Edge {
    int to;
    int weight;
    Edge(int t, int w) : to(t), weight(w) {}
};

// Структура для приоритетной очереди
struct QueueNode {
    int vertex;
    int distance;
    QueueNode(int v, int d) : vertex(v), distance(d) {}
    bool operator > (const QueueNode& other) const {
        return distance > other.distance;
    }
};

// Функция для выполнения алгоритма Дейкстры
void dijkstra(const vector<vector<Edge>>& graph, int start, 
              vector<int>& dist, vector<int>& prev) {
    int n = graph.size();
    dist.assign(n, INF);
    prev.assign(n, -1);
    
    priority_queue<QueueNode, vector<QueueNode>, greater<QueueNode>> pq;
    
    dist[start] = 0;
    pq.push(QueueNode(start, 0));
    
    while (!pq.empty()) {
        QueueNode node = pq.top();
        pq.pop();
        
        /*int u = node.vertex;
        int d = node.distance;*/
        
        // Пропускаем, так нашли ранее путь, который дешевле
        if (node.distance > dist[node.vertex]) continue;
        
        // Обрабатываем всех соседей вершины node.vertex
        for (const Edge& edge : graph[node.vertex]) {
            int v = edge.to;
            int new_dist = dist[node.vertex] + edge.weight;
            
            // Релаксация ребра
            if (new_dist < dist[v]) {
                dist[v] = new_dist;
                prev[v] = node.vertex;
                pq.push(QueueNode(node.vertex, new_dist));
            }
        }
    }
}

// Функция для восстановления пути
vector<int> getPath(const vector<int>& prev, int target) {
    vector<int> path;
    for (int v = target; v != -1; v = prev[v]) {
        path.push_back(v);
    }
    reverse(path.begin(), path.end());
    return path;
}

int main() {
    // Создаем граф (6 вершин: 0=A, 1=B, 2=C, 3=D, 4=E, 5=F)
    int n = 6;
    vector<vector<Edge>> graph(n);
    
    // Заполняем граф
    graph[0].push_back(Edge(1, 4)); // A->B (4)
    graph[0].push_back(Edge(2, 2)); // A->C (2)
    graph[1].push_back(Edge(2, 1)); // B->C (1)
    graph[1].push_back(Edge(3, 5)); // B->D (5)
    graph[2].push_back(Edge(1, 1)); // C->B (1)
    graph[2].push_back(Edge(3, 8)); // C->D (8)
    graph[2].push_back(Edge(4, 10)); // C->E (10)
    graph[3].push_back(Edge(4, 2)); // D->E (2)
    graph[3].push_back(Edge(5, 6)); // D->F (6)
    graph[4].push_back(Edge(5, 2)); // E->F (2)
    
    // Выполняем алгоритм Дейкстры
    vector<int> dist, prev;
    dijkstra(graph, 0, dist, prev);
    
    // Выводим результаты
    cout << "Кратчайшие расстояния от вершины A:\n";
    char vertexName = 'A';
    for (int i = 0; i < n; i++) {
        cout << "До " << vertexName++ << ": " << dist[i] << endl;
    }
    
    // Восстанавливаем и выводим путь до F
    cout << "\nПуть до F: ";
    vector<int> path = getPath(prev, 5);
    for (int i = 0; i < path.size(); i++) {
        if (i > 0) cout << " → ";
        cout << char('A' + path[i]);
    }
    cout << " (длина: " << dist[5] << ")" << endl;
    
    return 0;
}