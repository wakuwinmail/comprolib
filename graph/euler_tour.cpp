//https://onlinejudge.u-aizu.ac.jp/problems/2871
#include <iostream>
#include <vector>
#include <utility>

//cut begin
using Graph=std::vector<std::vector<int>>;//無向グラフ

struct EulerTour{
private:
    int sz;
    std::vector<int> in;
    std::vector<int> out;
    int ord;
    Graph g;

    void dfs(int v,int p){//0-indexed
        in[v]=ord;
        ++ord;
        for(int c:g[v]){
            if(c==p)continue;
            dfs(c,v);
        }
        out[v]=ord;
        ++ord;
    }
public:
    EulerTour(Graph g,int root=0):g(g){//initialize
        in.resize(g.size());
        out.resize(g.size());
        ord=0;
        dfs(root,-1);
    }

    std::pair<int,int> interval(int n){
        return std::make_pair(in[n],out[n]);
    }
};

//cut end

#include <algorithm>
#include <functional>

template <typename T,typename E>
struct LazySegmentTree{
private:
    typedef std::function<T(T,T)> F;
    typedef std::function<T(T,E)> G;
    typedef std::function<E(E,E)> H;
    typedef std::function<E(E,int)> P;
    int n;
    F cal;//function for merge
    G upd;//function for update
    H ecal;//function for evaluate
    P rcal;//function for range calculate
    std::vector<T> init;
    T initv;
    E opinit;
    std::vector<T> node;
    std::vector<E> lazy;
public:
    explicit LazySegmentTree(//配列で初期化する場合
            int sz,
            F cal,
            G upd,
            H ecal,
            P rcal=[](T a,int b){return a;},
            std::vector<T> init=std::vector<T>(1,0),
            E opinit=0
    ):
    cal(cal),
    upd(upd),
    ecal(ecal),
    rcal(rcal),
    init(init),
    opinit(opinit)
    {
        n=1;
        initv=init[0];
        while(n<sz)n=n*2;
        node.resize(static_cast<unsigned int>(2 * n - 1), init[0]);//はみ出る分は0番地に指定
        for (int i = 0; i <sz ; ++i) node[i+n-1]=init[i+1];
        for (int i = n-2; i >= 0 ; --i) node[i]=cal(node[2*i+1],node[2*i+2]);

        lazy.resize(static_cast<unsigned int>(2 * n - 1), opinit);
        for (int i = 0; i <sz ; ++i) lazy[i+n-1]=opinit;
        for (int i = n-2; i >= 0 ; --i) lazy[i]=ecal(lazy[2*i+1],lazy[2*i+2]);
    }

    void update(int p,int q,E val,int k=0,int l=0,int r=-1){//[p,q):0-indexed
        if(r<0)r=n;
        eval(k,r-l);
        if(r<=p||l>=q)return;
        if(p<=l&&r<=q){
            lazy[k]=ecal(lazy[k],val);
            eval(k,r-l);
        }
        else{
            update(p,q,val,2*k+1,l,(l+r)/2);
            update(p,q,val,2*k+2,(l+r)/2,r);
            node[k]=cal(node[2*k+1],node[2*k+2]);
        }
    }

    T query(int p,int q,int k=0,int l=0,int r=-1){//[p,q):0-indexed
        if(r<0)r=n;
        if(r<=p||l>=q)return initv;

        eval(k,r-l);
        if(p<=l&&r<=q)return node[k];
        T vl=query(p,q,2*k+1,l,(l+r)/2);
        T vr=query(p,q,2*k+2,(l+r)/2,r);
        return cal(vl,vr);
    }

    void eval(int k,int len){//k:0-indexed
        if(lazy[k]==opinit)return;
        node[k]=upd(node[k],rcal(lazy[k],len));
        if(k<n-1){
            lazy[2*k+1]=ecal(lazy[2*k+1],lazy[k]);
            lazy[2*k+2]=ecal(lazy[2*k+2],lazy[k]);
        }
        lazy[k]=opinit;
    }

    void dbg(){
        for(int i = 0; i < n-1; ++i){
            std::cout<<node[n+i-1]<<" ";
        }
        std::cout<<node.back();
    }
};

void solve(){
    int n,q;
    std::cin>>n>>q;
    std::vector<std::vector<int>> G(n);
    for(int i = 1; i < n; ++i){
        int p;
        std::cin>>p;
        --p;
        G[p].emplace_back(i);
    }
    EulerTour et(G);
    std::vector<long> val(n*2+1,0);
    for(int i = 0; i < n; ++i){
        char c;
        std::cin>>c;
        if(c=='G'){
            auto t=et.interval(i);
            val[t.first+1]=1;
            val[t.second+1]=1;
        }
    }

    LazySegmentTree<long,int> st(
        n*2,
        std::plus<long>(),
        [](long a,int b)->long{return b==0?a:b-a;},
        std::plus<int>(),
        [](int a,int b)->int{return a%2==0?0:b;},
        val
    );
    for(int i = 0; i < q; ++i){
        int v;
        std::cin>>v;
        --v;
        auto t=et.interval(v);
        st.update(t.first,t.second+1,1);
        if(st.query(0,2*n+1)>n)std::cout<<"broccoli"<<std::endl;
        else std::cout<<"cauliflower"<<std::endl;
    }
    

}

int main(){
    solve();
    return 0;
}