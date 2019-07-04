//https://onlinejudge.u-aizu.ac.jp/courses/library/3/DSL/all/DSL_2_A
#include <iostream>
#include <vector>
#include <algorithm>
#include <cstdint>
#include <functional>
#include "monoid.cpp"

//cut begin
template <typename T,typename E=T>
struct SegmentTree{
private:
    using G=std::function<T(T,E)>;
    int n;
    std::vector<T> init;
    monoid<T> mono;
    G upd;//function for update
    std::vector<T> node;

public:
    explicit SegmentTree(
            int sz,
            monoid<T> mono,
            G upd
    ):mono(mono),upd(upd)
    {
        n=1;
        while(n<sz)n=n*2;
        node.resize(static_cast<unsigned int>(2 * n - 1), mono.get_id());
        for (int i = 0; i <sz ; ++i) node[i+n-1]=mono.get_id();
        for (int i = n-2; i >= 0 ; --i) node[i]=mono.product(node[2*i+1],node[2*i+2]);
    }

    explicit SegmentTree(
            int sz,
            monoid<T> mono,
            G upd,
            std::vector<T> init
    ):mono(mono),upd(upd),init(init)
    {
        n=1;
        while(n<sz)n=n*2;
        node.resize(static_cast<unsigned int>(2 * n - 1), mono.get_id());
        for (int i = 0; i <sz ; ++i) node[i+n-1]=init[i+1];
        for (int i = n-2; i >= 0 ; --i) node[i]=mono.product(node[2*i+1],node[2*i+2]);
    }

    void update(int x, E val){//x:0-indexed
        x+=n-1;
        node[x]=upd(node[x],val);
        while(x>0){
            x=(x-1)/2;
            node[x]=mono.product(node[2*x+1],node[2*x+2]);
        }
    }

    T query(int p,int q,int k=0,int l=0,int r=-1){//[p,q):0-indexed
        if(r<0)r=n;
        if(r<=p||l>=q)return mono.get_id();//out of range
        if(p<=l&&r<=q)return node[k];

        return mono.product(query(p,q,2*k+1,l,(l+r)/2),query(p,q,2*k+2,(l+r)/2,r));
    }
};
//cut end
void solve(){
    int n,q;
    std::cin>>n>>q;
    monoid<int> mono([](int a,int b){return std::min(a,b);},std::numeric_limits<int>::max());
    SegmentTree<int> st(n,
        mono,
        [](int a,int b){return b;}
    );
    for(size_t i = 0; i < q; i++)
    {
        int com,x,y;
        std::cin>>com>>x>>y;
        if(com==0)//update
        {
            st.update(x,y);
        }
        else //find
        {
            std::cout<<st.query(x,y+1)<<std::endl;
        }
    }
}

int main(){
    solve();
    return 0;
}