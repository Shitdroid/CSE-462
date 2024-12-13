#include <iostream>
#include <vector>
#include <queue>

using namespace std;

int main() {
    int n, m;
    cin >> n >> m;

    vector<vector<int>> adj(n);
    vector<int> degree(n, 0);

    for (int i = 0; i < m; ++i) {
        int u, v;
        cin >> u >> v;
        adj[u].push_back(v);
        adj[v].push_back(u);
        degree[u]++;
        degree[v]++;
    }

    int leafCount = 0;
    for (int i = 0; i < n; ++i) {
        if (degree[i] == 1) {
            leafCount++;
        }
    }

    cout << leafCount << endl;

    return 0;
}