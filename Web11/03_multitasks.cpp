#include <iostream>
#include <vector>
#include <algorithm>
#include <queue>

using namespace std;

struct Task {
    int id;
    int duration;
};

// Структура для станка: текущая нагрузка и список задач
struct Machine {
    int totalTime = 0;
    vector<int> taskIds;
};

// Для очереди с приоритетом: нам нужен станок с минимальной нагрузкой
struct CompareMachine {
    bool operator()(const Machine &a, const Machine &b) {
        return a.totalTime > b.totalTime; // Min-heap по totalTime
    }
};

bool compareByDurationDesc(const Task &a, const Task &b) {
    return a.duration > b.duration;
}

int main() {
    vector<Task> tasks({{0, 5}, {1, 7}, {2, 5}, {3, 9}, {4, 2}, {5, 3}, {6, 4}, {7, 1}});
    int numMachines = 3, numTasks = tasks.size();
    
    // ЖАДНЫЙ АЛГОРИТМ LPT
    // Шаг 1: Сортировка задач по убыванию длительности
    sort(tasks.begin(), tasks.end(), compareByDurationDesc);

    // Шаг 2: Инициализация станков с помощью min-heap
    priority_queue<Machine, vector<Machine>, CompareMachine> machineQueue;
    for (int i = 0; i < numMachines; i++) {
        machineQueue.push(Machine());
    }

    // Шаг 3: Распределение задач
    vector<Machine> machines(numMachines);
    
    for (const Task &task : tasks) {
        // Берем станок с минимальной текущей нагрузкой
        Machine leastLoaded = machineQueue.top();
        machineQueue.pop();

        // Назначаем задачу на этот станок
        leastLoaded.totalTime += task.duration;
        leastLoaded.taskIds.push_back(task.id);

        // Возвращаем станок назад в очередь
        machineQueue.push(leastLoaded);
    }

    // Собираем итоговые данные из очереди
    // Для этого нужно извлечь все элементы (они будут не отсортированы)
    vector<Machine> result;
    while (!machineQueue.empty()) {
        result.push_back(machineQueue.top());
        machineQueue.pop();
    }

    // Находим время работы станков
    int span = 0;
    for (const Machine &m : result) {
        if (m.totalTime > span) {
            span = m.totalTime;
        }
    }

    // Вывод результатов
    cout << "\nРаспределение задач по станкам:\n";
    for (int i = 0; i < numMachines; i++) {
        cout << "Станок " << i + 1 << " (общее время: " << result[i].totalTime << "): ";
        for (int taskId : result[i].taskIds) {
            cout << "Z" << taskId << " ";
        }
        cout << endl;
    }
    cout << "Время завершения: " << span << endl;

    return 0;
}