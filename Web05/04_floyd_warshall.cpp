#include <iostream>
#include <vector>
#include <string>
#include <map>
#include <climits>
#include <cmath>

using namespace std;

const int INF = INT_MAX;

void floydWarshall(vector<vector<int>>& dist, 
                vector<vector<int>>& next, 
                const vector<vector<int>>& graph) {
    int n = dist.size();
    
    // Инициализация матриц
    for (int i = 0; i < n; i++) {
        for (int j = 0; j < n; j++) {
            if (i == j) {
                dist[i][j] = 0;
                next[i][j] = j;
            } else {                
                dist[i][j] = graph[i][j];
                if (dist[i][j] < INF) {
                    next[i][j] = j;
                } else {
                    next[i][j] = -1;
                }
            }
        }
    }
    
    // Алгоритм Флойда-Уоршелла
    for (int k = 0; k < n; k++) {
        for (int i = 0; i < n; i++) {
            for (int j = 0; j < n; j++) {
                if (dist[i][k] != INF && dist[k][j] != INF &&
                    dist[i][j] > dist[i][k] + dist[k][j]) {
                    dist[i][j] = dist[i][k] + dist[k][j];
                    next[i][j] = next[i][k];
                }
            }
        }
    }
}

// Восстановление пути
vector<int> reconstructPath(int i, int j, const vector<vector<int>>& next) {
    if (next[i][j] == -1) return {};
    
    vector<int> path;
    path.push_back(i);
    while (i != j) {
        i = next[i][j];
        path.push_back(i);
    }
    return path;
}

int main() {  
    int n = 6; // количество вершин
    int required_weight = 4; // требуемый вес груза в тоннах
    
    // Матрица маршрутов (n x n)
    vector<vector<int>> graph(n, vector<int>(n, INF));
    
    // Заполнение матрицы маршрутов
    graph[0][1] = 15;
    graph[0][2] = 3;
    graph[0][3] = 10;
    
    graph[1][2] = 2;
    graph[1][3] = 4;
    
    graph[2][3] = 3;
    graph[2][4] = 5;
    
    graph[3][4] = 8;
    graph[3][5] = 6;
    
    graph[4][5] = 7;
    
    // Матрицы для алгоритма Флойда-Уоршелла
    vector<vector<int>> dist(n, vector<int>(n, INF));
    vector<vector<int>> next(n, vector<int>(n, -1));
    
    // Выполнение алгоритма
    floydWarshall(dist, next, graph);
    
    // Вывод результатов
    cout << "Матрица минимальной стоимости доставки:\n\n";
    
    // Заголовок таблицы
    cout << "Из\\В\t";
    for (int j = 0; j < n; j++) {
        cout << j << "\t";
    }
    cout << "\n";
    
    // Данные таблицы
    for (int i = 0; i < n; i++) {
        cout << i << "\t";
        for (int j = 0; j < n; j++) {
            if (dist[i][j] == INF) {
                cout << "INF\t";
            } else {
                cout << dist[i][j] << "\t";
            }
        }
        cout << "\n";
    }
    
    // Примеры маршрутов
    cout << "\n--- Примеры оптимальных маршрутов ---\n";
    
    // 0 -> 5
    vector<int> path = reconstructPath(0, 5, next);
    if (!path.empty()) {
        cout << "0 → 5: " << dist[0][5] << "\n";
        cout << "Маршрут: ";
        for (size_t i = 0; i < path.size(); i++) {
            if (i > 0) cout << " → ";
            cout << path[i];
        }
        cout << "\n";
    }        
    return 0;
}
