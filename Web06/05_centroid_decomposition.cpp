#include <iostream>
#include <vector>
#include <algorithm>
#include <cstring>
#include <functional>
using namespace std;

class CentroidDecomposition {
private:
    int n;
    vector<vector<int>> adj;        // Исходное дерево, список смежности исходного дерева
    vector<vector<int>> treeCD;     // Дерево центроидов, список смежности для дерева центроидов
    vector<int> parentCD;           // Родители в дереве центроидов, массив, хранящий родителя каждой вершины в дереве центроидов
    vector<int> size;               // Размеры поддеревьев
    vector<bool> removed;           // Отметки об удаленных  (обработанных) вершинах в процессе центроидной декомпозиции
    int rootCD;                     // Корень дерева центроидов

    // Вычисление размеров поддеревьев
    void dfsSize(int u, int p) {
        size[u] = 1;
        for (int v : adj[u]) {
            if (v == p || removed[v]) continue;
            dfsSize(v, u);
            size[u] += size[v];
        }
    }

    // Поиск центроида в поддереве с корнем u
    int findCentroid(int u, int p, int totalSize) {
        for (int v : adj[u]) {
            if (v == p || removed[v]) continue;
            if (size[v] > totalSize / 2) {
                return findCentroid(v, u, totalSize);
            }
        }
        return u;
    }

    // Рекурсивная декомпозиция
    int decompose(int u, int p) {
        dfsSize(u, -1);
        int centroid = findCentroid(u, -1, size[u]);
        removed[centroid] = true;
        parentCD[centroid] = p;
        
        if (p == -1) {
            rootCD = centroid;
        } else {
            treeCD[p].push_back(centroid);
            treeCD[centroid].push_back(p);
        }
        
        // Рекурсивно обрабатываем компоненты
        for (int v : adj[centroid]) {
            if (!removed[v]) {
                int childCentroid = decompose(v, centroid);
                // Здесь можно добавить логику для решения конкретной задачи
            }
        }
        
        return centroid;
    }

public:
    CentroidDecomposition(int n) : n(n) {
        adj.resize(n);
        treeCD.resize(n);
        parentCD.resize(n, -1);
        size.resize(n);
        removed.assign(n, false);
    }
    
    void addEdge(int u, int v) {
        adj[u].push_back(v);
        adj[v].push_back(u);
    }
    
    void build() {
        // вызов рекурсивной функции
        decompose(0, -1);
    }
    
    // Пример: подсчет путей длины k
    long long countPathsOfLength(int k) {
        vector<int> depth(n, 0);
        long long result = 0;
        
        // Вспомогательная функция для подсчета путей через центроид:
        // функция для обхода поддерева и сбора количества вершин на каждой глубине от текущей вершины.
        // u: текущая вершина
        // p: родительская вершина
        // d: текущая глубина
        // depths: вектор, куда записывается найденные "глубины" d
        function<void(int, int, int, vector<int>&)> countPaths = 
        [&](int u, int p, int d, vector<int>& depths) {
            if (d > k) return;
            depths.push_back(d);
            for (int v : adj[u]) {
                if (v == p || removed[v]) continue;
                // переход на следующий уровень
                countPaths(v, u, d + 1, depths);
            }
        };
        
        // Функция для обработки центроида и подсчета путей через него
        function<void(int)> processCentroid = 
        [&](int centroid) {
            removed[centroid] = true;
            vector<int> allDepths;   //  Хранить количество вершин на каждой глубине от центроида во всех обработанных поддеревьях
            allDepths.push_back(0); // Путь из центроида в себя
            
            for (int v : adj[centroid]) {
                if (removed[v]) continue;
                
                vector<int> depths;
                countPaths(v, centroid, 1, depths); // собираем глубины вершин в векторе depths
                
                // Суммируем пути в одном поддереве
                for (int d : depths) {
                    if (k - d >= 0 && k - d < allDepths.size()) {
                        result += allDepths[k - d];   // d + (k - d) = k
                    }
                }
                
                // Добавляем глубины в общий массив
                for (int d : depths) {
                    if (d < allDepths.size()) {
                        allDepths[d]++;
                    } else {
                        allDepths.resize(d + 1, 0);
                        allDepths[d] = 1;
                    }
                }
            }
            
            // Добавляем пути через центроид
            if (k < allDepths.size()) {
                result += allDepths[k];
            }
            
            // Рекурсивно обрабатываем дочерние центроиды
            for (int v : treeCD[centroid]) {
                if (v != parentCD[centroid]) {
                    processCentroid(v);
                }
            }
        };
        
        // Сбрасываем массив removed для использования в подсчете путей
        removed.assign(n, false);  //  временный массив для отметки посещенных вершин
        processCentroid(rootCD);
        
        return result;
    }
    
    void printDecomposition() {
        cout << "Centroid Decomposition (Дерево):" << endl;
        for (int i = 0; i < n; i++) {
            if (!treeCD[i].empty()) {
                cout << i << " -> ";
                for (int j : treeCD[i]) {
                    cout << j << " ";
                }
                cout << endl;
            }
        }
        
        cout << "\nИерархия:" << endl;
        for (int i = 0; i < n; i++) {
            if (parentCD[i] != -1) {
                cout << "Родителем " << i << " является " << parentCD[i] << endl;
            }
        }
    }
};

int main() {
    int n = 10;  // количество вершин в дереве.
    CentroidDecomposition cd(n);
    
    // Построение дерева
    cd.addEdge(0, 1);
    cd.addEdge(0, 2);
    cd.addEdge(1, 3);
    cd.addEdge(1, 4);
    cd.addEdge(2, 5);
    cd.addEdge(2, 6);
    cd.addEdge(5, 7);
    cd.addEdge(5, 8);
    cd.addEdge(5, 9);
    
    // Построение декомпозиции
    cd.build();
    
    // Вывод результатов
    cd.printDecomposition();
    
    // Пример: подсчет путей длины 2
    long long paths = cd.countPathsOfLength(2);
    cout << "\nКоличество путей длины 2: " << paths << endl;
    
    return 0;
}