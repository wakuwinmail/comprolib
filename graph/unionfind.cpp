#include <iostream>
#include <vector>

struct UnionFind{
private:
    std::vector<int> par;
    std::vector<int> rank;
public:
    UnionFind(int size){
        par=std::vector<int>(size);
        rank=std::vector<int>(size);
        for(int i=0;i<size;++i){
            par[i]=i;
            rank[i]=1;
        }
    }

    int root(int x){
        if(par[x]==x)return x;
        else {
            par[x]=root(par[x]);
            return par[x];
        }
    }

    bool same(int x,int y){//0-indexed
        return root(x)==root(y);
    }

    void unite(int x,int y){//0-indexed
        x=root(x);
        y=root(y);
        if(x==y)return;

        if(rank[x]<rank[y]){
            par[x]=y;
        }
        else{
            par[y]=x;
            if(rank[x]==rank[y])++rank[x];
        }
    }
};

void solve(){
    int n,q;
    std::cin>>n>>q;
    UnionFind uf(n);
    int p,a,b;
    for(int i=0;i<q;++i){
        std::cin>>p>>a>>b;
        if(p==0)uf.unite(a,b);
        else{
            if(uf.same(a,b))std::cout<<"Yes\n";
            else std::cout<<"No\n";
        }
    }
}

int main(){
    solve();
    return 0;
}