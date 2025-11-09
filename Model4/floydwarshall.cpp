#include <iostream>
#include <vector>
#include <limits>

using namespace std;

const long long INF = numeric_limits<long long>::max() / 2;

pair<bool, long long> floydWarshall(int n, vector<vector<long long>>& dist) {
    // Floyd-Warshall algorithm
    for (int k = 1; k <= n; k++) {
        for (int i = 1; i <= n; i++) {
            for (int j = 1; j <= n; j++) {
                if (dist[i][k] != INF && dist[k][j] != INF) {
                    dist[i][j] = min(dist[i][j], dist[i][k] + dist[k][j]);
                }
            }
        }
    }
    
    // Check for negative cycle
    for (int i = 1; i <= n; i++) {
        if (dist[i][i] < 0) {
            return {false, 0}; // Negative cycle exists
        }
    }
    
    // Find minimum shortest path
    long long minPath = INF;
    for (int i = 1; i <= n; i++) {
        for (int j = 1; j <= n; j++) {
            if (i != j && dist[i][j] != INF) {
                minPath = min(minPath, dist[i][j]);
            }
        }
    }
    
    return {true, minPath};
}