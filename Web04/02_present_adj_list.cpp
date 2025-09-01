#include <iostream>
#include <vector>

class Graph {
private:
    int numVertices;
    std::vector<std::vector<int>> adjList;

public:
    // Конструктор
    Graph(int n) : numVertices(n), adjList(n) {}

    // Добавление ребра
    void addEdge(int i, int j) {
        adjList[i].push_back(j);
        adjList[j].push_back(i); // Для неориентированного графа
    }

    void print() {
        for (int i = 0; i < numVertices; ++i) {
            std::cout << i << ": ";
            for (int j : adjList[i]) {
                std::cout << j << " ";
            }
            std::cout << std::endl;
        }
    }
};


int main() {
    Graph g(4);
    g.addEdge(0, 1);
    g.addEdge(0, 2);
    g.addEdge(1, 2);
    g.addEdge(2, 3);
    g.print();
    return 0;
}