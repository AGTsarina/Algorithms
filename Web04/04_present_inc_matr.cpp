#include <iostream>
#include <vector>

class Graph {
private:
    int numVertices, numEdges;
    std::vector<std::vector<int>> incMatrix;

public:
    Graph(int n, int m) : numVertices(n), numEdges(m), incMatrix(n, std::vector<int>(m, 0)) {}

    void addEdge(int edgeIdx, int u, int v) {
        incMatrix[u][edgeIdx] = 1;
        incMatrix[v][edgeIdx] = 1; // Для неориентированного графа
    }

    void print() {
        for (int i = 0; i < numVertices; ++i) {
            for (int j = 0; j < numEdges; ++j) {
                std::cout << incMatrix[i][j] << " ";
            }
            std::cout << std::endl;
        }
    }
};

int main() {
    Graph g(4, 4); // 4 вершины, 4 ребра
    g.addEdge(0, 0, 1);
    g.addEdge(1, 0, 2);
    g.addEdge(2, 1, 2);
    g.addEdge(3, 2, 3);
    g.print();
    return 0;
}