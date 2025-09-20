#include <iostream>
#include <vector>
#include <fstream>
#include <climits>
#include <algorithm>
#include <string>

struct Edge {
    int to;
    int cost;
    
    Edge(int t, int c) : to(t), cost(c) {}
};

int main() {
    const char* filename = "edges.txt";
    
    std::ifstream file(filename);
    if (!file.is_open()) {
        std::cerr << "Error: Could not open file " << filename << std::endl;
        return 1;
    }
    
    int numNodes, numEdges;
    file >> numNodes >> numEdges;
    std::vector<std::vector<Edge>> graph(numNodes + 1); 
    
    for (int i = 0; i < numEdges; i++) {
        int u, v, cost;
        file >> u >> v >> cost;
        graph[u].emplace_back(v, cost);
        graph[v].emplace_back(u, cost); 
    }
    
    file.close();

    std::vector<bool> inMST(numNodes + 1, false); 
    std::vector<int> minCost(numNodes + 1, INT_MAX);  
    
    int startVertex = 1;
    minCost[startVertex] = 0;
    
    long long totalMSTCost = 0;
    int verticesAdded = 0;
    
    std::cout << "Building MST using Prim's algorithm..." << std::endl;
    
    while (verticesAdded < numNodes) {
        int minVertex = -1;
        int minEdgeCost = INT_MAX;
        for (int v = 1; v <= numNodes; v++) {
            if (!inMST[v] && minCost[v] < minEdgeCost) {
                minEdgeCost = minCost[v];
                minVertex = v;
            }
        }
        inMST[minVertex] = true;
        totalMSTCost += minEdgeCost;
        verticesAdded++;
        std::cout << "Added vertex " << minVertex << " with cost " << minEdgeCost 
                  << " (Total so far: " << totalMSTCost << ")" << std::endl;
        for (const Edge& edge : graph[minVertex]) {
            int neighbor = edge.to;
            int edgeCost = edge.cost;
            
            if (!inMST[neighbor] && edgeCost < minCost[neighbor]) {
                minCost[neighbor] = edgeCost;
            }
        }
    }
    
    std::cout << "\nMinimum Spanning Tree Cost: " << totalMSTCost << std::endl;
    
    return 0;
}