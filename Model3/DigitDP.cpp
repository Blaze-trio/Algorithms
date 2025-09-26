#include <iostream>
#include <string>
#include <cstring>
#include <algorithm>
using namespace std;

const int MOD = 1e9 + 7;
int memo[20][10][2][2];

int dp(int pos, int lastDigit, bool tight, bool started, const string& num) {
    if (pos == num.size()) {
        return started ? 1 : 0;
    }
    
    if (memo[pos][lastDigit][tight][started] != -1) {
        return memo[pos][lastDigit][tight][started];
    }
    
    int limit = tight ? (num[pos] - '0') : 9;
    int result = 0;
    
    for (int digit = 0; digit <= limit; digit++) {
        bool newTight = tight && (digit == limit);
        bool newStarted = started || (digit > 0);
        if (started && digit == lastDigit) {
            continue;
        }
        
        result += dp(pos + 1, digit, newTight, newStarted, num);
    }
    
    return memo[pos][lastDigit][tight][started] = result;
}

int countValidNumbers(long long x) {
    if (x <= 0) return 0;
    
    string num = to_string(x);
    memset(memo, -1, sizeof(memo));
    return dp(0, 0, true, false, num);
}

void solve() {
    long long a, b;
    cin >> a >> b;
    
    int right = countValidNumbers(b);
    int left = countValidNumbers(a - 1);
    
    cout << right - left << endl;
}

signed main() {
    ios_base::sync_with_stdio(false);
    cin.tie(NULL);
    
    solve();
    return 0;
}