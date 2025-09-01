#include <iostream>
#include <vector>
#include <stack>
#include <algorithm>

class Graph {
private:
    int n;
    std::vector<std::vector<int>> adj;
    std::vector<int> index, lowlink;
    std::vector<bool> onStack;
    std::stack<int> st;
    int idx;
    std::vector<std::vector<int>> sccs; // Хранит все КСС

    void dfs(int u) {
        index[u] = lowlink[u] = idx++;
        st.push(u);
        onStack[u] = true;

        for (int v : adj[u]) {
            if (index[v] == -1) { // если вершина не посещалась
                dfs(v);
                lowlink[u] = std::min(lowlink[u], lowlink[v]);
            } else if (onStack[v]) { // если проходили через эту вершину
                lowlink[u] = std::min(lowlink[u], index[v]);
            }
        }

        if (lowlink[u] == index[u]) { // найдена компонента связности, включающая вершину u
            std::vector<int> scc;
            int w;
            do {
                w = st.top();
                st.pop();
                onStack[w] = false;
                scc.push_back(w);
            } while (w != u);
            sccs.push_back(scc);
        }
    }

public:
    Graph(int n) : n(n), adj(n), index(n, -1), lowlink(n, -1), onStack(n, false), idx(0) {}

    void addEdge(int u, int v) {
        adj[u].push_back(v);
    }

    void findSCCs() {
        for (int i = 0; i < n; ++i) {
            if (index[i] == -1) { // если вершина не была посещена
                dfs(i); // обход в глубину
            }
        }
    }

    void printSCCs() {
        std::cout << "Strongly Connected Components:\n";
        for (const auto& scc : sccs) {
            std::cout << "[ ";
            for (int v : scc) {
                std::cout << v << " ";
            }
            std::cout << "]\n";
        }
    }
};

int main() {
    Graph g(8);
    g.addEdge(0, 1);
    g.addEdge(1, 2);
    g.addEdge(2, 0);
    g.addEdge(3, 4);
    g.addEdge(4, 5);
    g.addEdge(5, 3);
    g.addEdge(1, 3);
    g.addEdge(2, 5);
    g.addEdge(5, 6);
    g.addEdge(6, 7);
    g.addEdge(7, 6);

    g.findSCCs();
    g.printSCCs();
    return 0;
}