#include <iostream>
#include <vector>
#include <string>
#include <map>
#include <climits>
#include <cmath>

using namespace std;

const int INF = INT_MAX;

struct Route {
    int base_cost;
    int max_weight;
    bool has_customs;
    bool is_express;
    bool seasonal_increase;
};

// Функция для вычисления стоимости маршрута с учетом всех факторов
int calculateRouteCost(const Route& route, int weight) {
    if (weight > route.max_weight) return INF;
    
    int cost = route.base_cost;
    if (route.has_customs) cost += 500;
    if (route.seasonal_increase) cost = ceil(cost * 1.2);
    return cost;
}

void floydWarshallWithConstraints(vector<vector<int>>& dist, 
                                 vector<vector<int>>& next, 
                                 const vector<vector<Route>>& graph, 
                                 int required_weight) {
    int n = dist.size();
    
    // Инициализация матриц
    for (int i = 0; i < n; i++) {
        for (int j = 0; j < n; j++) {
            if (i == j) {
                dist[i][j] = 0;
                next[i][j] = j;
            } else {
                int cost = calculateRouteCost(graph[i][j], required_weight);
                dist[i][j] = cost;
                if (cost < INF) {
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
    // Сопоставление кодов городов с их номерами
    map<string, int> cityCodes = {
        {"MOW", 0}, // Москва
        {"LED", 1}, // Санкт-Петербург
        {"AER", 2}, // Сочи
        {"SVX", 3}, // Екатеринбург
        {"OVB", 4}, // Новосибирск
        {"VVO", 5}  // Владивосток
    };
    
    // Сопоставление номеров городов с их названиями
    vector<string> cityNames = {
        "Москва (MOW)",
        "Санкт-Петербург (LED)",
        "Сочи (AER)",
        "Екатеринбург (SVX)",
        "Новосибирск (OVB)",
        "Владивосток (VVO)"
    };
    
    int n = 6; // количество городов
    int required_weight = 4; // требуемый вес груза в тоннах
    
    // Матрица маршрутов (n x n)
    vector<vector<Route>> graph(n, vector<Route>(n, {INF, 0, false, false, false}));
    
    // Заполнение матрицы маршрутов
    graph[cityCodes["MOW"]][cityCodes["LED"]] = {1000, 5, false, false, false};
    graph[cityCodes["MOW"]][cityCodes["AER"]] = {2500, 3, false, false, false};
    graph[cityCodes["MOW"]][cityCodes["SVX"]] = {3500, 10, false, false, false};
    
    graph[cityCodes["LED"]][cityCodes["AER"]] = {4000, 2, false, false, false};
    graph[cityCodes["LED"]][cityCodes["SVX"]] = {4500, 4, false, false, false};
    
    graph[cityCodes["AER"]][cityCodes["SVX"]] = {3000, 3, true, false, false};
    graph[cityCodes["AER"]][cityCodes["OVB"]] = {7000, 5, false, false, true};
    
    graph[cityCodes["SVX"]][cityCodes["OVB"]] = {2000, 8, false, false, false};
    graph[cityCodes["SVX"]][cityCodes["VVO"]] = {12000, 6, false, false, false};
    
    graph[cityCodes["OVB"]][cityCodes["VVO"]] = {9000, 7, false, true, false};
    
    // Матрицы для алгоритма Флойда-Уоршелла
    vector<vector<int>> dist(n, vector<int>(n, INF));
    vector<vector<int>> next(n, vector<int>(n, -1));
    
    // Выполнение алгоритма
    floydWarshallWithConstraints(dist, next, graph, required_weight);
    
    // Вывод результатов
    cout << "Матрица минимальной стоимости доставки для всех пар городов (груз " << required_weight << " т):\n\n";
    
    // Заголовок таблицы
    cout << "Из\\В\t";
    for (int j = 0; j < n; j++) {
        cout << cityNames[j].substr(0, 10) << "\t";
    }
    cout << "\n";
    
    // Данные таблицы
    for (int i = 0; i < n; i++) {
        cout << cityNames[i].substr(0, 10) << "\t";
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
    
    // Москва -> Владивосток
    vector<int> path = reconstructPath(cityCodes["MOW"], cityCodes["VVO"], next);
    if (!path.empty()) {
        cout << "Москва → Владивосток: " << dist[cityCodes["MOW"]][cityCodes["VVO"]] << " руб.\n";
        cout << "Маршрут: ";
        for (size_t i = 0; i < path.size(); i++) {
            if (i > 0) cout << " → ";
            cout << cityNames[path[i]];
        }
        cout << "\n";
    }
    
    // Санкт-Петербург -> Новосибирск
    path = reconstructPath(cityCodes["LED"], cityCodes["OVB"], next);
    if (!path.empty()) {
        cout << "\nСанкт-Петербург → Новосибирск: " << dist[cityCodes["LED"]][cityCodes["OVB"]] << " руб.\n";
        cout << "Маршрут: ";
        for (size_t i = 0; i < path.size(); i++) {
            if (i > 0) cout << " → ";
            cout << cityNames[path[i]];
        }
        cout << "\n";
    }
    
    // Анализ для разных весов
    cout << "\n--- Анализ для разных весов груза ---\n";
    vector<int> test_weights = {2, 4, 6, 8};
    
    for (int weight : test_weights) {
        cout << "\nДля груза " << weight << " т:\n";
        
        // Пересчитываем матрицу для текущего веса
        vector<vector<int>> dist_weight(n, vector<int>(n, INF));
        vector<vector<int>> next_weight(n, vector<int>(n, -1));
        floydWarshallWithConstraints(dist_weight, next_weight, graph, weight);
        
        // Проверяем доступность Владивостока из Москвы
        if (dist_weight[cityCodes["MOW"]][cityCodes["VVO"]] == INF) {
            cout << "  Москва → Владивосток: недостижим\n";
        } else {
            cout << "  Москва → Владивосток: " << dist_weight[cityCodes["MOW"]][cityCodes["VVO"]] << " руб.\n";
        }
    }
    
    return 0;
}