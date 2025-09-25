#include <iostream>
#include <vector>
#include <stack>
#include <algorithm> // для std::reverse
using namespace std;

class Kosaraju {
private:
    int n; // количество вершин
    vector<vector<int>> graph; // исходный граф
    vector<vector<int>> reversedGraph; // транспонированный граф
    vector<bool> visited; // массив посещённых вершин
    stack<int> st; // стек для порядка завершения обхода
    vector<vector<int>> sccs; // список всех найденных КСС

    // Первый DFS для заполнения стека
    void dfsFirstPass(int u) {
        visited[u] = true;
        for (int v : graph[u]) {
            if (!visited[v]) {
                dfsFirstPass(v);
            }
        }
        st.push(u); // Кладём вершину в стек после обработки всех её соседей
    }

    // Второй DFS на транспонированном графе для нахождения КСС
    void dfsSecondPass(int u, vector<int>& component) {
        visited[u] = true;
        component.push_back(u);
        for (int v : reversedGraph[u]) {
            if (!visited[v]) {
                dfsSecondPass(v, component);
            }
        }
    }

public:
    Kosaraju(int nodes) : n(nodes), graph(nodes), reversedGraph(nodes), visited(nodes, false) {}

    // Метод для добавления ребра в граф
    void addEdge(int u, int v) {
        graph[u].push_back(v);
        reversedGraph[v].push_back(u); // Сразу строим транспонированный граф
    }

    // Основной метод для поиска всех КСС
    vector<vector<int>> findSCCs() {
        // Шаг 1: Первый проход DFS на исходном графе
        visited.assign(n, false);
        for (int i = 0; i < n; ++i) {
            if (!visited[i]) {
                dfsFirstPass(i);
            }
        }

        // Шаг 2: Второй проход DFS на транспонированном графе
        visited.assign(n, false);
        while (!st.empty()) {
            int u = st.top();
            st.pop();
            if (!visited[u]) {
                vector<int> component;
                dfsSecondPass(u, component);
                // (Опционально) Сортируем компоненту для красивого вывода
                // sort(component.begin(), component.end());
                sccs.push_back(component);
            }
        }
        return sccs;
    }
};

int main() {
    // Создаём граф с 7 вершинами (0..6)
    int n = 7;
    Kosaraju solver(n);

    // Добавляем рёбра
    solver.addEdge(0, 1);
    solver.addEdge(1, 2);
    solver.addEdge(2, 0);
    solver.addEdge(2, 3);
    solver.addEdge(3, 4);
    solver.addEdge(4, 5);
    solver.addEdge(5, 3);
    solver.addEdge(5, 6);

    // Находим все компоненты сильной связности
    vector<vector<int>> sccs = solver.findSCCs();

    // Выводим результат
    cout << "Количество компонент сильной связности: " << sccs.size() << endl;
    for (size_t i = 0; i < sccs.size(); ++i) {
        cout << "Компонента " << i + 1 << ": ";
        for (int node : sccs[i]) {
            cout << node << " ";
        }
        cout << endl;
    }

    return 0;
}