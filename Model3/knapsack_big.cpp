#include <iostream>
#include <fstream>
#include <vector>
#include <algorithm>
#include <unordered_map>

using namespace std;

struct Item {
    int value;
    int weight;
};

// Optimized DP using map to store only reachable states
long long knapsackOptimized(int capacity, const vector<Item>& items) {
    // Use a map to store weight->value pairs for reachable states
    unordered_map<int, long long> dp;
    dp[0] = 0;
    
    for (const auto& item : items) {
        // Create a temporary copy to avoid modifying while iterating
        auto temp = dp;
        
        for (const auto& state : temp) {
            int newWeight = state.first + item.weight;
            if (newWeight <= capacity) {
                long long newValue = state.second + item.value;
                
                // Update if this gives better value for the same or lower weight
                if (newValue > dp[newWeight]) {
                    dp[newWeight] = newValue;
                }
            }
        }
        
        // Optional: Clean up dominated states to save memory
        vector<int> toRemove;
        for (auto it1 = dp.begin(); it1 != dp.end(); ++it1) {
            for (auto it2 = dp.begin(); it2 != dp.end(); ) {
                if (it1 != it2 && it1->first <= it2->first && it1->second >= it2->second) {
                    it2 = dp.erase(it2);
                } else {
                    ++it2;
                }
            }
        }
    }
    
    // Find maximum value
    long long maxValue = 0;
    for (const auto& state : dp) {
        maxValue = max(maxValue, state.second);
    }
    
    return maxValue;
}

int main() {
    ifstream file("knapsack_big.txt");
    if (!file.is_open()) {
        cerr << "Error opening file" << endl;
        return 1;
    }
    
    int capacity, n;
    file >> capacity >> n;
    
    vector<Item> items(n);
    for (int i = 0; i < n; i++) {
        file >> items[i].value >> items[i].weight;
    }
    
    file.close();
    
    long long result = knapsackOptimized(capacity, items);
    cout << result << endl;
    
    return 0;
}