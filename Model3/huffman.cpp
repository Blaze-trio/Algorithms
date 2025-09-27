#include <iostream>
#include <fstream>
#include <vector>
#include <queue>
#include <algorithm>

using namespace std;

struct Node {
    long long weight;
    int max_depth;
    int min_depth;
    
    Node(long long w, int max_d, int min_d) : weight(w), max_depth(max_d), min_depth(min_d) {}
    bool operator>(const Node& other) const {
        return weight > other.weight;
    }
};

int main(){
    ifstream file("huffman.txt");
    if(!file.is_open()){
        cerr << "Error opening file." << endl;
        return 1;
    }

    int n;
    file >> n;

    priority_queue<Node, vector<Node>, greater<Node>> pq;

    for (int i = 0; i < n; i++) {
        long long w;
        file >> w;
        pq.push(Node(w, 0, 0));
    }

    while (pq.size() > 1) {
        Node node1 = pq.top();
        pq.pop();
        Node node2 = pq.top();
        pq.pop();
        long long new_weight = node1.weight + node2.weight;
        int new_max_depth = max(node1.max_depth, node2.max_depth) + 1;
        int new_min_depth = min(node1.min_depth, node2.min_depth) + 1;
        pq.push(Node(new_weight, new_max_depth, new_min_depth));
    }

    Node result = pq.top();
    cout << "Maximum codeword length: " << result.max_depth << endl;
    cout << "Minimum codeword length: " << result.min_depth << endl;
    return 0;
}
