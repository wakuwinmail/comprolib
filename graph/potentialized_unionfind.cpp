//http://judge.u-aizu.ac.jp/onlinejudge/description.jsp?id=1330
#include <iostream>
#include <vector>
#include <limits>
#include <numeric>
//cut begin
template<typename T>
struct PotentializedUnionFind{
private:
    private:
    std::vector<int> par;
    std::vector<int> size;
    std::vector<T> diff;
public:
    PotentializedUnionFind(int n){
        par.resize(n);
        std::iota(par.begin(),par.end(),0);
        size.resize(n);
        std::iota(size.begin(),size.end(),0);
        diff.assign(n,0);
    }

    int root(int x){
        if(par[x]==x){
            return x;
        }
        else{
            int r=root(par[x]);
            diff[x]+=diff[par[x]];
            return par[x]=r;
        }
    }

    T potential(int x){
        root(x);
        return diff[x];
    }

    T potential_diff(int x,int y){
        return potential(y)-potential(x);
    }

    bool same(int x,int y){//0-indexed
        return root(x)==root(y);
    }

    void unite(int x,int y,T w){//0-indexed,potential(y)-potential(x)=w
        w+=potential(x);
        w-=potential(y);
        
        x=root(x);
        y=root(y);
        if(x==y)return;

        if(size[x]<size[y]){
            par[x]=y;
            size[y]+=size[x];
            diff[x]=-w;
        }
        else{
            par[y]=x;
            size[x]+=size[y];
            diff[y]=w;
        }
    }
};
//cut end

int solve(){
    int n,m;
    std::cin>>n>>m;
    if(n==0&&m==0)return 1;
    PotentializedUnionFind<long long> pu(n);
    for (int i = 0; i < m; ++i){
        char c;
        std::cin>>c;
        int x,y,z;
        std::cin>>x>>y;
        --x;--y;
        if(c=='!'){
            std::cin>>z;
            pu.unite(x,y,z);
        }
        else{
            if(pu.same(x,y))std::cout<<pu.potential_diff(x,y)<<std::endl;
            else std::cout<<"UNKNOWN"<<std::endl;
        }
    }
    return 0;
}

int main(){
    while(solve()==0);
    return 0;
}