#include <iostream>
#include <vector>
#include <queue>
#include <limits>

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

pair<int, vector<pair<int, int>>> primPQ(const vector<vector<Edge>>& graph) {
    int n = graph.size();
    vector<int> dist(n, INF);
    vector<bool> visited(n, false);
    vector<int> parent(n, -1);
    priority_queue<QueueNode, vector<QueueNode>, greater<QueueNode>> pq;
    
    dist[0] = 0;
    pq.push(QueueNode(0, 0));
    int totalWeight = 0;
    
    while (!pq.empty()) {
        int u = pq.top().vertex;
        int w = pq.top().distance;
        pq.pop();
        
        if (visited[u]) continue;
        visited[u] = true;
        totalWeight += w;
        
        for (const auto& edge : graph[u]) {
            int v = edge.to;
            int weight = edge.weight;
            if (!visited[v] && weight < dist[v]) {
                dist[v] = weight;
                parent[v] = u;
                pq.push(QueueNode(v, weight));
            }
        }
    }

    vector<pair<int, int>> mst;
    for(int i=0; i<n; i++) if (parent[i] >= 0) mst.push_back({parent[i], i});
    return {totalWeight, mst};
}


int main(){
     int n = 5;
    vector<vector<Edge>> graph(n);
    
    // Заполняем граф
    graph[0].push_back(Edge(1, 2)); 
    graph[0].push_back(Edge(2, 5)); 
    graph[0].push_back(Edge(3, 6)); 
    graph[1].push_back(Edge(2, 3)); 
    graph[1].push_back(Edge(3, 8));
    graph[1].push_back(Edge(4, 5)); 
    graph[2].push_back(Edge(3, 4)); 
    graph[2].push_back(Edge(4, 7));
    graph[3].push_back(Edge(4, 9));     
    auto [length, mst]  = primPQ(graph);
    cout << "Суммарный вес остовного дерева " << length << endl;
    for(const auto &edge:mst){
        cout << edge.first << " -> " << edge.second << endl;
    }

}