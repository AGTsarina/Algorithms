#include <iostream>
#include <vector>
#include <algorithm>
#include <cmath>
#include <random>
#include <chrono>
#include <numeric>
using namespace std;

struct Point {
    double x, y;
};

class ClosestPairAnalyzer {
private:
    static double distance(const Point& a, const Point& b) {
        return sqrt((a.x - b.x) * (a.x - b.x) + (a.y - b.y) * (a.y - b.y));
    }
    
    static double bruteForceClosest(const vector<Point>& points, int left, int right) {
        double minDist = numeric_limits<double>::max();
        for (int i = left; i <= right; i++) {
            for (int j = i + 1; j <= right; j++) {
                double dist = distance(points[i], points[j]);
                if (dist < minDist) {
                    minDist = dist;
                }
            }
        }
        return minDist;
    }
    
    static double stripClosest(const vector<Point>& strip, double d) {
        double minDist = d;
        for (int i = 0; i < strip.size(); i++) {
            for (int j = i + 1; j < strip.size() && (strip[j].y - strip[i].y) < minDist; j++) {
                double dist = distance(strip[i], strip[j]);
                if (dist < minDist) {
                    minDist = dist;
                }
            }
        }
        return minDist;
    }
    
    static double closestUtil(vector<Point>& pointsX, int left, int right, int& recursiveCalls, int& stripChecks) {
        recursiveCalls++;
        
        int n = right - left + 1;
        if (n <= 3) {
            return bruteForceClosest(pointsX, left, right);
        }
        
        int mid = left + (right - left) / 2;
        double midX = pointsX[mid].x;
        
        double dLeft = closestUtil(pointsX, left, mid, recursiveCalls, stripChecks);
        double dRight = closestUtil(pointsX, mid + 1, right, recursiveCalls, stripChecks);
        double d = min(dLeft, dRight);
        
        // Полоса
        vector<Point> strip;
        for (int i = left; i <= right; i++) {
            if (abs(pointsX[i].x - midX) < d) {
                strip.push_back(pointsX[i]);
            }
        }
        
        sort(strip.begin(), strip.end(), [](Point a, Point b) { return a.y < b.y; });
        stripChecks += strip.size();
        
        return min(d, stripClosest(strip, d));
    }

public:
    static void analyzePerformance(int n) {
        // Генерируем случайные точки
        vector<Point> points;
        random_device rd;
        mt19937 gen(rd());
        uniform_real_distribution<> dis(0.0, 1000.0);
        
        for (int i = 0; i < n; i++) {
            points.push_back({dis(gen), dis(gen)});
        }
        
        // Сортируем по x
        vector<Point> pointsX = points;
        sort(pointsX.begin(), pointsX.end(), [](Point a, Point b) { return a.x < b.x; });
        
        int recursiveCalls = 0;
        int stripChecks = 0;
        
        auto start = chrono::high_resolution_clock::now();
        double minDist = closestUtil(pointsX, 0, n - 1, recursiveCalls, stripChecks);
        auto end = chrono::high_resolution_clock::now();
        
        auto duration = chrono::duration_cast<chrono::microseconds>(end - start);
        
        cout << "n = " << n << endl;
        cout << "  Минимальное расстояние: " << minDist << endl;
        cout << "  Рекурсивные вызовы: " << recursiveCalls << endl;
        cout << "  Точки в полосах: " << stripChecks << endl;
        cout << "  Время: " << duration.count() << " мкс" << endl;
        cout << "  O(n log n): " << n * log2(n) << endl;
        cout << "  O(n²): " << n * n << endl;
        cout << endl;
    }
};

int main(){
    ClosestPairAnalyzer::analyzePerformance(100000);
}