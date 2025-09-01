#include <iostream>
#include <vector>
#include <algorithm>
#include <set>

class Graph {
private:
    int n;
    std::vector<std::vector<int>> adj;
    std::vector<int> index, lowlink;
    int idx;
    std::set<int> articulationPoints;

    void dfs(int u, int parent) {
        index[u] = lowlink[u] = idx++;
        int children = 0;
        for (int v : adj[u]) {
            if (v == parent) continue;
            if (index[v] == -1) {
                children++;
                dfs(v, u);
                lowlink[u] = std::min(lowlink[u], lowlink[v]);
                if (parent != -1 && lowlink[v] >= index[u]) {
                    articulationPoints.insert(u);
                }
            } else {
                lowlink[u] = std::min(lowlink[u], index[v]);
            }
        }
        if (parent == -1 && children > 1) {
            articulationPoints.insert(u);
        }
    }

public:
    Graph(int n) : n(n), adj(n), index(n, -1), lowlink(n, -1), idx(0) {}

    void addEdge(int u, int v) {
        adj[u].push_back(v);
        adj[v].push_back(u);
    }

    void findArticulationPoints() {
        for (int i = 0; i < n; ++i) {
            if (index[i] == -1) {
                dfs(i, -1);
            }
        }
    }

    void printArticulationPoints() {
        std::cout << "Articulation Points:\n";
        for (int ap : articulationPoints) {
            std::cout << ap << "\n";
        }
    }
};

int main() {
    Graph g(7);
    g.addEdge(0, 1);
    g.addEdge(1, 2);
    g.addEdge(2, 0);
    g.addEdge(1, 3);
    g.addEdge(1, 4);
    g.addEdge(1, 6);
    g.addEdge(3, 5);
    g.addEdge(4, 5);

    g.findArticulationPoints();
    g.printArticulationPoints();
    return 0;
}