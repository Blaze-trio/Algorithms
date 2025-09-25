#include <iostream>
#include <fstream>
#include <vector>
#include <algorithm>
#include <cmath>

using namespace std;

struct Edge {
    int u, v, cost;
    
    bool operator<(const Edge& other) const {
        return cost < other.cost;
    }
};

class UnionFind {
private:
    vector<int> parent, rank;
    int numComponents;

public:
    UnionFind(int n) {
        parent.resize(n + 1);
        rank.resize(n + 1, 0);
        numComponents = n;
        
        for (int i = 1; i <= n; i++) {
            parent[i] = i;
        }
    }
    
    int find(int x) {
        if (parent[x] != x) {
            parent[x] = find(parent[x]);
        }
        return parent[x];
    }
    
    bool unite(int x, int y) {
        int rootX = find(x);
        int rootY = find(y);
        if (rootX == rootY) return false;
        
        if (rank[rootX] < rank[rootY]) {
            parent[rootX] = rootY;
        } else if (rank[rootX] > rank[rootY]) {
            parent[rootY] = rootX;
        } else {
            parent[rootY] = rootX;
            rank[rootX]++;
        }
        
        numComponents--;
        return true;
    }
    
    int getNumComponents() {
        return numComponents;
    }
};

int main() {
    string filename = "clustering1.txt";
    ifstream file(filename);
    
    if (!file.is_open()) {
        cerr << "Error opening file!" << endl;
        return 1;
    }
    
    int n;
    file >> n;
    
    vector<Edge> edges;
    int u, v, cost;
    
    while (file >> u >> v >> cost) {
        edges.push_back({u, v, cost});
    }
    
    file.close();
    
    sort(edges.begin(), edges.end());
    
    UnionFind uf(n);
    int targetClusters = 4;
    
    for (const Edge& edge : edges) {
        if (uf.getNumComponents() == targetClusters) {
            break;
        }
        uf.unite(edge.u, edge.v);
    }
    
    int maxSpacing = INT_MAX;
    for (const Edge& edge : edges) {
        if (uf.find(edge.u) != uf.find(edge.v)) {
            maxSpacing = min(maxSpacing, edge.cost);
        }
    }
    cout << "Maximum spacing of a 4-clustering: " << maxSpacing << endl;
    return 0;
}