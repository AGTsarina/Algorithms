#include <iostream>
#include <vector>
#include <algorithm>
#include <iomanip>
#include <functional>

using namespace std;

// Структура для представления заявки (интервала)
struct Interval {
    int id;          // идентификатор заявки
    int start;       // время начала
    int end;         // время окончания
    int duration() const { return end - start; } // длительность заявки
};

// Функция для сравнения заявок по времени окончания (для сортировки)
bool compareByEndTime(const Interval &a, const Interval &b) {
    return a.end < b.end;
}

// Функция для сравнения заявок по времени начала (для альтернативной стратегии)
bool compareByStartTime(const Interval &a, const Interval &b) {
    return a.start < b.start;
}

// Функция для сравнения заявок по длительности (для альтернативной стратегии)
bool compareByDuration(const Interval &a, const Interval &b) {
    return a.duration() < b.duration();
}

// Жадный алгоритм выбора заявок по одной из стратегий
// по времени окончания (оптимальная стратегия)
// по времени начала (не оптимальная)
// по длительности (не оптимальная)
vector<Interval> selectIntervals(vector<Interval> intervals, function<bool(const Interval &a, const Interval &b)> compareStrategy) {
    if (intervals.empty()) return {};
    
    // Сортируем заявки по времени окончания
    sort(intervals.begin(), intervals.end(), compareStrategy);
    
    vector<Interval> selected;
    int lastEndTime = -1; // время окончания последней выбранной заявки
    
    for (const auto &interval : intervals) {
        // Если текущая заявка начинается после окончания последней выбранной
        if (interval.start >= lastEndTime) {
            selected.push_back(interval);
            lastEndTime = interval.end;
        }
    }
    
    return selected;
}



// Функция для вывода расписания
void printSchedule(const vector<Interval> &schedule, const string &strategyName) {
    cout << "\n" << strategyName << ":\n";
    cout << "Количество выбранных заявок: " << schedule.size() << "\n";
    cout << "Выбранные заявки:\n";
    cout << "ID\tНачало\tОкончание\tДлительность\n";
    cout << "----------------------------------------\n";
    
    for (const auto &interval : schedule) {
        cout << interval.id << "\t" << interval.start << "\t" << interval.end 
             << "\t\t" << interval.duration() << "\n";
    }
}

// Функция для создания тестового набора заявок
vector<Interval> createTestIntervals() {
    return {
        {1, 1, 3},   // ID, Start, End
        {2, 2, 5},
        {3, 4, 7},
        {4, 6, 9},
        {5, 8, 10},
        {6, 9, 11},
        {7, 11, 14},
        {8, 13, 15}
    };
}

// Функция для создания контрпримера для стратегии "раннее начало"
vector<Interval> createCounterExample() {
    return {
        {1, 0, 10},  // Длинная ранняя заявка
        {2, 1, 2},   // Короткие заявки, которые можно все выбрать
        {3, 3, 4},
        {4, 5, 6},
        {5, 7, 8}
    };
}

int main() {
    cout << "=== ОПТИМИЗАЦИЯ РАСПИСАНИЯ ЗАЯВОК ===\n";
    
    // Тестовый пример 1: Общий случай
    cout << "\nТЕСТ 1: Общий случай\n";
    vector<Interval> intervals = createTestIntervals();
    
    // Выводим все заявки
    cout << "Все заявки:\n";
    cout << "ID\tНачало\tОкончание\tДлительность\n";
    cout << "----------------------------------------\n";
    for (const auto &interval : intervals) {
        cout << interval.id << "\t" << interval.start << "\t" << interval.end 
             << "\t\t" << interval.duration() << "\n";
    }
    
    // Сравниваем разные стратегии
    auto optimalSchedule = selectIntervals(intervals, compareByEndTime);
    auto startTimeSchedule = selectIntervals(intervals, compareByStartTime);
    auto durationSchedule = selectIntervals(intervals, compareByDuration);
    
    printSchedule(optimalSchedule, "СТРАТЕГИЯ: Раннее окончание (ОПТИМАЛЬНАЯ)");
    printSchedule(startTimeSchedule, "СТРАТЕГИЯ: Раннее начало");
    printSchedule(durationSchedule, "СТРАТЕГИЯ: Короткая длительность");
    
    // Тестовый пример 2: Контрпример для стратегии "раннее начало"
    cout << "\n\nТЕСТ 2: Контрпример для стратегии 'Раннее начало'\n";
    vector<Interval> counterExample = createCounterExample();
    
    cout << "Все заявки:\n";
    cout << "ID\tНачало\tОкончание\tДлительность\n";
    cout << "----------------------------------------\n";
    for (const auto &interval : counterExample) {
        cout << interval.id << "\t" << interval.start << "\t" << interval.end 
             << "\t\t" << interval.duration() << "\n";
    }
    
    auto optimal2 = selectIntervals(counterExample, compareByEndTime);
    auto startTime2 = selectIntervals(counterExample, compareByStartTime);
    
    printSchedule(optimal2, "СТРАТЕГИЯ: Раннее окончание (ОПТИМАЛЬНАЯ)");
    printSchedule(startTime2, "СТРАТЕГИЯ: Раннее начало (НЕОПТИМАЛЬНАЯ)");
    
    // Демонстрация работы алгоритма шаг за шагом
    cout << "\n\nДЕМОНСТРАЦИЯ АЛГОРИТМА (шаг за шагом):\n";
    cout << "=====================================\n";
    
    vector<Interval> demoIntervals = {
        {1, 1, 3}, {2, 2, 5}, {3, 4, 7}, {4, 6, 9}
    };
    
    sort(demoIntervals.begin(), demoIntervals.end(), compareByEndTime);
    
    cout << "Заявки после сортировки по времени окончания:\n";
    for (const auto &interval : demoIntervals) {
        cout << "Заявка " << interval.id << ": [" << interval.start 
             << ", " << interval.end << "]\n";
    }
    
    cout << "\nПроцесс выбора:\n";
    vector<Interval> selected;
    int lastEnd = -1;
    
    for (const auto &interval : demoIntervals) {
        cout << "Рассматриваем заявку " << interval.id << ": [" 
             << interval.start << ", " << interval.end << "]";
        
        if (interval.start >= lastEnd) {
            selected.push_back(interval);
            lastEnd = interval.end;
            cout << " -> ВЫБРАНА (lastEnd = " << lastEnd << ")\n";
        } else {
            cout << " -> ПРОПУЩЕНА (пересекается с последней выбранной)\n";
        }
    }
    
    cout << "\nИтоговое расписание:\n";
    for (const auto &interval : selected) {
        cout << "Заявка " << interval.id << ": [" << interval.start 
             << ", " << interval.end << "]\n";
    }
    
    return 0;
}