#include <iostream>
#include <vector>
#include <limits>

using namespace std;

const long long INF = numeric_limits<long long>::max();

struct Edge {
    int from, to;
    long long cost;
};

bool bellmanFord(vector<Edge>& edges, int n, int start, vector<long long>& dist) {
    dist.assign(n + 1, INF);
    dist[start] = 0;
    
    for (int i = 0; i < n - 1; i++) {
        for (auto& edge : edges) {
            if (dist[edge.from] != INF && 
                dist[edge.from] + edge.cost < dist[edge.to]) {
                dist[edge.to] = dist[edge.from] + edge.cost;
            }
        }
    }
    
    // Check for negative cycles
    for (auto& edge : edges) {
        if (dist[edge.from] != INF && 
            dist[edge.from] + edge.cost < dist[edge.to]) {
            return false; // Negative cycle detected
        }
    }
    
    return true; 
}

bool hasNegativeCycle(vector<Edge>& edges, int n) {
    vector<long long> dist;
    return !bellmanFord(edges, n, 1, dist);
}