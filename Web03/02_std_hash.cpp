#include <iostream>
#include <functional> // для std::hash
#include <string>

int main() {
    std::string key = "Hello, Hash!";

    // Создаем объект-функцию хеширования для std::string
    std::hash<std::string> string_hasher;

    // Вычисляем хеш
    std::size_t h = string_hasher(key);
    // Или просто: std::size_t h = std::hash<std::string>{}(key);

    std::cout << "Хеш для строки '" << key << "' равен: " << h << std::endl;

    // Также работает для других типов
    std::hash<int> int_hasher;
    std::cout << "Хеш для числа 42: " << int_hasher(42) << std::endl;

    return 0;
}