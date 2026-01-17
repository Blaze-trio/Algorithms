#include <iostream>
#include <vector>
#include <cmath>
#include <fstream>
#include <limits>
#include <iomanip>

using namespace std;

struct City {
    double x, y;
};

inline double get_sq_dist(const City& a, const City& b) {
    double dx = a.x - b.x;
    double dy = a.y - b.y;
    return (dx * dx) + (dy * dy);
}

int main() {
    ifstream infile("nn.txt");
    if (!infile) {
        cerr << "Error: nn.txt not found." << endl;
        return 1;
    }

    int num_cities;
    infile >> num_cities;

    vector<City> cities(num_cities);
    for (int i = 0; i < num_cities; ++i) {
        int temp_idx;
        infile >> temp_idx >> cities[i].x >> cities[i].y;
    }
    infile.close();

    vector<bool> visited(num_cities, false);
    double total_dist = 0.0;
    int current = 0;
    visited[0] = true;
    int visited_count = 1;

    while (visited_count < num_cities) {
        int best_idx = -1;
        double min_sq = numeric_limits<double>::max();

        for (int i = 0; i < num_cities; ++i) {
            if (!visited[i]) {
                double d2 = get_sq_dist(cities[current], cities[i]);
            
                if (d2 < min_sq) {
                    min_sq = d2;
                    best_idx = i;
                }
            }
        }

        total_dist += sqrt(min_sq);
        visited[best_idx] = true;
        current = best_idx;
        visited_count++;

        if (visited_count % 5000 == 0) 
            cout << "Progress: " << visited_count << " cities visited..." << endl;
    }

    total_dist += sqrt(get_sq_dist(cities[current], cities[0]));

    cout << "Final Euclidean Distance: " << fixed << setprecision(4) << total_dist << endl;
    cout << "Result (Rounded Down): " << (long long)floor(total_dist) << endl;

    return 0;
}