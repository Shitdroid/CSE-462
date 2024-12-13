#include<bits/stdc++.h>
using namespace std;
class node{
    public:
    int data;
    bool isVisited;
    vector<node*> children;
    bool isLeaf;
    int priority;
    node(int data){
        this->data = data;
        this->isVisited = false;
        this->isLeaf = true;
    }
};
int main(){
    int n,m;
    cin>>n>>m;
    vector<node*> nodes;
    for(int i=0;i<n;i++){
        node* temp = new node(i);
        nodes.push_back(temp);
    }
    for(int i=0;i<m;i++){
        int u,v;
        cin>>u>>v;
        nodes[u]->children.push_back(nodes[v]);
        nodes[v]->children.push_back(nodes[u]);
    }
    auto compare = [](node* a, node* b) {
        return a->priority < b->priority;
    };

    priority_queue<node*, vector<node*>, decltype(compare)> pq(compare);

    nodes[0]->priority = nodes[0]->children.size();
    pq.push(nodes[0]);
    nodes[0]->isVisited = true;

    while (!pq.empty()) {
        node* current = pq.top();
        pq.pop();

        if (current->isVisited) continue;
        current->isVisited = true;

        if (current->children.empty()) {
            current->isLeaf = true;
        } else {
            current->isLeaf = false;
        }

        for (auto child : current->children) {
            if (!child->isVisited) {
                child->priority = current->priority - child->children.size();
                pq.push(child);
                child->isVisited = true;
            }
        }
    }
    int leafCount = 0;
    for(int i=0;i<n;i++){
        if(nodes[i]->isLeaf){
            leafCount++;
        }
    }
    cout<<leafCount<<endl;
}
