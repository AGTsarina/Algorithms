#include <iostream>
#include <vector>
#include <algorithm>

using namespace std;

// Структура для хранения задачи: исходный индекс и время выполнения
struct Task {
    int id;
    int duration;
};

// Функция для сравнения задач по времени выполнения (для сортировки)
bool compareByDuration(const Task &a, const Task &b) {
    return a.duration < b.duration;
}

int main() {
    vector<Task> tasks({{0, 8}, {1, 4}, {2, 7}, {3, 1}, {4, 3}, {5, 2}});
    int n = tasks.size();   

    // ЖАДНЫЙ АЛГОРИТМ: сортировка по возрастанию длительности
    sort(tasks.begin(), tasks.end(), compareByDuration);

    // Вывод оптимального расписания
    cout << "\nОптимальный порядок выполнения задач:\n";
    int currentTime = 0;
    int totalCompletionTime = 0;

    for (const Task &task : tasks) {
        currentTime += task.duration; // Время завершения текущей задачи
        totalCompletionTime += currentTime; // Добавляем к суммарному времени
        cout << "Задача " << task.id << " (время: " << task.duration 
             << "), завершение в: " << currentTime << endl;
    }

    cout << "\nСуммарное время ожидания завершения: " << totalCompletionTime << endl;

    return 0;
}