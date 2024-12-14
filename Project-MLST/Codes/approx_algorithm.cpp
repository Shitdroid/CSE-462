#include <iostream>
#include <list>
#include <unordered_map>
#include <unordered_set>
#include <vector>
#include <algorithm>

using namespace std;

// ...existing code...

class Node {
public:
    vector<int> adjList; // adjacency list representation
    list<int>* listIn; // pointer to the list the node is in
    list<int>::iterator position; // position of the node in its list
    bool inF; // flag for vertices in V(F)
    int outsideDegree; // outside degree of the node

    Node() : listIn(nullptr), inF(false), outsideDegree(0) {}

    Node(const vector<int>& adjList) 
        : adjList(adjList), listIn(nullptr), inF(false), outsideDegree(adjList.size()) {}

    void addNeighbour(int neighbor) {
        adjList.push_back(neighbor);
        outsideDegree++;
    }
};

class DisjointSet {
    vector<int> parent, rank;

public:
    DisjointSet(int n) : parent(n), rank(n, 0) {
        for (int i = 0; i < n; ++i) {
            parent[i] = i;
        }
    }

    int find(int u) {
        if (u != parent[u]) {
            parent[u] = find(parent[u]);
        }
        return parent[u];
    }

    void unite(int u, int v) {
        int rootU = find(u);
        int rootV = find(v);
        if (rootU != rootV) {
            if (rank[rootU] > rank[rootV]) {
                parent[rootV] = rootU;
            } else if (rank[rootU] < rank[rootV]) {
                parent[rootU] = rootV;
            } else {
                parent[rootV] = rootU;
                rank[rootU]++;
            }
        }
    }
};

class Graph {
    // ...existing code...
    vector<Node> nodes; // vector of nodes
    list<int> R1, R2, R3, R4; // expansion rule candidate lists
    vector<pair<int, int>> treeEdges; // vector of tree edges
    vector<pair<int, int>> Edges; // vector of non-tree edges
    DisjointSet ds; // disjoint set for tracking trees

public:
    Graph(int n) : nodes(n), ds(n) {}

    void addEdge(int u, int v) {
        nodes[u].addNeighbour(v);
        nodes[v].addNeighbour(u);
        Edges.push_back({u, v});
        if (nodes[u].outsideDegree == 3) 
            addToList(u, R4);

        if (nodes[v].outsideDegree == 3) 
            addToList(v, R4);
    }

    void addToList(int v, list<int>& newList) {
        Node& node = nodes[v];
        // Remove v from any list it might be in
        if (node.listIn != nullptr) {
            node.listIn->erase(node.position);
            node.listIn = nullptr;
        }
        // Add v to the new list
        newList.push_back(v);
        node.listIn = &newList;
        node.position = --newList.end();
    }

    void updateLists(const unordered_set<int>& addedToForest, const unordered_set<int>& updatedNodes) {
        for (int v : addedToForest) {
            Node& node = nodes[v];
            if (node.listIn != nullptr) {
                node.listIn->erase(node.position);
                node.listIn = nullptr;
            }
            if (node.outsideDegree >= 2) {
                addToList(v, R1);
            }
        }

        for (int v : updatedNodes) {
            Node& node = nodes[v];
            if (!node.inF) {
                if (node.outsideDegree > 2) {
                    addToList(v, R2);
                } else if (node.outsideDegree == 2) {
                    addToList(v, R3);
                }
            } else if (node.listIn == &R1 && node.outsideDegree<2){
                node.listIn->erase(node.position);
                node.listIn = nullptr;
            }
        }
    }

    void expandVertexWithoutListUpdate(int v){
        nodes[v].inF = true;
        for(int neighbor : nodes[v].adjList){
            if(!nodes[neighbor].inF){
                nodes[neighbor].inF = true;
                treeEdges.push_back({v, neighbor});
                ds.unite(v, neighbor);
            }
        }
    }
    void expandVertex(int v) {
        bool outside = !(nodes[v].inF);
        bool isW = (nodes[v].listIn == &R2 || nodes[v].listIn == &R3);
        nodes[v].listIn = nullptr;
        nodes[v].inF = true;
        unordered_set<int> updatedNodes;
        unordered_set<int> addedToForest;
        if (outside) addedToForest.insert(v);
        for (int neighbor : nodes[v].adjList) {
            if (outside) {
                nodes[neighbor].outsideDegree--;
                updatedNodes.insert(neighbor);
            }
            if (!nodes[neighbor].inF) {
                nodes[neighbor].inF = true;
                treeEdges.push_back({v, neighbor});
                ds.unite(v, neighbor);
                addedToForest.insert(neighbor);
                
                for (int neighbors_neighbor : nodes[neighbor].adjList) {
                    nodes[neighbors_neighbor].outsideDegree--;
                    updatedNodes.insert(neighbors_neighbor);
                }
            } else if (isW) {
                treeEdges.push_back({v, neighbor});
                ds.unite(v, neighbor);
                isW = false;
            }
        }
        updateLists(addedToForest, updatedNodes);
    }

    void phase1() {
        if(R4.empty()){
            int leaves = max(2,(int)nodes.size());
            cout<<leaves<<endl;
        }
        while (!R1.empty() || !R2.empty() || !R3.empty() || !R4.empty()) {
            if (!R1.empty()) {
                int v = R1.front();
                R1.pop_front();
                expandVertex(v);
            } else if (!R2.empty()) {
                int v = R2.front();
                R2.pop_front();
                expandVertex(v);
            } else if (!R3.empty()) {
                int v = R3.front();
                R3.pop_front();
                expandVertex(v);
            } else if (!R4.empty()) {
                int v = R4.front();
                R4.pop_front();
                expandVertex(v);
            }
            // print();
        }
    }

    void phase2(){
        for (int v = 0; v < nodes.size(); ++v) {
            if (!nodes[v].inF) {
                expandVertexWithoutListUpdate(v);
            }
        }
        for(int i=0;i<Edges.size();i++)
        {
            if(ds.find(Edges[i].first)!=ds.find(Edges[i].second))
            {
                treeEdges.push_back(Edges[i]);
                ds.unite(Edges[i].first, Edges[i].second);
            }
        }
    }


    int countLeaves() {
        vector<int>degree(nodes.size(), 0);

        for (int i = 0 ; i < treeEdges.size(); i++) {
            degree[treeEdges[i].first]++;
            degree[treeEdges[i].second]++;
        }

        int leaves = 0;

        for (int i = 0; i < degree.size(); i++) {
            if (degree[i] == 1) {
                leaves++;
            }
        }

        return leaves;
    }
    
    void print() {
        cout << "*********************************" << endl;

        cout << "Tree Edges in the Forest:" << endl;
        for (const auto& edge : treeEdges) {
            cout << edge.first << " - " << edge.second << endl;
        }

        cout << "\nDifferent Trees in the Forest:" << endl;
        unordered_map<int, vector<int>> trees;
        for (int i = 0; i < nodes.size(); ++i) {
            int root = ds.find(i);
            trees[root].push_back(i);
        }

        for (const auto& tree : trees) {
            cout << "Tree rooted at " << tree.first << ": ";
            for (int node : tree.second) {
                cout << node << " ";
            }
            cout << endl;
        }

        cout << "\nNodes in each list:" << endl;
        cout << "R1: ";
        for (int node : R1) {
            cout << node << " ";
        }
        cout << endl;

        cout << "R2: ";
        for (int node : R2) {
            cout << node << " ";
        }
        cout<< endl;

        cout << "R3: ";
        for (int node : R3) {
            cout << node << " ";
        }
        cout << endl;

        cout << "R4: ";
        for (int node : R4) {
            cout << node << " ";
        }
        cout << endl;

        cout << "*********************************" << endl;
    }

    // ...existing code...
};

// ...existing code...

int main() {
    // freopen("input.txt", "r", stdin);
    // freopen("output.txt", "w", stdout);

    int n, m;
    cin >> n >> m;

    Graph graph(n);

    for (int i = 0; i < m; ++i) {
        int u, v;
        cin >> u >> v;
        graph.addEdge(u, v);
    }

    // graph.print();
    
    graph.phase1();

    // graph.print();

    graph.phase2();
    // graph.print();
    int leaves=graph.countLeaves();
    cout<<leaves;
    return 0;
}
