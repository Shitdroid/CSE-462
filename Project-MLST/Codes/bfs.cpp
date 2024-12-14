#include<bits/stdc++.h>
using namespace std;
class node{
    public:
    int data;
    bool isVisited;
    vector<node*> children;
    bool isLeaf;
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
    queue<node*> q;
    q.push(nodes[0]);
    nodes[0]->isVisited = true;
    // int leafCount = 0;
    while(!q.empty()){
        node* temp = q.front();
        q.pop();
        for(int i=0;i<temp->children.size();i++){
            if(!temp->children[i]->isVisited){
                q.push(temp->children[i]);
                temp->isLeaf = false;
                temp->children[i]->isVisited = true;
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
    for(int i=0;i<n;i++)delete nodes[i];
    
}
