//https://onlinejudge.u-aizu.ac.jp/courses/library/3/DSL/2/DSL_2_E
#include <iostream>
#include <vector>
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
    vector<T> init;
    E opinit;
    F f;//function for merge
    G g;//function for update
    H h;//function for evaluate
    P p;//function for range calculate
    vector<T> node;
    vector<E> lazy;
public:
    explicit LazySegmentTree(//配列で初期化する場合
            int sz,
            F cal,
            G upd,
            H ecal,
            P rcal=[](T a,int b){return a;},
            vector<T> initv=vector<T>(1,0),
            E opinitv=0
    ){
        n=1;
        f=cal;
        g=upd;
        h=ecal;
        p=rcal;
        init=initv;
        opinit=opinitv;
        while(n<sz)n=n*2;
        node.resize(static_cast<unsigned int>(2 * n - 1), init[0]);//はみ出る分は0番地に指定
        for (int i = 0; i <sz ; ++i) node[i+n-1]=init[i+1];
        for (int i = n-2; i >= 0 ; --i) node[i]=f(node[2*i+1],node[2*i+2]);

        lazy.resize(static_cast<unsigned int>(2 * n - 1), opinit);
        for (int i = 0; i <sz ; ++i) lazy[i+n-1]=opinit;
        for (int i = n-2; i >= 0 ; --i) lazy[i]=h(lazy[2*i+1],lazy[2*i+2]);
    }

    explicit LazySegmentTree(//特定の要素で初期化する場合
            int sz,
            F cal,
            G upd,
            H ecal,
            P rcal=[](T a,int b){return a;},
            T initv,
            E opinitv=0
    ){
        n=1;
        f=cal;
        g=upd;
        h=ecal;
        p=rcal;
        init=initv;
        opinit=opinitv;
        while(n<sz)n=n*2;
        node.resize(static_cast<unsigned int>(2 * n - 1), init);
        for (int i = 0; i <sz ; ++i) node[i+n-1]=init;
        for (int i = n-2; i >= 0 ; --i) node[i]=f(node[2*i+1],node[2*i+2]);

        lazy.resize(static_cast<unsigned int>(2 * n - 1), opinit);
        for (int i = 0; i <sz ; ++i) lazy[i+n-1]=opinit;
        for (int i = n-2; i >= 0 ; --i) lazy[i]=h(lazy[2*i+1],lazy[2*i+2]);
    }

    void update(int p,int q,E val,int k=0,int l=0,int r=-1){//[p,q):0-indexed
        if(r<0)r=n;
        eval(k,r-l);
        if(r<=p||l>=q)return;
        if(p<=l&&r<=q){
            lazy[k]=h(lazy[k],val);
            eval(k,r-l);
        }
        else{
            update(p,q,val,2*k+1,l,(l+r)/2);
            update(p,q,val,2*k+2,(l+r)/2,r);
            node[k]=f(node[2*k+1],node[2*k+2]);
        }
    }

    T query(int p,int q,int k=0,int l=0,int r=-1){//[p,q):0-indexed
        if(r<0)r=n;
        if(r<=p||l>=q)return init[0];

        eval(k,r-l);
        if(p<=l&&r<=q)return node[k];
        T vl=query(p,q,2*k+1,l,(l+r)/2);
        T vr=query(p,q,2*k+2,(l+r)/2,r);
        return f(vl,vr);
    }

    void eval(int k,int len){//k:0-indexed
        if(lazy[k]==opinit)return;
        node[k]=g(node[k],p(lazy[k],len));
        if(k<n-1){
            lazy[2*k+1]=h(lazy[2*k+1],lazy[k]);
            lazy[2*k+2]=h(lazy[2*k+2],lazy[k]);
        }
        lazy[k]=opinit;
    }
};

void solve(){
    int n,q;
    std::cin>>n>>q;
    LazySegmentTree<long,long> st(n,
        std::plus<long>(),
        std::plus<long>(),
        std::plus<long>(),
        std::multiplies<long>(),
        0L,
        0L
    );
    for(size_t i = 0; i < q; i++)
    {
        int com,x,p,q;
        std::cin>>com;
        if(com==0)//add
        {
            std::cin>>p>>q>>x;
            st.update(p-1,q,x);
        }
        else //get
        {
            std::cin>>x;
            std::cout<<st.query(x-1,x)<<std::endl;
        }
    }
}

int main(){
    solve();
    return 0;
}