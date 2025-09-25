// Метод последовательного объединения компонент

#include <iostream>
#include <vector>
#include <stack>
#include <unordered_set>
#include <algorithm>
#include <memory>
using namespace std;

class IncrementalSCC {
private:
    int n; // количество вершин
    int next_component_id; // следующий ID для компоненты
    
    // Структура для представления компоненты сильной связности
    struct Component {
        int id;
        unordered_set<int> nodes;
        unordered_set<shared_ptr<Component>> out_edges;  // компоненты, от которых есть ребро
        unordered_set<shared_ptr<Component>> in_edges; // компоненты, к которым есть ребро
        
        Component(int comp_id) : id(comp_id) {}
        
        void add_node(int node) {
            nodes.insert(node);
        }
        
        bool contains(int node) const {
            return nodes.find(node) != nodes.end();
        }
    };
    
    vector<shared_ptr<Component>> node_to_component; // принадлежность вершины к компоненте
    vector<vector<int>> graph; // исходный граф
    
    // Поиск пути между компонентами в графе конденсации
    bool has_path(shared_ptr<Component> from, shared_ptr<Component> to, 
                 unordered_set<int>& visited) {
        if (from == to) return true;
        if (!visited.insert(from->id).second) return false;
        
        for (auto& neighbor : from->out_edges) {
            if (has_path(neighbor, to, visited)) {
                return true;
            }
        }
        return false;
    }
    
    // Объединение компонент
    shared_ptr<Component> merge_components(const vector<shared_ptr<Component>>& components_to_merge) {
        auto new_component = make_shared<Component>(next_component_id++);
        
        // Объединяем все вершины
        for (auto& comp : components_to_merge) {
            for (int node : comp->nodes) {
                new_component->add_node(node);
                node_to_component[node] = new_component;
            }
        }
        
        // Объединяем входящие и исходящие рёбра
        unordered_set<shared_ptr<Component>> all_in_edges;
        unordered_set<shared_ptr<Component>> all_out_edges;
        
        for (auto& comp : components_to_merge) {
            all_in_edges.insert(comp->in_edges.begin(), comp->in_edges.end());
            all_out_edges.insert(comp->out_edges.begin(), comp->out_edges.end());
        }
        
        // Удаляем самоссылки
        all_in_edges.erase(new_component);
        all_out_edges.erase(new_component);
        
        // Обновляем рёбра
        new_component->in_edges = all_in_edges;
        new_component->out_edges = all_out_edges;
        
        // Обновляем ссылки у соседей
        for (auto& in_comp : new_component->in_edges) {
            in_comp->out_edges.erase(new_component);
            in_comp->out_edges.insert(new_component);
        }
        
        for (auto& out_comp : new_component->out_edges) {
            out_comp->in_edges.erase(new_component);
            out_comp->in_edges.insert(new_component);
        }
        
        return new_component;
    }
    
public:
    IncrementalSCC(int nodes) : n(nodes), next_component_id(0), 
                               node_to_component(nodes), graph(nodes) {
        // Изначально каждая вершина - отдельная компонента
        for (int i = 0; i < n; ++i) {
            auto comp = make_shared<Component>(next_component_id++);
            comp->add_node(i);
            node_to_component[i] = comp;
        }
    }
    
    // Добавление ребра
    void addEdge(int u, int v) {
        graph[u].push_back(v);
        
        auto comp_u = node_to_component[u];
        auto comp_v = node_to_component[v];
        
        // Если вершины в одной компоненте - просто добавляем ребро
        if (comp_u == comp_v) {
            return;
        }
        
        // Добавляем ребро в граф конденсации
        comp_u->out_edges.insert(comp_v);
        comp_v->in_edges.insert(comp_u);
        
        // Проверяем, создаёт ли это ребро цикл
        unordered_set<int> visited;
        if (has_path(comp_v, comp_u, visited)) {
            // Найден цикл - нужно объединить компоненты на пути
            vector<shared_ptr<Component>> components_to_merge;
            unordered_set<int> visited2;
            
            // Находим все компоненты на пути от comp_v к comp_u
            find_components_on_path(comp_v, comp_u, components_to_merge, visited2);
            
            // Объединяем компоненты
            auto new_component = merge_components(components_to_merge);
        }
    }
    
    // Вспомогательная функция для поиска компонент на пути
    void find_components_on_path(shared_ptr<Component> current, shared_ptr<Component> target,
                                vector<shared_ptr<Component>>& result, 
                                unordered_set<int>& visited) {
        if (!visited.insert(current->id).second) return;
        
        result.push_back(current);
        
        if (current == target) return;
        
        for (auto& neighbor : current->out_edges) {
            unordered_set<int> temp_visited = visited;
            if (has_path(neighbor, target, temp_visited)) {
                find_components_on_path(neighbor, target, result, visited);
                return;
            }
        }
    }
    
    // Получение компоненты для вершины
    int getComponent(int node) {
        return node_to_component[node]->id;
    }
    
    // Вывод всех компонент
    void printComponents() {
        unordered_set<shared_ptr<Component>> printed;
        cout << "Компоненты сильной связности:" << endl;
        
        for (int i = 0; i < n; ++i) {
            auto comp = node_to_component[i];
            if (printed.find(comp) == printed.end()) {
                printed.insert(comp);
                cout << "Компонента " << comp->id << ": ";
                for (int node : comp->nodes) {
                    cout << node << " ";
                }
                cout << endl;
            }
        }
    }
    
    // Вывод графа конденсации
    void printCondensationGraph() {
        unordered_set<shared_ptr<Component>> printed;
        cout << "Граф конденсации:" << endl;
        
        for (int i = 0; i < n; ++i) {
            auto comp = node_to_component[i];
            if (printed.find(comp) == printed.end()) {
                printed.insert(comp);
                cout << "Компонента " << comp->id << " -> ";
                for (auto& out_comp : comp->out_edges) {
                    if (out_comp != comp) {
                        cout << out_comp->id << " ";
                    }
                }
                cout << endl;
            }
        }
    }
};

// Пример использования
int main() {
    int n = 4; // Вершины 0, 1, 2, 3
    IncrementalSCC scc(n);
    
    cout << "Исходное состояние:" << endl;
    scc.printComponents();
    cout << endl;
    
    cout << "Добавляем ребро 0 -> 1" << endl;
    scc.addEdge(0, 1);
    scc.printComponents();
    scc.printCondensationGraph();
    cout << endl;
    
    cout << "Добавляем ребро 1 -> 2" << endl;
    scc.addEdge(1, 2);
    scc.printComponents();
    scc.printCondensationGraph();
    cout << endl;
    
    cout << "Добавляем ребро 2 -> 0" << endl;
    scc.addEdge(2, 0);
    scc.printComponents();
    scc.printCondensationGraph();
    cout << endl;
    
    cout << "Добавляем ребро 2 -> 3" << endl;
    scc.addEdge(2, 3);
    scc.printComponents();
    scc.printCondensationGraph();
    cout << endl;
    
    cout << "Добавляем ребро 3 -> 2" << endl;
    scc.addEdge(3, 2);
    scc.printComponents();
    scc.printCondensationGraph();
    
    return 0;
}