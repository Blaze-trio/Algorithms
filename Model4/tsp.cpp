#include <fstream> 
#include <iostream>
#include <vector>
#include <cmath>
#include <iomanip>
#include <algorithm>

using namespace std;

// Use a large value for infinity
const double INF = 1e18;

struct City {
    double x, y;
};

// Euclidean distance calculation
double get_dist(const City& a, const City& b) {
    return sqrt(pow(a.x - b.x, 2) + pow(a.y - b.y, 2));
}

int main() {
    ifstream infile("tsp.txt"); 
    int n;
    if (!(infile >> n)) return 0;

    vector<City> cities(n);
    for (int i = 0; i < n; ++i) {
        infile >> cities[i].x >> cities[i].y;
    }

    // Precompute distance matrix
    vector<vector<double>> dist(n, vector<double>(n));
    for (int i = 0; i < n; ++i) {
        for (int j = 0; j < n; ++j) {
            dist[i][j] = get_dist(cities[i], cities[j]);
        }
    }

    // DP table: dp[mask][last_city]
    // size is (1 << n) rows and n columns
    vector<vector<double>> dp(1 << n, vector<double>(n, INF));

    // Base case: Start at the first city (index 0)
    dp[1][0] = 0;

    // Iterate through all possible subsets of cities
    for (int mask = 1; mask < (1 << n); ++mask) {
        for (int i = 0; i < n; ++i) {
            // If city i is in the current subset
            if (mask & (1 << i)) {
                if (dp[mask][i] == INF) continue;

                // Try to go to the next city j
                for (int j = 0; j < n; ++j) {
                    // If city j is NOT in the subset
                    if (!(mask & (1 << j))) {
                        int next_mask = mask | (1 << j);
                        dp[next_mask][j] = min(dp[next_mask][j], dp[mask][i] + dist[i][j]);
                    }
                }
            }
        }
    }

    // Find the minimum cost to return to the starting city (0)
    double min_cost = INF;
    int full_mask = (1 << n) - 1;
    for (int i = 1; i < n; ++i) {
        min_cost = min(min_cost, dp[full_mask][i] + dist[i][0]);
    }

    // Output the result rounded down to the nearest integer
    cout << "Minimum TSP Cost: " << (long long)floor(min_cost) << endl;

    return 0;
}