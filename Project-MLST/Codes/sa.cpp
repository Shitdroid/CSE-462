#include<bits/stdc++.h>


// class declaration
class Graph;

// function declarations
Graph priorityBfs(Graph& graph, int startVertex); 
Graph simulatedAnnealing(Graph& graph, int iterations);
void randomNeighbouringTree(Graph& graph);
bool isConnected(Graph& graph);
bool isTree(Graph& graph);
int countLeafNodes(Graph& graph);
std::pair<double, int>fitnessFunction(Graph& graph);

std::random_device rd;
std::mt19937 gen(rd());

class Graph{
    
public:
    int numVertices;
    std::vector<std::vector<int>> adjMatrix;
    std::unordered_map<int, int> degrees;

    Graph(int numVertices) {
        this->numVertices = numVertices;
        this->adjMatrix = std::vector<std::vector<int>>(numVertices, std::vector<int>(numVertices, 0));
    }

    void addEdge(int u, int v) {
        adjMatrix[u][v] = 1;
        adjMatrix[v][u] = 1;
        degrees[u]++;
        degrees[v]++;
    }

    void removeEdge(int u, int v) {
        adjMatrix[u][v] = 0;
        adjMatrix[v][u] = 0;
        degrees[u]--;
        degrees[v]--;
    }

    int findDegree(int u) {
        int degree = 0;

        for (int v = 0; v < numVertices; v++) {
            degree += adjMatrix[u][v];
        }

        return degree;
    }

    int getMaxDegreeVertex() {
        int maxDegree = -1;
        int maxDegreeVertex = -1;

        for (int u = 0; u < numVertices; u++) {
            if (degrees[u] >= maxDegree) {
                maxDegree = degrees[u];
                maxDegreeVertex = u;
            }
        }
        return maxDegreeVertex;
    }

    Graph& operator=(const Graph& other) {
        if (this != &other) {
            numVertices = other.numVertices;
            adjMatrix = other.adjMatrix; 
            degrees = other.degrees;
        }
        // std::cout << "Assignment operator called\n";
        return *this;
    }


};

Graph priorityBfs(Graph& graph, int startVertex) {
    std::cout << "Start vertex: " << startVertex << std::endl;
    Graph tree(graph.numVertices);
    int visited[graph.numVertices] = {0};
    std::priority_queue<std::pair<int, int>> pq;

    pq.emplace(-graph.degrees[startVertex], startVertex);
    visited[startVertex] = true;

    while (!pq.empty()) {
        int u = pq.top().second;
        pq.pop();

        for (int v = 0; v < graph.numVertices; v++) {
            if (graph.adjMatrix[u][v] == 1 && !visited[v]) {
                int priority = graph.degrees[v] - graph.degrees[u];
                tree.addEdge(u, v);
                visited[v] = true;
                pq.emplace(-priority, v);
            }
        }
    }

    return tree;
}

Graph simulatedAnnealing(Graph& graph, int iterations) {
    std::uniform_real_distribution<> probDist(0.0, 1.0);
    
    Graph currTree = graph;
    double currFitness = fitnessFunction(currTree).first;

    double temperature = 1.0;
    double coolingRate = 0.995;

    for (int i = 0; i < iterations; i++) {
        Graph nextTree = currTree;
        randomNeighbouringTree(nextTree);
        // std::cout << "Debug 1" << std::endl;

        double nextFitness = fitnessFunction(nextTree).first;
        double acceptanceProbability = std::exp((nextFitness - currFitness - 1) / temperature);
        
        // std::cout << nextFitness << " " << currFitness << " " << temperature << '\n';

        if (nextFitness >= currFitness || probDist(gen) < acceptanceProbability) {
            currTree = nextTree;
            currFitness = nextFitness;
        }

        temperature *= coolingRate;
    }

    std::cout << temperature << std::endl;

    return currTree;
}

void randomNeighbouringTree(Graph& graph) {
    std::vector<std::pair<int, int>> treeEdges;

    // step 1: remove a random edge from the tree
    for (int u = 0; u < graph.numVertices; u++) {
        for (int v = u + 1; v < graph.numVertices; v++) {
            if (graph.adjMatrix[u][v] == 1) {
                treeEdges.emplace_back(u, v);
            }
        }
    }
    std::uniform_int_distribution<> edgeDist(0, treeEdges.size() - 1);
    if (treeEdges.empty()) {
        throw std::runtime_error("No edges available to remove.");
    }

    std::pair<int, int> randEdge = treeEdges[edgeDist(gen)];
    // graph.removeEdge(edge.first, edge.second);

    graph.adjMatrix[randEdge.first][randEdge.second] = 0;
    graph.adjMatrix[randEdge.second][randEdge.first] = 0;

    // std::cout << "Debug 2" << std::endl;

    //Step 2: Find a random edge to reconnect the components

    std::vector<std::vector<int>> components;
    int visited[graph.numVertices] ={0};

    for (int u = 0; u < graph.numVertices; u++) {
        if (!visited[u]) {
            std::vector<int> component;
            std::queue<int> q;

            q.push(u);
            // visited[u] = true;

            while (!q.empty()) {
                int currVertex = q.front();
                q.pop();
                visited[currVertex] = true;
                component.push_back(currVertex);

                for (int v = 0; v < graph.numVertices; v++) {
                    if (graph.adjMatrix[currVertex][v] == 1 && !visited[v]) {
                        q.push(v);
                    }
                }
            }
            components.push_back(component);
        }
    }
    // std::cout << "Debug 3" << std::endl;
    if (components.size() < 2) {
        throw std::runtime_error("Not enough components to select two distinct ones.");
    }


    std::uniform_int_distribution<> compDist(0, components.size()-1);

    int i = compDist(gen);
    int j = compDist(gen);

    // std::cout << "i: " << i << " j: " << j << std::endl;

    while (i == j) {
        j = compDist(gen);
    }

    std::vector<int>comp1 = components[i];
    std::vector<int>comp2 = components[j];

    // std::cout << "Debug 4" << std::endl;

    // std::cout << "Comp1 size: " << comp1.size() << std::endl;
    // std::cout << "Comp2 size: " << comp2.size() << std::endl;
    

    std::uniform_int_distribution<> nodeDist1(0, comp1.size() - 1);
    std::uniform_int_distribution<> nodeDist2(0, comp2.size() - 1);

    // std::cout << "Debug 4.5" << std::endl;

    int node1 = comp1[nodeDist1(gen)];
    int node2 = comp2[nodeDist2(gen)];

    while (graph.adjMatrix[node1][node2] == 1) {
        node1 = comp1[nodeDist1(gen)];
        node2 = comp2[nodeDist2(gen)];
    }

    // std::cout << "Debug 5" << std::endl;

    graph.adjMatrix[node1][node2] = 1;
    graph.adjMatrix[node2][node1] = 1;
}

bool isConnected(Graph& graph) {
    int visited[graph.numVertices] = {0};
    std::queue<int> q;
    int allVisited = 0;

    q.push(0);

    while (!q.empty()) {
        int u = q.front();
        q.pop();
        if (!visited[u]) {
            visited[u] = true;
            allVisited++;

            for (int v = 0; v < graph.numVertices; v++) {
                if (graph.adjMatrix[u][v] == 1 && !visited[v]) {
                    q.push(v);
                }
            }
        }
    }

    return allVisited == graph.numVertices;
}

bool isTree(Graph& graph) {
    int numEdges = 0;

    for (int u = 0; u < graph.numVertices; u++) {
        for (int v = u + 1; v < graph.numVertices; v++) {
            numEdges += graph.adjMatrix[u][v];
        }
    }

    numEdges /= 2;

    return numEdges == graph.numVertices - 1 && isConnected(graph);
}

int countLeafNodes(Graph& graph) {
    int leafNodes = 0;

    for (int u = 0; u < graph.numVertices; u++) {
        if (graph.findDegree(u) == 1) {
            leafNodes++;
        }
    }

    return leafNodes;
}

std::pair<double, int> fitnessFunction(Graph& graph) {
    double totalWeight = 0.0;
    int leafNodes = 0;

    for (int u = 0; u < graph.numVertices; u++) {
        int deg = graph.findDegree(u);

        if (deg == 1) {
            totalWeight += 1.0;
            leafNodes++;
        } 
        else if (deg == 2 || deg == 3) {
            totalWeight += 0.5;
        }
    }

    return {totalWeight/graph.numVertices, leafNodes};
}

Graph readGraphFromFile(const std::string& filePath) {
    std::ifstream file(filePath);
    int numVertices, numEdges;
    file >> numVertices >> numEdges;

    Graph graph(numVertices);
    for (int i = 0; i < numEdges; ++i) {
        int u, v;
        file >> u >> v;
        graph.addEdge(u, v);
    }
    return graph;
}

int main() {
    std::string filePath = "example.txt";
    Graph graph = readGraphFromFile(filePath);
    std::cout << graph.findDegree(28) << " " << graph.findDegree(30) << std::endl;

    Graph tree = priorityBfs(graph, graph.getMaxDegreeVertex());
    std::cout << "Tree is tree: " << isTree(tree) << "\n";
    std::cout << "Leaf nodes in tree: " << countLeafNodes(tree) << "\n";

    int iterations = 1000;
    Graph resultTree = simulatedAnnealing(tree, iterations);
    // std::cout << "hello"<<std::endl;

    std::cout << "Resulting tree is tree: " << isTree(resultTree) << "\n";
    std::cout << "Leaf nodes in resulting tree: " << countLeafNodes(resultTree) << "\n";

    return 0;
}
