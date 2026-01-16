#include <iostream>
#include <vector>
#include <limits>
#include <algorithm>
#include <cmath> 
#include <utility> 
#include <string>
#include <bits/stdc++.h>

using namespace std;
//==================================================j===========================//
int smallest_prime_factor(int n) {
    for (int i = 2; i <= sqrt(n); i++) {
        if (n % i == 0) {
            return i;
        }
    }
    return n;
}
int binary_search_largest_smaller(vector<int>& b, int x) {
    int l = 0, r = b.size() - 1;
    int ans = -1;
    while (l <= r) {
        int m = (l + r) / 2;
        if (b[m] != -1 && b[m] <= x) { 
            ans = m; 
            l = m + 1;
        } else if (b[m] == -1) {
            l = m + 1; 
        } else {
            r = m - 1;
        }
    }
    return ans;
}
void solve(int n, vector<int> b) {
    int res = 0;
    for (int i = 0; i < b.size(); i++) {
        b[i] = smallest_prime_factor(b[i]);
    }
    for(int i = 0; i < b.size(); i++) {
        sort(b.begin(), b.end());
        int j = binary_search_largest_smaller(b, n);
        if(j == -1 || b[j] == -1){
            break;
        }
        res++;
        n += b[j];
        b[j] = -1;
    }
    printf("%d %d\n", res, n);
}// int main() {
//     int t;
//     std::cin >> t;
//     while(t--) {
//         int n, size;
//         std::cin >> size >> n;
//         vector<int> b(size);
//         for (int j = 0; j < size; j++) {
//             std::cin >> b[j];
//         }
//         solve(n, b);
//     }
//     return 0;
// }
//==================================================c==========================//
int Cut(string n, string m){
    int i = n.size() - 1;
    int j = m.size() - 1;
    while(i >= 0 && j >= 0){
        if(n[i] != m[j]){
            break;
        }else{
            i--;
            j--;
        }
    }
    return i + 1 + j + 1;
}
// int main(){
//     string n, m;
//     std::cin >> n;
//     std::cin >> m;
//     printf("%d\n", Cut(n, m));
//     return 0;
// }
//==================================================f==========================//
int Equal(int k, int n){
    return (k / n) * n;
}
int main(){
    int k, n;
    std::cin >> k >> n;
    printf("%d\n", Equal(k, n));
    return 0;
}

