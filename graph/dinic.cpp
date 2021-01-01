//https://onlinejudge.u-aizu.ac.jp/courses/library/5/GRL/6/GRL_6_A
#include <iostream>
#include <vector>
#include <queue>

template<typename T=long long>
struct flow_edge {
public:
    int to;
    T cap;
    int rev;
    flow_edge(int to,T cap,int rev):to(to),cap(cap),rev(rev){}
};

template<typename T>
using graph = std::vector<std::vector<flow_edge<T>>>;

template<typename T = long long>
struct dinic {
private:
    graph<T> g;
    T flow;
    int n;
    std::vector<int> level;
    std::vector<int> used;
    T inf;//流量の最大値

public:
    dinic(int n,T inf = 1000000007):n(n),inf(inf){
        g.resize(n);
    }

    bool add_edge(int from,int to,int cap) {//0-indexed
        if(from>=n||from<0||to>=n||to<0)return false;
        g[from].emplace_back(to,cap,g[to].size());
        g[to].emplace_back(from,0,g[from].size()-1);
        return true;
    }

    T max_flow(int s,int t) {
        flow=0;
        bfs(s);
        while(level[t]!=-1) {
/*
            std::cerr<<level[t]<<std::endl;
            int x;
            std::cin>>x;
            if(x==1) {
                for(int i = 0; i < n; ++i){
                    std::cerr<<i<<":"<<std::endl;
                    for(auto j:g[i]) {
                        std::cerr<<j.to<<" "<<j.cap<<" "<<j.rev<<std::endl;
                    }
                }
            }
*/
            T ret=0;
            used.assign(n,0);
            while(true) {
                ret=dfs(s,t,inf);
                //std::cerr<<ret<<std::endl;
                if(ret==0)break;
                flow+=ret;
            }
            
            bfs(s);
        }
        return flow;
    }

private:
    void bfs(int s) {
        level.assign(n,-1);
        level[s]=0;
        std::queue<int> q;
        q.emplace(s);
        while(!q.empty()) {
            int x=q.front();
            q.pop();
            for(auto i:g[x]) {
                if(i.cap<=0||level[i.to]>=0)continue;
                q.emplace(i.to);
                level[i.to]=level[x]+1;
            }
        }
    }

    int dfs(int v,int t,T f) {//f:これまでに見た最小の容量
        //std::cerr<<v<<" "<<f<<std::endl;
        if(v==t) {
            return f;
        }
        int rf=0;
        for(int j = used[v]; j < g[v].size(); ++j) {
            ++used[v];
            if(level[v]>=level[g[v][j].to]||g[v][j].cap<=0) {
                continue;
            }
            rf=dfs(g[v][j].to,t,std::min(f,g[v][j].cap));
            if(rf>0) {
                used[v]=j+1;
                g[v][j].cap-=rf;
                g[g[v][j].to][g[v][j].rev].cap+=rf;
                return rf;
            }
        }
        return 0;
    }
};


void solve() {
    int v,e;
    std::cin>>v>>e;
    dinic<int> di(v);
    for(int i = 0; i < e; ++i){
        int a,b,c;
        std::cin>>a>>b>>c;
        di.add_edge(a,b,c);
    }
    std::cout<<di.max_flow(0,v-1)<<std::endl;
}

int main() {
    solve();
    return 0;
}