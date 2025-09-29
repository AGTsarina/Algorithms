#include <iostream>
#include <vector>
#include <string>

using namespace std;

class MazeSolverDivideConquer {
private:
    vector<vector<int>> maze;
    vector<vector<bool>> visited;
    int rows, cols;
    
    // Статистика для анализа
    struct Stats {
        int recursiveCalls = 0;
        int backtrackSteps = 0;
        int successfulPaths = 0;
        int deadEnds = 0;
    } stats;
    
    bool isValid(int x, int y) {
        return x >= 0 && x < rows && y >= 0 && y < cols && 
               maze[x][y] == 1 && !visited[x][y];
    }
    
    void printMazeWithPath(int currentX, int currentY) {
        vector<string> visualization(rows, string(cols, ' '));
        
        for (int i = 0; i < rows; i++) {
            for (int j = 0; j < cols; j++) {
                if (maze[i][j] == 0) visualization[i][j] = 'x';
                else if (visited[i][j]) visualization[i][j] = '.';
                else visualization[i][j] = ' ';
            }
        }
        visualization[currentX][currentY] = 'O'; // Текущая позиция
        
        for (const auto& row : visualization) {
            cout << row << endl;
        }
        cout << "-------------------" << endl;
    }
    
public:
    MazeSolverDivideConquer(vector<vector<int>> m) : maze(m) {
        rows = maze.size();
        cols = maze[0].size();
        visited = vector<vector<bool>>(rows, vector<bool>(cols, false));
    }
    
    bool solve(int startX, int startY, int endX, int endY) {
        stats.recursiveCalls++;
        
        // Базовый случай 1: достигли выхода
        if (startX == endX && startY == endY) {
            stats.successfulPaths++;
            visited[startX][startY] = true;
            cout << "ДОСТИГНУТ ВЫХОД!" << endl;
            printMazeWithPath(startX, startY);
            return true;
        }
        
        // Базовый случай 2: тупик
        if (!isValid(startX, startY)) {
            stats.deadEnds++;
            return false;
        }
        
        // РАЗДЕЛЯЙ: Помечаем текущую позицию
        visited[startX][startY] = true;
        cout << "Позиция: (" << startX << ", " << startY << ")" << endl;
        printMazeWithPath(startX, startY);
        
        // ВЛАСТВУЙ: Рекурсивно исследуем все направления
        vector<pair<int, int>> directions = {{1,0}, {0,1}, {-1,0}, {0,-1}}; // вниз, вправо, вверх, влево
        vector<bool> directionResults;
        
        for (auto [dx, dy] : directions) {
            int newX = startX + dx;
            int newY = startY + dy;
            directionResults.push_back(solve(newX, newY, endX, endY));
        }
        
        // ОБЪЕДИНЯЙ: Анализируем результаты подзадач
        bool pathFound = false;
        for (bool result : directionResults) {
            if (result) {
                pathFound = true;
                break;
            }
        }
        
        // Если ни один путь не ведет к выходу - ОТХОД НАЗАД
        if (!pathFound) {
            stats.backtrackSteps++;
            visited[startX][startY] = false; // Важный шаг отката!
            cout << "ОТХОД НАЗАД из (" << startX << ", " << startY << ")" << endl;
        }
        
        return pathFound;
    }
    
    void printStats() {
        cout << "\n=== СТАТИСТИКА АЛГОРИТМА ===" << endl;
        cout << "Рекурсивных вызовов: " << stats.recursiveCalls << endl;
        cout << "Шагов отхода назад: " << stats.backtrackSteps << endl;
        cout << "Тупиков: " << stats.deadEnds << endl;
        cout << "Успешных путей: " << stats.successfulPaths << endl;
    }
};

int main(){
    vector<vector<int>> maze = {
        {1, 1, 0, 1, 1},  // 0: стена, 1: проход
        {1, 0, 1, 1, 0},
        {1, 1, 1, 0, 1},
        {0, 0, 1, 1, 1},
        {1, 1, 0, 1, 1}
    };

    // Координаты
    pair<int, int> start = {0, 0};  // Левый верхний угол
    pair<int, int> end   = {4, 4};  // Правый нижний угол

    auto[x0, y0] = start;
    auto[xn, yn] = end;

    MazeSolverDivideConquer mazeSolver(maze);
    mazeSolver.solve(x0, y0, xn, yn);
    mazeSolver.printStats();
}


