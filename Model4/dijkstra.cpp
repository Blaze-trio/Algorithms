#include <iostream>
#include <vector>
#include <queue>
#include <limits>
#include <fstream>

using namespace std;

const long long INF = numeric_limits<long long>::max();

struct Edge {
    int to;
    long long cost;
};

long long dijkstra(vector<vector<Edge>>& graph, int start, int end, int n) {
    vector<long long> dist(n + 1, INF);
    priority_queue<pair<long long, int>, vector<pair<long long, int>>, greater<pair<long long, int>>> pq;
    
    dist[start] = 0;
    pq.push({0, start});
    
    while (!pq.empty()) {
        auto [d, u] = pq.top();
        pq.pop();
        
        if (d > dist[u]) continue;
        
        for (auto& edge : graph[u]) {
            int v = edge.to;
            long long cost = edge.cost;
            
            if (dist[u] != INF && dist[u] + cost < dist[v]) {
                dist[v] = dist[u] + cost;
                pq.push({dist[v], v});
            }
        }
    }
    
    return dist[end];
}