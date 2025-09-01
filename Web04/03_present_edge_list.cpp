#include <iostream>
#include <vector>

struct Edge {
    int u, v;
    Edge(int u, int v) : u(u), v(v) {}
};

class Graph {
private:
    std::vector<Edge> edges;

public:
    void addEdge(int u, int v) {
        edges.emplace_back(u, v);
    }

    void print() {
        for (const Edge& e : edges) {
            std::cout << e.u << " - " << e.v << std::endl;
        }
    }
};

// Пример использования
int main() {
    Graph g;
    g.addEdge(0, 1);
    g.addEdge(0, 2);
    g.addEdge(1, 2);
    g.addEdge(2, 3);
    g.print();
    return 0;
}