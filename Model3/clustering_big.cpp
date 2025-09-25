#include <iostream>
#include <fstream>
#include <vector>
#include <unordered_map>
#include <bitset>

using namespace std;

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
    string filename = "clustering_big.txt";
    ifstream file(filename);
    
    if (!file.is_open()) {
        cerr << "Error opening file!" << endl;
        return 1;
    }
    
    int n, bits;
    file >> n >> bits;
    
    vector<int> nodes(n);
    unordered_map<int, int> nodeMap;
    
    for (int i = 0; i < n; i++) {
        int pattern = 0;
        for (int j = 0; j < bits; j++) {
            int bit;
            file >> bit;
            pattern = (pattern << 1) | bit;
        }
        nodes[i] = pattern;
        nodeMap[pattern] = i;
    }
    
    file.close();
    
    UnionFind uf(n);

    for (int i = 0; i < n; i++) {
        int pattern = nodes[i];
        if (nodeMap.find(pattern) != nodeMap.end()) {
            uf.unite(i, nodeMap[pattern]);
        }
        for (int j = 0; j < bits; j++) {
            int neighborPattern = pattern ^ (1 << j);
            if (nodeMap.find(neighborPattern) != nodeMap.end()) {
                uf.unite(i, nodeMap[neighborPattern]);
            }
        }
        for (int j = 0; j < bits; j++) {
            for (int k = j + 1; k < bits; k++) {
                int neighborPattern = pattern ^ (1 << j) ^ (1 << k);
                if (nodeMap.find(neighborPattern) != nodeMap.end()) {
                    uf.unite(i, nodeMap[neighborPattern]);
                }
            }
        }
    }
    
    cout << "Largest k with spacing at least 3: " << uf.getNumComponents() << endl;
    
    return 0;
}