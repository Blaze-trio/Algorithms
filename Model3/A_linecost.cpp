// CPP program to implement sequence alignment
// problem.
#include <bits/stdc++.h>

using namespace std;

// ======== Debug Function to Print 2D DP Matrix ========
template <size_t R, size_t C>
void printDPMatrix(int (&dp)[R][C], int rows, int cols, const string& x, const string& y) {
    cout << "\nDP Matrix (" << rows << "x" << cols << "):\n\n";
    cout << "     ";
    for (int j = 0; j < cols - 1; ++j) {
        if (j == 0) cout << "   ";
        else cout << setw(4) << y[j - 1];
    }
    cout << "\n";

    for (int i = 0; i < rows; ++i) {
        if (i == 0) cout << "   ";
        else cout << setw(2) << x[i - 1] << " ";
        for (int j = 0; j < cols; ++j) {
            cout << setw(4) << dp[i][j];
        }
        cout << '\n';
    }
    cout << '\n';
}

// ======== Core Function to Find Minimum Penalty ========

// function to find out the minimum penalty
void getMinimumPenalty(string x, string y, int missC, int matchC, int gapC)
{
    int i, j; // initialising variables
    
    int m = x.length(); // length of gene1
    int n = y.length(); // length of gene2
    
    // table for storing optimal substructure answers
    int dp[n+m+1][n+m+1] = {0};
    //int gaps[n+m+1][n+m+1] = {0};
    int gapi = 0;
    int gapj = 0;

    // initialising the table 
    for (i = 0; i <= (n+m); i++)
    {
        dp[i][0] = i * i * gapC;
        dp[0][i] = i * i * gapC;
    }    

    // calculating the minimum penalty
    for (i = 1; i <= m; i++)
    {
        for (j = 1; j <= n; j++)
        {
            if (x[i - 1] == y[j - 1])
            {
                int curr = dp[i - 1][j - 1] + matchC;
                int pathi = dp[i - 1][j] + (gapi + 1) * (gapi + 1) * gapC - max(gapi, 0) * max(gapi, 0) * gapC;
                int pathj = dp[i][j - 1] + (gapj + 1) * (gapj + 1) * gapC - max(gapj, 0) * max(gapj, 0) * gapC;
                bool gapiTaken = pathi > curr;
                bool gapjTaken = pathj > curr;
                if(gapiTaken && pathi > pathj){
                    gapi++;
                    gapj = 0;
                    curr = pathi;
                }else if(gapjTaken){
                    gapj++;
                    gapi = 0;
                    curr = pathj;
                }else{
                    gapi = 0;
                    gapj = 0;
                }
                dp[i][j] = curr;
            }
            else
            {
                int curr = dp[i - 1][j - 1] + missC;
                int pathi = dp[i - 1][j] + (gapi + 1) * (gapi + 1) * gapC - max(gapi, 0) * max(gapi, 0) * gapC;
                int pathj = dp[i][j - 1] + (gapj + 1) * (gapj + 1) * gapC - max(gapj, 0) * max(gapj, 0) * gapC;
                bool gapiTaken = pathi > curr;
                bool gapjTaken = pathj > curr;
                if(gapiTaken && pathi > pathj){
                    gapi++;
                    gapj = 0;
                    curr = pathi;
                }else if(gapjTaken){
                    gapj++;
                    gapi = 0;
                    curr = pathj;
                }else{
                    gapi = 0;
                    gapj = 0;
                }
                dp[i][j] = curr;
            }
        }
    }
    // Print column indices
    cout << "     ";
    for (int j = 0; j < n+m+1; ++j)
        cout << setw(5) << j;
    cout << "\n";

    // Print separator
    cout << "     ";
    for (int j = 0; j < n+m+1; ++j)
        cout << "-----";
    cout << "\n";

    // Print each row with row index
    for (int i = 0; i < n+m+1; ++i) {
        cout << setw(3) << i << " |";
        for (int j = 0; j < n+m+1; ++j)
            cout << setw(5) << dp[i][j];
        cout << "\n";
    }

    cout << "=======================================\n";
    // Printing the final answer
    cout << "Minimum Penalty in aligning the genes = ";
    cout << dp[m][n] << "\n";
    return;
}

// Driver code
int main(){
    // input strings
    string gene1 = "C";
    string gene2 = "TCGCGTGG";
    // c            -C--         
    
    // initialising penalties of different types
    int misMatchCost = -3;
    int matchCost = 1;
    int gapCost = -19;
    

    // calling the function to calculate the result
    getMinimumPenalty(gene1, gene2, 
        misMatchCost, matchCost, gapCost);
    return 0;
}