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
    auto compare = [](node* a, node* b) {
        return a->priority < b->priority;
    };
    int max_leaf_count=-1;
    for(int i=0;i<nodes.size();i++){
        for(int j=0;j<n;j++){
            nodes[j]->priority=nodes[j]->children.size();
            nodes[j]->isVisited=false;
            nodes[j]->isLeaf=true;
        }

        priority_queue<node*, vector<node*>, decltype(compare)> pq(compare);

        pq.push(nodes[i]);
        nodes[i]->setVisited();
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
        if(leafCount>max_leaf_count)max_leaf_count=leafCount;
    }
    cout<<max_leaf_count;
    for(int i=0;i<n;i++)delete nodes[i];
}
