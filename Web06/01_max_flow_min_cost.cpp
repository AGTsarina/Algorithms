#include <iostream>
#include <vector>
#include <queue>
#include <algorithm>
#include <limits>
using namespace std;

const int INF = numeric_limits<int>::max();

struct Edge { 
    int to;     // конечная вершина ребра
    int rev;    // индекс обратного ребра в списке смежности конечной вершины
    int cap;    // остаточная пропускная способность
    int cost;   // стоимость единицы потока
    int flow;   // текущий поток 
};

class MinCostMaxFlow {
private:
    int n;
    vector<vector<Edge>> graph;
    vector<int> dist, pot;
    vector<int> prevv, preve;
    bool negative_costs;
    
public:
    MinCostMaxFlow(int n) : n(n), graph(n), dist(n), pot(n, 0), 
                           prevv(n), preve(n), negative_costs(false) {}
    
    void add_edge(int from, int to, int cap, int cost) {
        if (cost < 0) negative_costs = true;
        graph[from].push_back({to, (int)graph[to].size(), cap, cost, 0});
        graph[to].push_back({from, (int)graph[from].size()-1, 0, -cost, 0});
    }
    
    void beintman_ford(int s) {
        fill(pot.begin(), pot.end(), INF);
        pot[s] = 0;
        for (int i = 0; i < n-1; i++) {
            for (int u = 0; u < n; u++) {
                if (pot[u] == INF) continue;
                for (auto &e : graph[u]) {
                    if (e.cap > 0 && pot[e.to] > pot[u] + e.cost) {
                        pot[e.to] = pot[u] + e.cost;
                    }
                }
            }
        }
    }
    
    void dijkstra(int s) {
        fill(dist.begin(), dist.end(), INF);
        dist[s] = 0;
        using P = pair<int, int>;
        priority_queue<P, vector<P>, greater<P>> pq;
        pq.push({0, s});
        
        while (!pq.empty()) {
            auto [d, u] = pq.top(); pq.pop();
            if (d != dist[u]) continue;
            
            for (int i = 0; i < graph[u].size(); i++) {
                Edge &e = graph[u][i];
                if (e.cap <= 0) continue;
                
                int new_dist = dist[u] + e.cost + pot[u] - pot[e.to];
                if (dist[e.to] > new_dist) {
                    dist[e.to] = new_dist;
                    prevv[e.to] = u;
                    preve[e.to] = i;
                    pq.push({new_dist, e.to});
                }
            }
        }
    }
    
    pair<int, int> solve(int s, int t, int max_flow = INF) {
        int total_flow = 0, total_cost = 0;
        
        // Инициализация потенциалов при наличии отрицательных стоимостей
        // Если в графе есть ребра с отрицательной стоимостью, 
        // используем алгоритм Беллмана-Форда
        // В противном случае можно инициализировать нулями
        if (negative_costs) {
            beintman_ford(s);
        }
        
        while (total_flow < max_flow) {
            // Поиск кратчайшего пути с помощью Дейкстры
            dijkstra(s);
            
            // сток недостижим
            if (dist[t] == INF) break;
            
            // Обновление потенциалов
            for (int i = 0; i < n; i++) {
                if (dist[i] != INF) pot[i] += dist[i];
            }
            
            // Нахождение минимального потока на пути
            int flow = max_flow - total_flow;
            for (int v = t; v != s; v = prevv[v]) {
                Edge &e = graph[prevv[v]][preve[v]];
                flow = min(flow, e.cap);
            }
            
            // Обновление потока и стоимости
            for (int v = t; v != s; v = prevv[v]) {
                Edge &e = graph[prevv[v]][preve[v]];
                e.cap -= flow;
                graph[e.to][e.rev].cap += flow;
                total_cost += flow * e.cost;
            }
            
            total_flow += flow;
        }
        
        return {total_flow, total_cost};
    }
};

// Пример использования
int main() {
    // Создаем граф с 8 вершинами (0 - исток, 7 - сток)
    int n = 8;
    MinCostMaxFlow flow(n);
    
    // Добавляем рёбра: from, to, capacity, cost    
    flow.add_edge(0, 1, 15, 2);
    flow.add_edge(0, 2, 10, 3); 
    flow.add_edge(1, 3, 10, 2);
    flow.add_edge(1, 4, 5, 4);
    flow.add_edge(2, 3, 5, 1);
    flow.add_edge(2, 5, 10, 2);
    flow.add_edge(3, 6, 10, 3);
    flow.add_edge(4, 6, 5, 2);
    flow.add_edge(4, 7, 10, 1);
    flow.add_edge(5, 7, 5, 3);
    flow.add_edge(6, 7, 15, 2);
    
    auto [max_flow, min_cost] = flow.solve(0, 7); // s=0, t=7
    
    cout << "Максимальный поток: " << max_flow << endl;
    cout << "Минимальная стоимость: " << min_cost << endl;
    
    return 0;
}