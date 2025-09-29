#include <iostream>
#include <fstream>
#include <vector>
#include <algorithm>

using namespace std;

long long knapsack(int capacity, const vector<pair<int, int>>& items) {
    int n = items.size();
    vector<long long> dp(capacity + 1, 0);
    
    for (int i = 0; i < n; i++) {
        int value = items[i].first;
        int weight = items[i].second;
        
        for (int w = capacity; w >= weight; w--) {
            dp[w] = max(dp[w], dp[w - weight] + value);
        }
    }
    
    return dp[capacity];
}

int main() {
    ifstream file("knapsack1.txt");
    if (!file.is_open()) {
        cerr << "Error opening file" << endl;
        return 1;
    }
    
    int capacity, n;
    file >> capacity >> n;
    
    vector<pair<int, int>> items;
    for (int i = 0; i < n; i++) {
        int value, weight;
        file >> value >> weight;
        items.push_back({value, weight});
    }
    
    file.close();
    
    long long result = knapsack(capacity, items);
    cout << result << endl;
    
    return 0;
}