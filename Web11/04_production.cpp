#include <iostream>
#include <vector>
#include <algorithm>
#include <iomanip>

using namespace std;

struct Product {
    int id;
    double time;      // время на единицу (часы)
    double material;  // материал на единицу (кг)
    double cost;      // себестоимость единицы (деньги)
    double profit;    // прибыль на единицу (деньги)
    double quantity;  // количество для производства (результат)
};

// Функция для сравнения продуктов по эффективности
bool compareEfficiency(const Product &a, const Product &b) {
    // Расчет эффективности как прибыль на сумму взвешенных ресурсов
    double efficiency_a = a.profit / (0.4*a.time + 0.3*a.material + 0.3*a.cost);
    double efficiency_b = b.profit / (0.4*b.time + 0.3*b.material + 0.3*b.cost);
    return efficiency_a > efficiency_b;
}

// Жадный алгоритм распределения ресурсов
void optimizeProduction(vector<Product> &products, double max_time, 
                       double max_material, double max_budget) {
    // Сортируем продукты по эффективности (жадный выбор)
    sort(products.begin(), products.end(), compareEfficiency);
    
    double remaining_time = max_time;
    double remaining_material = max_material;
    double remaining_budget = max_budget;
    
    cout << "Оптимальный план производства:\n";
    cout << "===============================\n";
    
    for (auto &product : products) {
        // Максимально возможное количество для текущего продукта
        // с учетом каждого ограничения в отдельности
        double max_by_time = remaining_time / product.time;
        double max_by_material = remaining_material / product.material;
        double max_by_budget = remaining_budget / product.cost;
        
        // Берем минимум из всех ограничений
        product.quantity = min({max_by_time, max_by_material, max_by_budget});
        
        // Если нельзя произвести ни одной единицы, переходим к следующему
        if (product.quantity <= 0) {
            product.quantity = 0;
            continue;
        }
        
        // Обновляем оставшиеся ресурсы
        remaining_time -= product.quantity * product.time;
        remaining_material -= product.quantity * product.material;
        remaining_budget -= product.quantity * product.cost;
        
        // Выводим информацию о продукте
        double efficiency = product.profit / (0.4*product.time + 0.3*product.material + 0.3*product.cost);
        cout << "Продукт " << product.id << ":\n";
        cout << "  Количество: " << fixed << setprecision(2) << product.quantity << " ед.\n";
        cout << "  Прибыль: " << product.quantity * product.profit << " д.е.\n";
        cout << "  Эффективность: " << efficiency << " прибыль/ресурс\n";
        cout << "  Затраты времени: " << product.quantity * product.time << " ч.\n";
        cout << "  Затраты материала: " << product.quantity * product.material << " кг.\n";
        cout << "  Затраты бюджета: " << product.quantity * product.cost << " д.е.\n\n";
    }
    
    // Расчет итоговых показателей
    double total_profit = 0;
    double total_time_used = 0;
    double total_material_used = 0;
    double total_cost_used = 0;
    
    for (const auto &product : products) {
        total_profit += product.quantity * product.profit;
        total_time_used += product.quantity * product.time;
        total_material_used += product.quantity * product.material;
        total_cost_used += product.quantity * product.cost;
    }
    
    cout << "ИТОГИ:\n";
    cout << "======\n";
    cout << "Общая прибыль: " << total_profit << " д.е.\n";
    cout << "Использовано времени: " << total_time_used << "/" << max_time << " ч. (" 
         << (total_time_used/max_time)*100 << "%)\n";
    cout << "Использовано материала: " << total_material_used << "/" << max_material 
         << " кг. (" << (total_material_used/max_material)*100 << "%)\n";
    cout << "Использовано бюджета: " << total_cost_used << "/" << max_budget 
         << " д.е. (" << (total_cost_used/max_budget)*100 << "%)\n";
}

int main() {
    // Инициализация данных
    vector<Product> products = {
        {1, 2.0, 5.0, 100.0, 50.0, 0},   // Продукт 1: 2ч, 5кг, 100д.е., прибыль 50
        {2, 3.0, 3.0, 80.0, 40.0, 0},    // Продукт 2: 3ч, 3кг, 80д.е., прибыль 40
        {3, 1.5, 7.0, 120.0, 60.0, 0},   // Продукт 3: 1.5ч, 7кг, 120д.е., прибыль 60
        {4, 4.0, 2.0, 90.0, 55.0, 0},    // Продукт 4: 4ч, 2кг, 90д.е., прибыль 55
        {5, 2.5, 4.0, 110.0, 65.0, 0}    // Продукт 5: 2.5ч, 4кг, 110д.е., прибыль 65
    };
    
    // Ограничения ресурсов
    double max_time = 100.0;        // 100 часов
    double max_material = 200.0;    // 200 кг материала
    double max_budget = 5000.0;     // 5000 д.е. бюджета
    
    // Запуск оптимизации
    optimizeProduction(products, max_time, max_material, max_budget);
    
    return 0;
}