#include <iostream>
#include <vector>
//cut begin
struct UnionFind{
private:
    std::vector<int> par;
    std::vector<int> siz;
public:
    UnionFind(int size){
        par=std::vector<int>(size);
        siz=std::vector<int>(size);
        for(int i=0;i<size;++i){
            par[i]=i;
            siz[i]=1;
        }
    }

    int root(int x){
        if(par[x]==x)return x;
        else {
            par[x]=root(par[x]);
            return par[x];
        }
    }

    int size(int x) {
        return siz[root(x)];
    }

    bool same(int x,int y){//0-indexed
        return root(x)==root(y);
    }

    void unite(int x,int y){//0-indexed
        x=root(x);
        y=root(y);
        if(x==y)return;

        if(siz[x]<siz[y]){
            par[x]=y;
            siz[y]=siz[x];
        }
        else{
            par[y]=x;
            siz[x]=siz[y];
        }
    }
};
//cut end
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
