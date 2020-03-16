//http://judge.u-aizu.ac.jp/onlinejudge/description.jsp?id=GRL_5_A&lang=jp
#include <vector>
#include <iostream>
#include <utility>

template<typename T=long long>
struct edge {
public:
    int from;
    int to;
    T cost;
    edge(int from,int to,T cost):from(from),to(to),cost(cost){}
};

template<typename T>
using graph = std::vector<std::vector<edge<T>>>;

//G:グラフを管理してる型（vector<vector>やvector<set>を指定する）
//T:辺のコスト
template <typename T>
struct TreeDiam {
private:
    using G = graph<T>;
    int u,v;//直径の両端
    T maxdist;//直径
    void dfs(G &g,int x,int p,int dist) {
    //std::cerr<<"x:"<<x<<std::endl;
        if(dist>maxdist){
            v=x;
            maxdist=dist;
        }
        for(auto y:g[x]) {
            if(y.to==p)continue;
            dfs(g,y.to,x,dist+y.cost);
        }
    }

public:
    TreeDiam(G g) {
        u=0;//仮に置いた端点
        maxdist=0;
        dfs(g,u,-1,0);
    //std::cerr<<"md:"<<maxdist<<std::endl;
        u=v;
        maxdist=0;
        dfs(g,u,-1,0);
    }

    std::pair<int,int> end_nodes(){
        return std::make_pair(u,v);
    }

    T diameter(){
        return maxdist;
    }
};

void solve() {
    int n;
    std::cin>>n;
    graph<int> G(n);
    for(int i = 0;i < n-1;++i) {
        int s,t,w;
        std::cin>>s>>t>>w;
        G[s].emplace_back(s,t,w);
        G[t].emplace_back(t,s,w);
    }
    TreeDiam<int> td(G);
    std::cout<<td.diameter()<<std::endl;
}

int main() {
    solve();
    return 0;
}
