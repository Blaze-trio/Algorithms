#include <iostream>
#include <fstream>
#include <queue>
#include <vector>
#include <functional>

using namespace std;

int main() {
    ifstream file("Median.txt", ios::in);
    if (!file.is_open()) {
        cerr << "Error: Unable to open file 'Median.txt'." << endl;
        return 1;
    }

    priority_queue<int> low;
    priority_queue<int, vector<int>, greater<int>> high;
    int x;
    long long sum = 0;

    while (file >> x) {
        if (low.empty() || x <= low.top()) {
            low.push(x);
        } else {
            high.push(x);
        }

        if (low.size() > high.size() + 1) {
            high.push(low.top());
            low.pop();
        } else if (high.size() > low.size()) {
            low.push(high.top());
            high.pop();
        }

        sum += low.top();
    }

    file.close();
    cout << sum % 10000 << endl;

    return 0;
}