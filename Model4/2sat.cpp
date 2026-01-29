#include <iostream>
#include <vector>
#include <fstream>
#include <algorithm>
#include <stack>

using namespace std;

struct Tarjan2SAT {
    int n; // number of variables
    vector<vector<int>> adj;
    vector<int> dfn, low, scc;
    vector<bool> in_stack;
    stack<int> st;
    int timer, scc_cnt;

    Tarjan2SAT(int variables) {
        n = variables;
        adj.resize(2 * n + 1);
        dfn.assign(2 * n + 1, -1);
        low.assign(2 * n + 1, -1);
        scc.assign(2 * n + 1, -1);
        in_stack.assign(2 * n + 1, false);
        timer = scc_cnt = 0;
    }

    // Maps variable x to 2x, and -x to 2x+1
    int get_node(int x) {
        if (x > 0) return 2 * x;
        return 2 * abs(x) + 1;
    }

    int neg(int node) {
        return node ^ 1;
    }

    void add_edge(int u, int v) {
        adj[u].push_back(v);
    }

    void add_clause(int u_raw, int v_raw) {
        int u = get_node(u_raw);
        int v = get_node(v_raw);
        // (u or v) is equivalent to: (!u -> v) and (!v -> u)
        add_edge(neg(u), v);
        add_edge(neg(v), u);
    }

    void dfs(int u) {
        dfn[u] = low[u] = ++timer;
        st.push(u);
        in_stack[u] = true;

        for (int v : adj[u]) {
            if (dfn[v] == -1) {
                dfs(v);
                low[u] = min(low[u], low[v]);
            } else if (in_stack[v]) {
                low[u] = min(low[u], dfn[v]);
            }
        }

        if (low[u] == dfn[u]) {
            scc_cnt++;
            while (true) {
                int node = st.top();
                st.pop();
                in_stack[node] = false;
                scc[node] = scc_cnt;
                if (node == u) break;
            }
        }
    }

    bool is_satisfiable() {
        for (int i = 2; i <= 2 * n + 1; ++i) {
            if (dfn[i] == -1) dfs(i);
        }
        for (int i = 1; i <= n; ++i) {
            if (scc[2 * i] == scc[2 * i + 1]) return false;
        }
        return true;
    }
};

void solve_file(string filename) {
    ifstream file(filename);
    if (!file.is_open()) {
        cout << "Could not open " << filename << endl;
        return;
    }

    int num_vars;
    file >> num_vars;
    Tarjan2SAT solver(num_vars);

    int u, v;
    while (file >> u >> v) {
        solver.add_clause(u, v);
    }

    if (solver.is_satisfiable()) cout << "1";
    else cout << "0";
    
    file.close();
}

int main() {
    cout << "Result: ";
    for (int i = 1; i <= 6; ++i) {
        string filename = "2sat" + to_string(i) + ".txt";
        solve_file(filename);
    }
    cout << endl;
    return 0;
}