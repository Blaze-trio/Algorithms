#include <iostream>
#include <fstream>
#include <vector>
#include <algorithm>

using namespace std;

int main(){
    ifstream file("mwis.txt");

    if(!file.is_open()){
        cerr << "Error opening file." << endl;
        return 1;
    }
    
    int n;
    file >> n;

    vector<long long> weights(n + 1);

    for(int i = 1; i <= n; i++){
        file >> weights[i];
    }

    vector<long long> dp(n + 1);
    dp[0] = 0;
    dp[1] = weights[1];
    for(int i = 2; i <= n; i++){
        dp[i] = max(dp[i - 1], dp[i - 2] + weights[i]);
    }

    vector<bool> inSet(n + 1, false);
    int i = n;
    while (i >= 1) {
        if (i == 1) {
            inSet[i] = true;
            break;
        }
        if (dp[i] == dp[i - 1]) {
            i--;
        } else {
            inSet[i] = true;
            i -= 2;
        }
    }

    vector<int> checkVertices = {1, 2, 3, 4, 17, 117, 517, 997};
    string result;
    for (int v : checkVertices) {
        if (inSet[v]) {
            result += '1';
        } else {
            result += '0';
        }
    }
    cout << result << endl;
    
    return 0;
}