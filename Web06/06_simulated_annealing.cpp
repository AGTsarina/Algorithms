#include <iostream>
#include <vector>
#include <cmath>
#include <random>
#include <algorithm>
#include <chrono>
#include <iomanip>
#include <limits>
#include <fstream>

using namespace std;

// Вычисление стоимости пути в графе
double pathCost(const vector<int>& path, const vector<vector<double>>& graph) {
    double cost = 0.0;
    int n = path.size();
    for (int i = 0; i < n - 1; ++i) {
        cost += graph[path[i]][path[i+1]];
    }
    cost += graph[path.back()][path[0]]; // Замкнуть цикл
    return cost;
}

// Генерация соседнего решения, зависящая от температуры
vector<int> generateNeighbor(const vector<int>& current, double temperature, double initialTemp) {
    vector<int> neighbor = current;
    random_device rd;
    mt19937 gen(rd());
    
    // Нормализованная температура (0-1)
    double normalizedTemp = temperature / initialTemp;
    
    // Чем выше температура, тем более радикальные изменения
    if (normalizedTemp > 0.5) {
        // Высокая температура - большие изменения (перестановка двух случайных вершин)
        uniform_int_distribution<int> dist(0, neighbor.size() - 1);
        int i = dist(gen), j = dist(gen);
        swap(neighbor[i], neighbor[j]);
    } 
    else if (normalizedTemp > 0.2) {
        // Средняя температура - средние изменения (переворот подпоследовательности)
        uniform_int_distribution<int> dist(0, neighbor.size() - 1);
        int i = dist(gen), j = dist(gen);
        if (i > j) swap(i, j);
        reverse(neighbor.begin() + i, neighbor.begin() + j + 1);
    }
    else {
        // Низкая температура - маленькие изменения (перестановка соседних вершин)
        uniform_int_distribution<int> dist(0, neighbor.size() - 2);
        int i = dist(gen);
        swap(neighbor[i], neighbor[i+1]);
    }
    
    return neighbor;
}

// Алгоритм имитации отжига
vector<int> simulatedAnnealing(
    const vector<vector<double>>& graph,
    double initialTemp,
    double coolingRate,
    double minTemp,
    int maxIterations) {
    
    int n = graph.size();
    vector<int> currentPath(n);
    for (int i = 0; i < n; ++i) currentPath[i] = i;
    shuffle(currentPath.begin(), currentPath.end(), default_random_engine());
    
    vector<int> bestPath = currentPath;
    double currentCost = pathCost(currentPath, graph);
    double bestCost = currentCost;
    
    double temp = initialTemp;
    random_device rd;
    mt19937 gen(rd());
    uniform_real_distribution<double> prob(0.0, 1.0);
    
    int iteration = 0;
    while (temp > minTemp && iteration < maxIterations) {
        vector<int> neighbor = generateNeighbor(currentPath, temp, initialTemp);
        double neighborCost = pathCost(neighbor, graph);
        double delta = neighborCost - currentCost;
        
        if (delta < 0 || exp(-delta / temp) > prob(gen)) {
            currentPath = neighbor;
            currentCost = neighborCost;
            
            if (currentCost < bestCost) {
                bestPath = currentPath;
                bestCost = currentCost;
                cout << "Новое лучшее решение: " << bestCost << " (температура: " << temp << ")" << endl;
            }
        }
        
        temp *= coolingRate;
        iteration++;
    }
    
    return bestPath;
}

// Точное решение методом полного перебора (только для небольших графов)
vector<int> bruteForceTSP(const vector<vector<double>>& graph) {
    int n = graph.size();
    vector<int> path(n);
    for (int i = 0; i < n; ++i) path[i] = i;
    
    vector<int> bestPath = path;
    double bestCost = pathCost(path, graph);
    
    // Перебираем все перестановки
    while (next_permutation(path.begin(), path.end())) {
        double cost = pathCost(path, graph);
        if (cost < bestCost) {
            bestCost = cost;
            bestPath = path;
        }
    }
    
    return bestPath;
}

// Генерация случайного графа
vector<vector<double>> generateRandomGraph(int size, double minWeight, double maxWeight) {
    vector<vector<double>> graph(size, vector<double>(size, 0.0));
    random_device rd;
    mt19937 gen(rd());
    uniform_real_distribution<double> dist(minWeight, maxWeight);
    
    for (int i = 0; i < size; ++i) {
        for (int j = i + 1; j < size; ++j) {
            double weight = dist(gen);
            graph[i][j] = weight;
            graph[j][i] = weight;
        }
    }
    
    return graph;
}

void printGraph(const string &filename, const vector<vector<double>> &graph, int n){
    ofstream out(filename);
    out << "---\nconfig:\n\ttheme: redux\n\tlayout: fixed\n---\nflowchart TB\n";
    for(int i=0; i<n;i++){
        for(int j=i + 1; j < n; j++){
            if (graph[i][j] > 0){
                out << "\t" << i << "--"<< static_cast<int>(graph[i][j])<<"---" << j << "\n";
            }
        }
    }

}

void simulation(const vector<vector<double>>& graph, double coolingRate, double exactCost, bool compareExact){
    // Параметры алгоритма
    double initialTemp = 10000;    
    double minTemp = 1e-5;
    int maxIterations = 100000;
    int graphSize = graph.size();
    cout << "Запуск имитации отжига для графа из " << graphSize << " вершин при коэффициенте охлаждения " << coolingRate << endl;
    auto start = chrono::high_resolution_clock::now();
    
    vector<int> result = simulatedAnnealing(graph, initialTemp, coolingRate, minTemp, maxIterations);
    double resultCost = pathCost(result, graph);
    
    auto end = chrono::high_resolution_clock::now();
    chrono::duration<double> duration = end - start;
    
    cout << "\nРезультат имитации отжига:" << endl;
    cout << "Лучший путь: ";
    for (int node : result) cout << node << " ";
    cout << "\nСтоимость: " << resultCost << endl;
    cout << "Время выполнения: " << duration.count() << " секунд" << endl;
    if (compareExact) {
        cout << "\nОтклонение от оптимального решения: " 
             << fixed << setprecision(2) << ((resultCost - exactCost) / exactCost * 100) << "%" << endl;
        
        if (abs(resultCost - exactCost) < 1e-6) {
            cout << "Алгоритм нашел оптимальное решение!" << endl;
        } else {
            cout << "Алгоритм нашел приближенное решение." << endl;
        }
    }
}

double exact(const vector<vector<double>>& graph){
    cout << "\nПоиск точного решения полным перебором..." << endl;
    auto start = chrono::high_resolution_clock::now();
        
    vector<int> exactSolution = bruteForceTSP(graph);
    double exactCost = pathCost(exactSolution, graph);
        
    auto end = chrono::high_resolution_clock::now();
    chrono::duration<double> duration = end - start;
        
    cout << "Точное решение: ";
    for (int node : exactSolution) cout << node << " ";
    cout << "\nТочная стоимость: " << exactCost << endl;
    cout << "Время перебора: " << duration.count() << " секунд" << endl;
    return exactCost;
}


int main() {
    // Генерация графа с 10 вершинами
    int graphSize = 10;
    vector<vector<double>> graph = generateRandomGraph(graphSize, 1.0, 50.0);
    printGraph("graph.txt", graph, graphSize);
    // Точное решение для сравнения (только для небольших графов)    
    double exactCost = 0;
    if (graphSize < 11) exactCost = exact(graph);
    
    simulation(graph, 0.99, exactCost, graphSize < 11);
    simulation(graph, 0.995, exactCost, graphSize < 11);
    simulation(graph, 0.999, exactCost, graphSize < 11);
    simulation(graph, 0.9995, exactCost, graphSize < 11);
    
    
    
    
}