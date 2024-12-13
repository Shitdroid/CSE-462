#include <iostream>
#include <vector>
#include <queue>
#include <functional>

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

    auto cmp = [&](int a, int b) {
        return degree[a] > degree[b];
    };

    priority_queue<int, vector<int>, decltype(cmp)> pq(cmp);
    vector<bool> visited(n, false);

    pq.push(0);
    visited[0] = true;

    int leafCount = 0;

    while (!pq.empty()) {
        int node = pq.top();
        pq.pop();

        if (degree[node] == 1) {
            leafCount++;
        }

        for (int neighbor : adj[node]) {
            if (!visited[neighbor]) {
                pq.push(neighbor);
                visited[neighbor] = true;
            }
        }
    }

    cout << leafCount << endl;

    return 0;
}