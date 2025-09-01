#include <functional>
#include <string>

struct Point {
    int x;
    int y;
    std::string name;

    // Оператор сравнения для равенства (ОБЯЗАТЕЛЕН для использования в unordered контейнерах)
    bool operator==(const Point& other) const {
        return x == other.x && y == other.y && name == other.name;
    }
};

// Специализация std::hash для нашего типа Point
namespace std {
    template<>
    struct hash<Point> {
        std::size_t operator()(const Point& p) const {
            // Начинаем с хеша первого поля
            std::size_t h = std::hash<int>{}(p.x);
            std::size_t gold = 0x9e3779b9;
            // Комбинируем с хешем второго поля.
            // 0x9e3779b9 - "магическое" число (золотое сечение), помогает хорошо перемешивать биты.
            h ^= (std::hash<int>{}(p.y) + 0x9e3779b9 + (h << 6) + (h >> 2));
            // Комбинируем с хешем третьего поля
            h ^= (std::hash<std::string>{}(p.name) + 0x9e3779b9 + (h << 6) + (h >> 2));
            return h;
        }
    };
}