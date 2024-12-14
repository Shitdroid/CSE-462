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
    void setVisited(){
        if(this->isVisited)return;
        for(int i=0;i<children.size();i++){
            children[i]->priority--;
        }
        this->isVisited=true;
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
    int max_priority_index=0;
    for(int i=0;i<n;i++){
        nodes[i]->priority=nodes[i]->children.size();
        if(nodes[i]->priority>nodes[max_priority_index]->priority)max_priority_index=i;
    }
    auto compare = [](node* a, node* b) {
        return a->priority < b->priority;
    };

    priority_queue<node*, vector<node*>, decltype(compare)> pq(compare);

    pq.push(nodes[max_priority_index]);
    nodes[max_priority_index]->setVisited();
    while (!pq.empty()) {
        node* current = pq.top();
        pq.pop();

        for (auto child : current->children) {
            if (!child->isVisited) {
                child->setVisited();
                pq.push(child);
                current->isLeaf = false;
            }
        }
    }
    int leafCount = 0;
    for(int i=0;i<n;i++){
        if(nodes[i]->isLeaf){
            leafCount++;
        }
    }
    cout<<leafCount;
    for(int i=0;i<n;i++)delete nodes[i];
}
