#include <iostream>
#include <string>
using namespace std;

int main() {
    int t;
    cin >> t;
    while (t--) {
        int n;
        cin >> n;
        int low = 0, high = n - 2;
        string response;
        while (low < high) {
            int mid = (low + high + 1) / 2;
            cout << "READ " << mid << endl;
            cout.flush();
            cin >> response;
            if (response == "true") {
                low = mid;
            } else {
                high = mid - 1;
            }
        }
        cout << "OUTPUT " << low << endl;
        cout.flush();
    }
    return 0;
}