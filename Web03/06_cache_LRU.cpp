#include <list>
#include <unordered_map>
#include <iostream>
using namespace std;

template<typename Key, typename Value>
class LRUCache {
private:
    // Типы для удобства чтения
    using ListIter = typename list<pair<Key, Value>>::iterator;
    
    // Основные структуры данных
    list<pair<Key, Value>> items;                         // Список пар (ключ, значение)
    unordered_map<Key, ListIter> cache;                        // Хеш-таблица: ключ → итератор в списке
    size_t capacity;                                                // Максимальная ёмкость кэша

public:
    LRUCache(size_t capacity) : capacity(capacity) {}
    
    // Операции
    Value get(Key key);
    void put(Key key, Value value);
    void display() const;
};

template<typename Key, typename Value>
Value LRUCache<Key, Value>::get(Key key) {
    // Ищем ключ в хеш-таблице
    auto it = cache.find(key);
    
    // Если ключ не найден
    if (it == cache.end()) {
        throw "Ключ не найден";
    }
    
    // Перемещаем элемент в начало списка (как недавно использованный)
    items.splice(items.begin(), items, it->second);
    
    // Возвращаем значение
    return it->second->second;
}

template<typename Key, typename Value>
void LRUCache<Key, Value>::put(Key key, Value value) {
    // Проверяем, есть ли уже такой ключ в кэше
    auto it = cache.find(key);
    
    if (it != cache.end()) {
        // Ключ уже существует - обновляем значение
        it->second->second = value;
        // Перемещаем элемент в начало списка
        items.splice(items.begin(), items, it->second);
        return;
    }
    
    // Если кэш полон, удаляем самый старый элемент
    if (items.size() >= capacity) {
        // Находим ключ последнего (самого старого) элемента в списке
        Key last_key = items.back().first;
        // Удаляем из хеш-таблицы
        cache.erase(last_key);
        // Удаляем из списка
        items.pop_back();
    }
    
    // Добавляем новый элемент в начало списка
    items.emplace_front(key, value);
    
    // Добавляем в хеш-таблицу: ключ → итератор на новый элемент
    cache[key] = items.begin();
}

template<typename Key, typename Value>
void LRUCache<Key, Value>::display() const {
    cout << "LRUCache (capacity: " << capacity << ", size: " << items.size() << "):" << endl;
    cout << "List (MRU to LRU): ";
    for (const auto& item : items) {
        cout << "(" << item.first << ":" << item.second << ") ";
    }
    cout << endl;
}

int main() {
    // Создаем кэш емкостью 3
    LRUCache<int, string> cache(3);
    
    // Добавляем элементы
    cache.put(1, "one");
    cache.put(2, "two");
    cache.put(3, "three");
    cache.display();   
    
    // Обращаемся к элементу 2, чтобы сделать его недавно использованным
    cout << "Get key 2: " << cache.get(2) << endl;
    cache.display();    
    
    // Добавляем новый элемент, вытесняя самый старый (1)
    cache.put(4, "four");
    cache.display();
        
    // Пытаемся получить значение для ключа 1 (который был вытеснен)
    try{
        cout << "Get key 1: ";
        cout << cache.get(1) << endl; 
    }
    catch(...){
        cerr << "Ошибка!\n";// Выведет сообщение об ошибке
    }
    
    return 0;
}
