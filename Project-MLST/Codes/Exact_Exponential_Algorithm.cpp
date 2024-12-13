#include <bits/stdc++.h>
using namespace std;
struct Graph{
    vector<set<int> > g;
    set<int>IN, BN, LN, FL, FREE;
    int n;
    void reset(){
        IN.clear();
        BN.clear();
        LN.clear();
        FL.clear();
        FREE.clear();
    }
    Graph(int n){
        g.resize(n);
        this->n = n;
    }
    void addEdge(int u, int v){
        g[u].insert(v);
        g[v].insert(u);
    }
    bool isEdge(int u, int v){
        return g[u].find(v) != g[u].end();
    }
    void removeEdge(int u, int v){
        g[u].erase(v);
        g[v].erase(u);
    }
    int deg(int u){
        if(BN.find(u) != BN.end()) {
            int ret = 0;
            for(int v : g[u]){
                if(FREE.find(v) != FREE.end() or FL.find(v) != FL.end()) //counting how many of the neighbors of u are either FREE or FL nodes.
                 ++ret;
            }
            return ret;
        }
        else if(FL.find(u) != FL.end()) {
            int ret = 0;
            for(int v : g[u]){
                if(FREE.find(v) != FREE.end() or BN.find(v) != BN.end()) ++ret;
            }
            return ret;
        }
        else if(FREE.find(u) != FREE.end()) {
            int ret = 0;
            for(int v : g[u]){
                if(FREE.find(v) != FREE.end() or FL.find(v) != FL.end() or BN.find(v) != BN.end() ) ++ret;
            }
            return ret;
        }
        else assert(false);
    }
    
    int timer;
    vector<int>tin, low;
    vector<int> visited;
    set<int> art_points;
    void dfs(int v, int p = -1){  // Shafaet blog page 101
        visited[v] = 1;
        tin[v] = low[v] = timer++;
        int children = 0;
        for(int to: g[v]){
            if(to == p) continue;
            if(visited[to]){
                low[v] = min(low[v], tin[to]);
            }
            else{
                dfs(to, v);
                low[v] = min(low[v], low[to]);
                if(low[to] >= tin[v] && p != -1){
                    art_points.insert(v);
                }
                ++children;
            }
        }
        if(p == -1 && children > 1){
            art_points.insert(v);
        }
    }

    void find_art_point(){ // Shafaet blog page 101
        timer = 0;
        tin.assign(n, -1);
        low.assign(n, -1);
        visited.assign(n, 0);
        art_points.clear();
        for(int i = 0; i < n; i++){
            if(!visited[i]){
                dfs(i, -1);
            }
        }
    }

    bool R1(){
        // cout << "Inside R1" << endl;
        bool changedonce = false;
        while(true){
            // cout << "Inside R1 while" << endl;
            bool applied = false;
            for(int u = 0; u < n; ++u){
                if(FL.find(u) != FL.end()){
                    for(auto v: g[u]){
                        if(FL.find(v) != FL.end()){
                            // cout << "removing edge " << u << " " << v << endl;
                            removeEdge(u, v);
                            applied = true;
                            changedonce = true;
                            break;
                        }
                    }
                    if(applied) break;  
                }
                else if(BN.find(u) != BN.end()){
                    for(auto v: g[u]){
                        if(BN.find(v) != BN.end()){
                            // cout << "removing edge BN" << u << " " << v << endl;
                            removeEdge(u, v);
                            // cout << "here " << endl;
                            applied = true;
                            changedonce = true; 
                            break;
                        }
                    }
                    if(applied) break;
                }
            }
            // cout << applied << endl;
            if(!applied) break;
        }
        return changedonce;
    }
    
    bool R2(){
        bool changedonece = false;
        while(true){
            bool applied = false;
            for(auto e: BN){
                if(deg(e) == 0){
                    LN.insert(e);
                    BN.erase(e);
                    applied = true;
                    changedonece = true;
                    break;
                }
            }
            if(!applied) break;
        }
        return changedonece;
    }
    bool R3(){
        bool changedonce = false;
        while(true){
            bool applied = false;
            for(auto e: FREE){
                if(deg(e) == 1){
                    FREE.erase(e);
                    FL.insert(e);
                    applied = true;
                    changedonce = true;
                    break;
                }
            }
            if(!applied) break;
        }
        return changedonce;
    }
    bool R4(){
        bool changedonce = false;
        while(true){
            bool applied = false;
            for(auto v: FREE){
                bool check = false;
                for(auto u: g[v]){
                    if(FREE.find(u) != FREE.end() 
                        || FL.find(u) != FL.end()){
                        check = true;
                        break;
                    }
                }
                if(!check){
                    FREE.erase(v);
                    FL.insert(v);
                    applied = true;
                    changedonce = true;
                }
                if(applied) break;
            }
            if(!applied) break;
        }
        return changedonce;
    }
    bool R5(){
        bool changedonce = false;
        while(true){
            bool applied = false;
            for(auto x: FREE){
                if(deg(x) == 2){
                    vector<int>neighbors(g[x].begin(), g[x].end());
                    for(int i=0; i<(int)neighbors.size(); ++i){
                        for(int j=i+1; j<(int)neighbors.size(); ++j){
                            int v = neighbors[i];
                            int w = neighbors[j];
                            if(isEdge(v, w)){
                                FREE.erase(x);
                                FL.insert(x);
                                applied = true;
                                changedonce = true;
                                goto endd;
                            }
                        }
                    }
                }
            }
            endd:;
            if(!applied) break;
        }
        return changedonce;
    }
    bool R6(){
        bool changedonce = false;
        while(true){
            bool changed = false;
            find_art_point();
            for(auto e: art_points){
                if(BN.find(e) != BN.end()){
                    BN.erase(e);
                    IN.insert(e);
                    for(int v : g[e]){
                        if(FREE.find(v) != FREE.end()){
                            FREE.erase(v);
                            BN.insert(v);
                        }
                        if(FL.find(v) != FL.end()){
                            FL.erase(v);
                            LN.insert(v);
                        }
                    }
                    changed = true;
                    changedonce = true;
                    break;
                }

            }
            if(!changed)break;
        }
        return changedonce;
    }
    bool R7(){
        bool changedonce = false;
        while(true){
            bool changed = false;
            for(auto u: LN){
                for(auto v: g[u]){
                    if(IN.find(v) == IN.end()){
                        removeEdge(u, v);
                        changed = true;
                        changedonce = true;
                        goto endr7;
                    }
                }
            }
            endr7:;
            if(!changed)break;
        }
        return changedonce;
    }
    void reduction(){
        bool changed = true;
        while(changed){
            // cout << "inside reduction" << endl;
            changed = R1() || R2() || R3() || R4() || R5() || R6() || R7();
            // exit(0);
        }
    }
    void print(){
        cout << "IN: ";
        for(auto x: IN) cout << x << " ";
        cout << endl;
        cout << "BN: ";
        for(auto x: BN) cout << x << " ";
        cout << endl;
        cout << "LN: ";
        for(auto x: LN) cout << x << " ";
        cout << endl;
        cout << "FL: ";
        for(auto x: FL) cout << x << " ";
        cout << endl;
        cout << "FREE: ";
        for(auto x: FREE) cout << x << " ";
        cout << endl;
    }
    void printGraph(){
        for(int i = 0; i < n; i++){
            cout << i << ": ";
            for(auto x: g[i]) cout << x << " ";
            cout << endl;
        }
    }
    bool check_if_connected(int root){
        vector<int> visited(n, 0);
        queue<int> q;
        q.push(root);
        visited[root] = 1;
        while(!q.empty()){
            int u = q.front();
            q.pop();
            for(auto v: g[u]){
                if(visited[v] == 0){
                    visited[v] = 1;
                    q.push(v);
                }
            }
        }
        for(int i = 0; i < n; i++){
            if(visited[i] == 0) return false;
        }
        return true;
    }
    void insert_IN(int x){
        if(BN.find(x) != BN.end())
            BN.erase(x);
        else if(FL.find(x) != FL.end())
            FL.erase(x);
        else if(FREE.find(x) != FREE.end())
            FREE.erase(x);
        else assert(false);

        IN.insert(x);
        for(int v : g[x]){
            if(FREE.find(v) != FREE.end()){
                FREE.erase(v);
                BN.insert(v);
            }
            if(FL.find(v) != FL.end()){
                FL.erase(v);
                LN.insert(v);
            }
        }
    }
    
};
// int cnt = 0;
int M_Algorithm(int root, Graph g){
    // ++cnt;
    // cout << cnt << endl;
    // cout << "test redu" << endl;
    g.reduction();
    // cout << "test redu end " << endl;

    if(g.check_if_connected(root) == false) return 0;
    if((int)g.IN.size() + (int)g.LN.size() == g.n) return g.LN.size();
    int mx = INT_MIN;
    int mxv = -1;
    for(auto e: g.BN){
        int tmp = g.deg(e);
        if(tmp > mx){
            mx = tmp;
            mxv = e;
        }
    }
    if(mxv == -1){
        // cout << g.BN.size() << endl;
        // cout << "h/ere \n";
        return 0;
    }
    int neighbourcount = 0;
    for(auto x: g.g[mxv]){
        if(g.FL.find(x) != g.FL.end()) ++neighbourcount;
    }
    if(mx >= 3 || (mx == 2 &&  neighbourcount)){
        Graph g2 = g;
        g2.insert_IN(mxv);
        g.BN.erase(mxv);
        g.LN.insert(mxv);
        return max(M_Algorithm(root, g), M_Algorithm(root, g2));
    }
    else if(mx == 2){
        vector<int> tmpar;
        for(auto x: g.g[mxv]){
            if(g.FREE.find(x) != g.FREE.end()){
                tmpar.push_back(x);
            }
        }
        if(tmpar.size() != 2){
            cout << tmpar.size() << endl;
            
        }
        assert(tmpar.size() == 2);
        int x1 = tmpar[0];
        int x2 = tmpar[1];
        int dx1 = g.deg(x1);
        int dx2 = g.deg(x2);
        if(dx1 > dx2){
            swap(x1, x2);
            swap(dx1, dx2);
            swap(tmpar[0], tmpar[1]);
        }
        if(dx1 == 2){
            for(auto z: g.g[x1])if(z != mxv){
                if(g.FREE.find(z) != g.FREE.end()){
                    Graph g2 = g, g3 = g;
                    g.BN.erase(mxv);
                    g.LN.insert(mxv);
                    g2.insert_IN(mxv);
                    g2.insert_IN(x1);
                    g3.insert_IN(mxv);
                    g3.FREE.erase(x1);
                    g3.LN.insert(x1);
                    return max(M_Algorithm(root, g), max(M_Algorithm(root, g2), M_Algorithm(root, g3)));
                    
                }
                else if(g.FL.find(z) != g.FL.end()){
                    g.insert_IN(mxv);
                    return M_Algorithm(root, g); 
                }
                else{
                    return 0;
                }
            }
            cout << "souldnt be here" << endl;
            return 0;
        }
        else{
            set<int> inter; 
            for(auto x: g.g[x2]){
                if(g.g[x1].find(x) != g.g[x1].end()){
                    inter.insert(x);
                }
            }
            for(auto x: g.FL){
                if(inter.find(x) != inter.end()){
                    inter.erase(x);
                }
            }
            bool chk1 = (inter.size() == 1 && *inter.begin() == mxv);
            set<int> inter2;
            for(auto x: g.FL){
                if(g.g[x1].find(x) != g.g[x1].end() && g.g[x2].find(x) != g.g[x2].end()){
                    inter2.insert(x);
                }
            }
            bool chk2 = true;
            for(auto x: inter2){
                if(g.deg(x) < 3){
                    chk2 = false;
                    break;
                }
            }
            if(chk1 && chk2){
                Graph g2 = g, g3 = g, g4 = g;
                g.BN.erase(mxv);
                g.LN.insert(mxv);

                g2.insert_IN(mxv);
                g2.insert_IN(x1);

                g3.insert_IN(mxv);
                g3.insert_IN(x2);
                g3.FREE.erase(x1);
                g3.LN.insert(x1);

                g4.insert_IN(mxv);
                g4.FREE.erase(x1);
                g4.FREE.erase(x2);
                g4.LN.insert(x1);
                g4.LN.insert(x2);

                set<int> tmpset;
                for(auto x: g4.FREE){
                    if(g4.g[x1].find(x) != g4.g[x1].end() || g4.g[x2].find(x) != g4.g[x2].end()){
                        tmpset.insert(x);
                    }
                }
                for(auto x: tmpset){
                    g4.FREE.erase(x);
                    g4.FL.insert(x);
                }

                tmpset.clear();
                for(auto x: g4.BN){
                    if(g4.g[x1].find(x) != g4.g[x1].end() || g4.g[x2].find(x) != g4.g[x2].end()){
                        tmpset.insert(x);
                    }
                }
                tmpset.erase(mxv);
                for(auto x: tmpset){
                    g4.BN.erase(x);
                    g4.LN.insert(x);
                }
                return max({M_Algorithm(root, g), M_Algorithm(root, g2),
                 M_Algorithm(root, g3), M_Algorithm(root, g4)});
            }
            else{
                Graph g2 = g, g3 = g;
                g.BN.erase(mxv); 
                g.LN.insert(mxv);
                
                g2.insert_IN(mxv);
                g2.insert_IN(x1);

                g3.insert_IN(mxv);
                g3.insert_IN(x2);
                g3.FREE.erase(x1);
                g3.LN.insert(x1);
                return max(M_Algorithm(root, g), 
                max(M_Algorithm(root, g2), M_Algorithm(root, g3)));
            }
        }
    }
    else if(mx == 1){
        vector<int> path = {mxv};
        while(true){
            bool found = false;
            for(int u : g.g[ path.back() ]){
                if(path.size()>=2 and path[path.size()-2] == u) continue;
                if(g.FREE.find(u) != g.FREE.end()){
                    if(g.deg(u)==2){
                        path.push_back(u);
                        found = true;
                        break;
                    }
                }
            }
            if(!found) break;
        }
        set<int> tmpset = g.g[path.back()];
        for(auto e: path) tmpset.erase(e);
        int z = -1;
        for(auto e: tmpset){
            if(g.IN.find(e) == g.IN.end()){
                z = e;
            }
        }
        if(z == -1){
            return 0;
        }
        int zdeg = g.deg(z);
        if(g.FL.find(z) != g.FL.end() and zdeg == 1){
            for(int w: path){
                g.insert_IN(w);
            }
            g.FL.erase(z);
            g.LN.insert(z);
            return M_Algorithm(root, g);
        }
        else if(g.FL.find(z) != g.FL.end() and zdeg > 1){
            int last = path.back();
            path.pop_back();
            for(int w: path){
                g.insert_IN(w);
            }
            g.FREE.erase(last);
            g.LN.insert(last);
            return M_Algorithm(root, g);
        }
        else if(g.BN.find(z) != g.BN.end()){
            g.BN.erase(mxv);
            g.LN.insert(mxv);
            return M_Algorithm(root, g);
        }
        else if(g.FREE.find(z) != g.FREE.end()){
            Graph g2 = g;
            for(int w: path){
                g.insert_IN(w);
            }
            g.insert_IN(z);
            g2.BN.erase(mxv);
            g2.LN.insert(mxv);
            return max(M_Algorithm(root, g), M_Algorithm(root, g2));
        }
        else return 0;
        
    }
    
    else return 0;
}

void solve(Graph g){
    int ans = INT_MIN;
    for(int i = 0; i < g.n; i++){
        // cout << i << endl;
        Graph g1 = g;
        g1.reset();
        for(int j = 0; j < g.n; ++j)
            g1.FREE.insert(j);
        g1.IN.insert(i);
        g1.FREE.erase(i);
        for(auto u: g.g[i]){
            g1.BN.insert(u);
            g1.FREE.erase(u);
        }
        // cout << "here " << endl;
        ans = max(ans, M_Algorithm(i, g1));
    }
    cout << ans << '\n';
}
int main(){
    ios::sync_with_stdio(false);
    cin.tie(0);
    int n, m;
    cin >> n >> m;
    Graph g(n);
    for(int i = 0; i < m; i++){
        int u, v;
        cin >> u >> v;
        // cout << u << ' ' << v << ' ' << g.g.size() << endl;
        g.addEdge(u, v);
    }
    solve(g);
    return 0;
}
