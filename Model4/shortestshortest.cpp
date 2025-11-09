#include <iostream>
#include <fstream>
#include <vector>
#include <queue>
#include <limits>
#include <string>

using namespace std;

const long long INF = numeric_limits<long long>::max() / 2;

struct Edge {
    int from, to;
    long long cost;
};

struct GraphEdge {
    int to;
    long long cost;
};

// ==================== BELLMAN-FORD ====================
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

// ==================== FLOYD-WARSHALL ====================
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

// ==================== JOHNSON'S ALGORITHM ====================
bool bellmanFordForJohnson(vector<Edge> edges, int n, vector<long long>& h) {
    h.assign(n + 2, 0);
    
    // Add edges from source (vertex n+1) to all vertices with cost 0
    for (int i = 1; i <= n; i++) {
        edges.push_back({n + 1, i, 0});
    }
    
    // Relax edges n times
    for (int i = 0; i <= n; i++) {
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

long long dijkstraReweighted(vector<vector<GraphEdge>>& graph, int start, int n, vector<long long>& h) {
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
    
    return dist[start]; // Return distance to start (will be used for all pairs)
}

pair<bool, long long> johnson(vector<Edge> edges, int n) {
    vector<long long> h;
    
    // Step 1: Run Bellman-Ford 
    if (!bellmanFordForJohnson(edges, n, h)) {
        return {false, 0}; // Negative cycle detected
    }
    
    // Step 2: Reweight edges
    vector<vector<GraphEdge>> graph(n + 1);
    for (auto& edge : edges) {
        if (edge.from <= n && edge.to <= n) {
            long long newCost = edge.cost + h[edge.from] - h[edge.to];
            graph[edge.from].push_back({edge.to, newCost});
        }
    }
    
    // Step 3: Run Dijkstra from each vertex
    long long globalMin = INF;
    
    for (int u = 1; u <= n; u++) {
        vector<long long> dist(n + 1, INF);
        priority_queue<pair<long long, int>, vector<pair<long long, int>>, greater<>> pq;
        
        dist[u] = 0;
        pq.push({0, u});
        
        while (!pq.empty()) {
            auto [d, v] = pq.top();
            pq.pop();
            
            if (d > dist[v]) continue;
            
            for (auto& edge : graph[v]) {
                if (dist[v] != INF && dist[v] + edge.cost < dist[edge.to]) {
                    dist[edge.to] = dist[v] + edge.cost;
                    pq.push({dist[edge.to], edge.to});
                }
            }
        }
        
        // Convert back to original distances and find minimum
        for (int v = 1; v <= n; v++) {
            if (u != v && dist[v] != INF) {
                long long originalDist = dist[v] - h[u] + h[v];
                globalMin = min(globalMin, originalDist);
            }
        }
    }
    
    return {true, globalMin};
}

// ==================== MAIN PROCESSING ====================
pair<bool, long long> processGraph(const string& filename) {
    ifstream file(filename);
    if (!file.is_open()) {
        cerr << "Error: Cannot open " << filename << endl;
        return {false, 0};
    }
    
    int n, m;
    file >> n >> m;
    
    vector<Edge> edges;
    vector<vector<long long>> distMatrix(n + 1, vector<long long>(n + 1, INF));

    for (int i = 1; i <= n; i++) {
        distMatrix[i][i] = 0;
    }
 
    for (int i = 0; i < m; i++) {
        int u, v;
        long long cost;
        file >> u >> v >> cost;
        edges.push_back({u, v, cost});
        distMatrix[u][v] = min(distMatrix[u][v], cost);
    }
    
    file.close();
    
    cout << "\nProcessing " << filename << "..." << endl;
    cout << "Vertices: " << n << ", Edges: " << m << endl;
    
    // Method 1: Bellman-Ford check for negative cycle
    cout << "Method 1 (Bellman-Ford): ";
    bool hasNoCycle = !hasNegativeCycle(edges, n);
    if (!hasNoCycle) {
        cout << "Negative cycle detected!" << endl;
        return {false, 0};
    }
    cout << "No negative cycle" << endl;
    
    // Method 2: Floyd-Warshall
    cout << "Method 2 (Floyd-Warshall): ";
    auto [noNegCycleFW, minPathFW] = floydWarshall(n, distMatrix);
    if (!noNegCycleFW) {
        cout << "Negative cycle detected!" << endl;
        return {false, 0};
    }
    cout << "Shortest shortest path = " << minPathFW << endl;

    // Method 3: Johnson's Algorithm
    cout << "Method 3 (Johnson's): ";
    auto [noNegCycleJ, minPathJ] = johnson(edges, n);
    if (!noNegCycleJ) {
        cout << "Negative cycle detected!" << endl;
        return {false, 0};
    }
    cout << "Shortest shortest path = " << minPathJ << endl;
    
    return {true, min(minPathFW, minPathJ)};
}

int main() {
    vector<string> files = {"g1.txt", "g2.txt", "g3.txt"};
    vector<pair<bool, long long>> results;
    
    cout << "========================================" << endl;
    cout << "ALL-PAIRS SHORTEST PATH ANALYZER" << endl;
    cout << "========================================" << endl;
    
    for (const string& file : files) {
        auto result = processGraph(file);
        results.push_back(result);
    }
    
    cout << "\n========================================" << endl;
    cout << "FINAL RESULTS" << endl;
    cout << "========================================" << endl;
    
    int graphsWithoutNegCycle = 0;
    long long globalMin = INF;
    
    for (size_t i = 0; i < results.size(); i++) {
        cout << "Graph " << (i + 1) << ": ";
        if (results[i].first) {
            cout << "Shortest shortest path = " << results[i].second << endl;
            graphsWithoutNegCycle++;
            globalMin = min(globalMin, results[i].second);
        } else {
            cout << "Has negative cycle" << endl;
        }
    }
    
    cout << "\n========================================" << endl;
    if (graphsWithoutNegCycle == 0) {
        cout << "ANSWER: NULL (all graphs have negative cycles)" << endl;
    } else {
        cout << "ANSWER: " << globalMin << endl;
        cout << "(" << graphsWithoutNegCycle << " graph(s) without negative cycles)" << endl;
    }
    cout << "========================================" << endl;
    
    return 0;
}