#include <iostream>
#include <vector>
#include <queue>
#include <limits>

using namespace std;

const long long INF = numeric_limits<long long>::max();

struct Edge {
    int from, to;
    long long cost;
};

struct GraphEdge {
    int to;
    long long cost;
};

// Bellman-Ford to detect negative cycles and compute h values
bool bellmanFordForJohnson(vector<Edge>& edges, int n, vector<long long>& h) {
    h.assign(n + 2, 0); // n+1 vertices + 1 source vertex
    
    // Add edges from source (vertex n+1) to all vertices with cost 0
    for (int i = 1; i <= n; i++) {
        edges.push_back({n + 1, i, 0});
    }
    
    // Relax edges n times (including the extra vertex)
    for (int i = 0; i < n; i++) {
        for (auto& edge : edges) {
            if (h[edge.from] != INF && h[edge.from] + edge.cost < h[edge.to]) {
                h[edge.to] = h[edge.from] + edge.cost;
            }
        }
    }
    
    // Check for negative cycles
    for (auto& edge : edges) {
        if (h[edge.from] != INF && h[edge.from] + edge.cost < h[edge.to]) {
            return false;
        }
    }
    
    return true;
}

// Dijkstra with reweighted edges
long long dijkstraReweighted(vector<vector<GraphEdge>>& graph, int start, int n) {
    vector<long long> dist(n + 1, INF);
    priority_queue<pair<long long, int>, vector<pair<long long, int>>, greater<>> pq;
    
    dist[start] = 0;
    pq.push({0, start});
    
    while (!pq.empty()) {
        auto [d, u] = pq.top();
        pq.pop();
        
        if (d > dist[u]) continue;
        
        for (auto& edge : graph[u]) {
            if (dist[u] != INF && dist[u] + edge.cost < dist[edge.to]) {
                dist[edge.to] = dist[u] + edge.cost;
                pq.push({dist[edge.to], edge.to});
            }
        }
    }
    
    long long minDist = INF;
    for (int i = 1; i <= n; i++) {
        if (dist[i] != INF) {
            minDist = min(minDist, dist[i]);
        }
    }
    
    return minDist;
}

pair<bool, long long> johnson(vector<Edge>& edges, int n) {
    vector<long long> h;
    
    // Step 1: Run Bellman-Ford 
    if (!bellmanFordForJohnson(edges, n, h)) {
        return {false, 0}; // Negative cycle detected
    }
    
    // Step 2: Reweight edges
    vector<vector<GraphEdge>> graph(n + 1);
    for (auto& edge : edges) {
        if (edge.from <= n && edge.to <= n) { // Exclude edges from dummy vertex
            long long newCost = edge.cost + h[edge.from] - h[edge.to];
            graph[edge.from].push_back({edge.to, newCost});
        }
    }
    
    // Step 3: Run Dijkstra from each vertex
    long long globalMin = INF;
    for (int i = 1; i <= n; i++) {
        long long minDist = dijkstraReweighted(graph, i, n);
        if (minDist != INF) {
            // Revert to original weights
            for (int j = 1; j <= n; j++) {
                long long originalDist = minDist - h[i] + h[j];
                globalMin = min(globalMin, originalDist);
            }
        }
    }
    
    return {true, globalMin};
}