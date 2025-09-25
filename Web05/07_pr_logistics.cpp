#include <iostream>
#include <vector>
#include <queue>
#include <string>
#include <map>
#include <climits>
#include <cmath>

using namespace std;

const int INF = INT_MAX;

struct Route {
    int to;
    int base_cost;
    int max_weight;
    bool has_customs;
    bool is_express;
    bool seasonal_increase;
};

struct DeliveryOption {
    int cost;
    int max_weight;
    vector<int> path;
};

// Функция для преобразования стоимости в строковый формат
string formatCost(int cost) {
    return to_string(cost) + " руб.";
}

vector<DeliveryOption> findOptimalDeliveryRoutes(int n, int source, int required_weight, 
                                                const vector<vector<Route>>& graph, 
                                                const vector<int>& city_weights) {
    // dist[город] = (стоимость, максимальный вес, путь)
    vector<DeliveryOption> options(n, {INF, 0, {}});
    options[source] = {0, INT_MAX, {source}};
    
    // Приоритетная очередь: (стоимость, город)
    priority_queue<pair<int, int>, vector<pair<int, int>>, greater<pair<int, int>>> pq;
    pq.push({0, source});
    
    while (!pq.empty()) {
        int current_cost = pq.top().first;
        int current_city = pq.top().second;
        pq.pop();
        
        if (current_cost > options[current_city].cost) {
            continue;
        }
        
        for (const auto& route : graph[current_city]) {
            int next_city = route.to;
            
            // Проверяем ограничение по весу
            if (required_weight > route.max_weight) {
                continue;
            }
            
            // Расчет стоимости с учетом всех факторов
            int route_cost = route.base_cost;
            if (route.has_customs) {
                route_cost += 500; // Таможенный сбор
            }
            if (route.seasonal_increase) {
                route_cost = ceil(route_cost * 1.2); // Зимняя наценка 20%
            }
            
            int new_cost = current_cost + route_cost;
            int new_max_weight = min(options[current_city].max_weight, route.max_weight);
            
            if (new_cost < options[next_city].cost) {
                options[next_city].cost = new_cost;
                options[next_city].max_weight = new_max_weight;
                options[next_city].path = options[current_city].path;
                options[next_city].path.push_back(next_city);
                
                pq.push({new_cost, next_city});
            }
        }
    }
    
    return options;
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
    int source = cityCodes["MOW"]; // начинаем из Москвы
    int required_weight = 4; // требуемый вес груза в тоннах
    
    // Создаём граф логистических маршрутов
    vector<vector<Route>> graph(n);
    
    // Добавляем маршруты
    graph[cityCodes["MOW"]].push_back({cityCodes["LED"], 1000, 5, false, false, false});
    graph[cityCodes["MOW"]].push_back({cityCodes["AER"], 2500, 3, false, false, false});
    graph[cityCodes["MOW"]].push_back({cityCodes["SVX"], 3500, 10, false, false, false});
    
    graph[cityCodes["LED"]].push_back({cityCodes["AER"], 4000, 2, false, false, false});
    graph[cityCodes["LED"]].push_back({cityCodes["SVX"], 4500, 4, false, false, false});
    
    graph[cityCodes["AER"]].push_back({cityCodes["SVX"], 3000, 3, true, false, false});
    graph[cityCodes["AER"]].push_back({cityCodes["OVB"], 7000, 5, false, false, true});
    
    graph[cityCodes["SVX"]].push_back({cityCodes["OVB"], 2000, 8, false, false, false});
    graph[cityCodes["SVX"]].push_back({cityCodes["VVO"], 12000, 6, false, false, false});
    
    graph[cityCodes["OVB"]].push_back({cityCodes["VVO"], 9000, 7, false, true, false});
    
    // Находим оптимальные маршруты доставки
    vector<DeliveryOption> delivery_options = findOptimalDeliveryRoutes(n, source, required_weight, graph, {});
    
    // Вывод результатов
    cout << "Минимальная стоимость доставки груза " << required_weight << " т из Москвы:\n\n";
    for (int i = 0; i < n; i++) {
        if (i == source) {
            cout << cityNames[i] << ": доставка не требуется (0 руб.)\n";
        } else if (delivery_options[i].cost == INF) {
            cout << cityNames[i] << ": доставка невозможна (превышение ограничений по весу или маршруту)\n";
        } else {
            cout << cityNames[i] << ": " << formatCost(delivery_options[i].cost) << "\n";
            cout << "  Маршрут: ";
            for (size_t j = 0; j < delivery_options[i].path.size(); j++) {
                if (j > 0) cout << " → ";
                cout << cityNames[delivery_options[i].path[j]];
            }
            cout << "\n";
        }
    }
    
    // Анализ для разных весов
    cout << "\n--- Анализ для разных весов груза ---\n";
    vector<int> test_weights = {2, 4, 6, 8};
    for (int weight : test_weights) {
        cout << "\nДля груза " << weight << " т:\n";
        vector<DeliveryOption> weight_options = findOptimalDeliveryRoutes(n, source, weight, graph, {});
        
        if (weight_options[cityCodes["VVO"]].cost == INF) {
            cout << "  До Владивостока: недостижим\n";
        } else {
            cout << "  До Владивостока: " << formatCost(weight_options[cityCodes["VVO"]].cost) << "\n";
        }
    }
    
    return 0;
}