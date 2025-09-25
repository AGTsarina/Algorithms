#include <iostream>
#include <vector>
#include <queue>
#include <string>
#include <map>
#include <climits>

using namespace std;

const int INF = INT_MAX;

// Структура для хранения информации о рейсе
struct Flight {
    int to;
    int duration; // в минутах
};

// Функция для преобразования часов в минуты
int hoursToMinutes(double hours) {
    return static_cast<int>(hours * 60);
}

// Функция для преобразования минут в часы и минуты
string minutesToTime(int minutes) {
    int hours = minutes / 60;
    int mins = minutes % 60;
    return to_string(hours) + "ч " + to_string(mins) + "м";
}

vector<int> findOptimalFlightTimes(int n, int source, const vector<vector<Flight>>& graph) {
    vector<int> dist(n, INF);
    dist[source] = 0;
    
    // Приоритетная очередь: (время, город)
    priority_queue<pair<int, int>, vector<pair<int, int>>, greater<pair<int, int>>> pq;
    pq.push({0, source});
    
    while (!pq.empty()) {
        int current_time = pq.top().first;
        int current_city = pq.top().second;
        pq.pop();
        
        if (current_time > dist[current_city]) {
            continue;
        }
        
        for (const auto& flight : graph[current_city]) {
            int next_city = flight.to;
            int flight_time = flight.duration;
            int new_time = current_time + flight_time;
            
            if (new_time < dist[next_city]) {
                dist[next_city] = new_time;
                pq.push({new_time, next_city});
            }
        }
    }
    
    return dist;
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
    
    // Создаём граф рейсов
    vector<vector<Flight>> graph(n);
    
    // Добавляем рейсы (в минутах)
    graph[cityCodes["MOW"]].push_back({cityCodes["LED"], hoursToMinutes(1.5)});
    graph[cityCodes["MOW"]].push_back({cityCodes["AER"], hoursToMinutes(2.5)});
    graph[cityCodes["MOW"]].push_back({cityCodes["SVX"], hoursToMinutes(2.0)});
    
    graph[cityCodes["LED"]].push_back({cityCodes["AER"], hoursToMinutes(3.5)});
    graph[cityCodes["LED"]].push_back({cityCodes["SVX"], hoursToMinutes(3.0)});
    
    graph[cityCodes["AER"]].push_back({cityCodes["SVX"], hoursToMinutes(3.0)});
    graph[cityCodes["AER"]].push_back({cityCodes["OVB"], hoursToMinutes(5.0)});
    
    graph[cityCodes["SVX"]].push_back({cityCodes["OVB"], hoursToMinutes(2.0)});
    graph[cityCodes["SVX"]].push_back({cityCodes["VVO"], hoursToMinutes(8.0)});
    
    graph[cityCodes["OVB"]].push_back({cityCodes["VVO"], hoursToMinutes(6.0)});
    
    // Находим оптимальное время перелёта
    vector<int> optimal_times = findOptimalFlightTimes(n, source, graph);
    
    // Вывод результатов
    cout << "Минимальное время перелёта из Москвы (MOW):\n\n";
    for (int i = 0; i < n; i++) {
        if (i == source) {
            cout << cityNames[i] << ": вы уже здесь! (0 минут)\n";
        } else if (optimal_times[i] == INF) {
            cout << cityNames[i] << ": недостижим прямым или connecting рейсом\n";
        } else {
            cout << cityNames[i] << ": " << minutesToTime(optimal_times[i]) << "\n";
        }
    }
    
    // Демонстрация оптимальных маршрутов
    cout << "\n--- Примеры оптимальных маршрутов ---\n";
    cout << "Москва → Владивосток: " << minutesToTime(optimal_times[cityCodes["VVO"]]) << "\n";
    cout << "  (Москва → Екатеринбург → Владивосток)\n";
    
    cout << "Москва → Новосибирск: " << minutesToTime(optimal_times[cityCodes["OVB"]]) << "\n";
    cout << "  (Москва → Екатеринбург → Новосибирск)\n";
    
    cout << "Москва → Сочи: " << minutesToTime(optimal_times[cityCodes["AER"]]) << "\n";
    cout << "  (Прямой рейс)\n";
    
   
}