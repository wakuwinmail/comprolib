//https://onlinejudge.u-aizu.ac.jp/courses/library/3/DSL/2/DSL_2_E
#include <iostream>
#include <vector>
#include <algorithm>

template <typename T>
struct LazySegmentTree{
private:
    int n;
    T init;
    std::vector<T> node;
    std::vector<T> lazy;
public:
    explicit LazySegmentTree(int sz,T initv=0){
        n=1;
        init=initv;
        while(n<sz)n=n*2;
        node.resize(static_cast<unsigned int>(2 * n - 1), init);
        for (int i = 0; i <sz ; ++i) node[i+n-1]=init;
        for (int i = n-2; i >= 0 ; --i) node[i]=node[2*i+1]+node[2*i+2];

        lazy.resize(static_cast<unsigned int>(2 * n - 1), init);
        for (int i = 0; i <sz ; ++i) lazy[i+n-1]=init;
        for (int i = n-2; i >= 0 ; --i) lazy[i]=lazy[2*i+1]+lazy[2*i+2];
    }

    void add(int p,int q,T val,int k=0,int l=0,int r=-1){//[p,q):0-indexed
        if(r<0)r=n;
        update(k);
        if(r<=p||l>=q)return;
        if(p<=l&&r<=q){
            lazy[k]+=val*(r-l);
            update(k);
        }
        else{
            add(p,q,val,2*k+1,l,(l+r)/2);
            add(p,q,val,2*k+2,(l+r)/2,r);
            node[k]=node[2*k+1]+node[2*k+2];
        }
    }

    T getsum(int p,int q,int k=0,int l=0,int r=-1){//[p,q):0-indexed
        if(r<0)r=n;
        if(r<=p||l>=q)return 0;

        update(k);
        if(p<=l&&r<=q)return node[k];
        T vl=getsum(p,q,2*k+1,l,(l+r)/2);
        T vr=getsum(p,q,2*k+2,(l+r)/2,r);
        return vl+vr;
    }

    void update(int k){//k:0-indexed
        if(lazy[k]!=0){
            node[k]+=lazy[k];
            if(k<n-1){
                lazy[2*k+1]+=lazy[k]/2;
                lazy[2*k+2]+=lazy[k]/2;
            }
            lazy[k]=0;
        }
    }
};

void solve(){
    int n,q;
    std::cin>>n>>q;
    LazySegmentTree<long> st(n);
    for(size_t i = 0; i < q; i++)
    {
        int com,x,p,q;
        std::cin>>com;
        if(com==0)//add
        {
            std::cin>>p>>q>>x;
            st.add(p-1,q,x);
        }
        else //get
        {
            std::cin>>x;
            std::cout<<st.getsum(x-1,x)<<std::endl;
        }
    }
}

int main(){
    solve();
    return 0;
}