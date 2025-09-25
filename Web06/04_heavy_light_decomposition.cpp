#include <iostream>
#include <vector>
#include <algorithm>
using namespace std;

class HLD {
private:
    int n, timer;
    vector<vector<int>> adj;
    vector<int> parent, depth, heavy, head, pos, size;
    
    int dfs(int u) {
        int size = 1;
        int max_child_size = 0;
        
        for (int v : adj[u]) {
            if (v != parent[u]) {
                parent[v] = u;
                depth[v] = depth[u] + 1;
                int child_size = dfs(v);
                size += child_size;
                
                if (child_size > max_child_size) {
                    max_child_size = child_size;
                    heavy[u] = v;
                }
            }
        }
        return size;
    }
    
    void decompose(int u, int h) {
        head[u] = h;
        pos[u] = timer++;
        
        if (heavy[u] != -1) {
            decompose(heavy[u], h);
        }
        
        for (int v : adj[u]) {
            if (v != parent[u] && v != heavy[u]) {
                decompose(v, v);
            }
        }
    }
    
public:
    HLD(int n) : n(n), timer(0) {
        adj.resize(n);
        parent.resize(n, -1);
        depth.resize(n, 0);
        heavy.resize(n, -1);
        head.resize(n, -1);
        pos.resize(n, -1);
        size.resize(n, 0);
    }
    
    void addEdge(int u, int v) {
        adj[u].push_back(v);
        adj[v].push_back(u);
    }
    
    void build(int root = 0) {
        parent[root] = -1;
        depth[root] = 0;
        dfs(root);
        decompose(root, root);
    }
    
    int query(int u, int v) {
        int res = 0;
        while (head[u] != head[v]) {
            if (depth[head[u]] < depth[head[v]]) {
                swap(u, v);
            }
            // Здесь выполняется запрос на отрезке [pos[head[u]], pos[u]]
            // res = max(res, segment_tree.query(pos[head[u]], pos[u]]);
            u = parent[head[u]];
        }
        
        if (depth[u] > depth[v]) {
            swap(u, v);
        }
        // Запрос на отрезке [pos[u], pos[v]]
        // res = max(res, segment_tree.query(pos[u], pos[v]]);
        return res;
    }
    
    void update(int u, int v, int value) {
        while (head[u] != head[v]) {
            if (depth[head[u]] < depth[head[v]]) {
                swap(u, v);
            }
            // Обновление отрезка [pos[head[u]], pos[u]]
            // segment_tree.update(pos[head[u]], pos[u], value);
            u = parent[head[u]];
        }
        
        if (depth[u] > depth[v]) {
            swap(u, v);
        }
        // Обновление отрезка [pos[u], pos[v]]
        // segment_tree.update(pos[u], pos[v], value);
    }
    
    int getPosition(int u) const { return pos[u]; }
    int getHead(int u) const { return head[u]; }
    int getParent(int u) const { return parent[u]; }
};

// Пример использования
int main() {
    int n = 7;
    HLD hld(n);
    
    // Построение дерева
    hld.addEdge(0, 1);
    hld.addEdge(0, 2);
    hld.addEdge(1, 3);
    hld.addEdge(1, 4);
    hld.addEdge(2, 5);
    hld.addEdge(2, 6);
    
    hld.build(0);
    
    cout << "HLD decomposition:" << endl;
    for (int i = 0; i < n; i++) {
        cout << "Node " << i << ": head=" << hld.getHead(i) 
             << ", pos=" << hld.getPosition(i) << endl;
    }
    
    // Пример запроса между вершинами 3 и 6
    int result = hld.query(3, 6);
    cout << "Query result between 3 and 6: " << result << endl;
    
    return 0;
}