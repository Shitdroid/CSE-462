#include <iostream>
#include <vector>
#include <queue>
#include <functional>

using namespace std;

int bfs(const vector<vector<int>>& adj, const vector<int>& degree, int start) {
    auto cmp = [&](int a, int b) {
        return degree[a] > degree[b];
    };

    priority_queue<int, vector<int>, decltype(cmp)> pq(cmp);
    vector<bool> visited(adj.size(), false);

    pq.push(start);
    visited[start] = true;

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

    return leafCount;
}

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

    int maxLeafCount = 0;

    for (int i = 0; i < n; ++i) {
        maxLeafCount = max(maxLeafCount, bfs(adj, degree, i));
    }

    cout << maxLeafCount << endl;

    return 0;
}