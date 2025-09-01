#include <iostream>
#include <vector>
#include <algorithm>

class Graph {
private:
    int n;
    std::vector<std::vector<int>> adj;
    std::vector<int> index, lowlink;
    int idx;
    std::vector<std::pair<int, int>> bridges;

    void dfs(int u, int parent) {
        index[u] = lowlink[u] = idx++;
        for (int v : adj[u]) {
            if (v == parent) continue;
            if (index[v] == -1) {
                dfs(v, u);
                lowlink[u] = std::min(lowlink[u], lowlink[v]);
                if (lowlink[v] > index[u]) {
                    bridges.emplace_back(u, v);
                }
            } else {
                lowlink[u] = std::min(lowlink[u], index[v]);
            }
        }
    }

public:
    Graph(int n) : n(n), adj(n), index(n, -1), lowlink(n, -1), idx(0) {}

    void addEdge(int u, int v) {
        adj[u].push_back(v);
        adj[v].push_back(u);
    }

    void findBridges() {
        for (int i = 0; i < n; ++i) {
            if (index[i] == -1) {
                dfs(i, -1);
            }
        }
    }

    void printBridges() {
        std::cout << "Bridges:\n";
        for (const auto& bridge : bridges) {
            std::cout << bridge.first << " - " << bridge.second << "\n";
        }
    }
};

int main() {
    Graph g(6);
    g.addEdge(0, 1);
    g.addEdge(1, 2);
    g.addEdge(2, 0);
    g.addEdge(1, 3);
    g.addEdge(3, 4);
    g.addEdge(4, 5);
    g.addEdge(5, 3);

    g.findBridges();
    g.printBridges();
    return 0;
}