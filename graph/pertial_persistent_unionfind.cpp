//https://atcoder.jp/contests/code-thanks-festival-2017-open/tasks/code_thanks_festival_2017_h
#include <iostream>
#include <vector>
#include <limits>
//cut begin
struct PPUnionFind{
private:
    private:
    std::vector<int> par;
    std::vector<int> time;
    std::vector<int> size;
public:
    PPUnionFind(int n){
        par=std::vector<int>(n);
        time=std::vector<int>(n);
        size=std::vector<int>(n);
        for(int i=0;i<n;++i){
            par[i]=i;
            time[i]=std::numeric_limits<int>::max();
            size[i]=1;
        }
    }

    int root(int t,int x){
        if(time[x]>t)return x;
        else return root(t,par[x]);
    }

    bool same(int t,int x,int y){//0-indexed
        return root(t,x)==root(t,y);
    }

    void unite(int t,int x,int y){//0-indexed
        x=root(t,x);
        y=root(t,y);
        if(x==y)return;

        if(size[x]<size[y]){
            par[x]=y;
            time[x]=t;
            size[y]+=size[x];
        }
        else{
            par[y]=x;
            time[y]=t;
            size[x]+=size[y];
        }
    }

    int whenMerged(int t,int x,int y){//0-indexed
        if(!same(t,x,y))return -2;
        int ng=-1,ok=t,mid;
        mid=(ng+ok)/2;
        while(ok-ng!=1){
            mid=(ng+ok)/2;
            if(same(mid,x,y))ok=mid;
            else ng=mid;
        }
        return ok;
    }
};
//cut end

void solve(){
    int n,m,q;
    std::cin>>n>>m;
    PPUnionFind uf(n);
    for (int i = 0; i < m; ++i){
        int x,y;
        std::cin>>x>>y;
        --x;--y;
        uf.unite(i,x,y);
    }
    std::cin>>q;
    for (int i = 0; i < q; i++)
    {
        int x,y;
        std::cin>>x>>y;
        --x;--y;
        std::cout<<uf.whenMerged(m,x,y)+1<<std::endl;
    }
}

int main(){
    solve();
    return 0;
}