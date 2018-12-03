//https://onlinejudge.u-aizu.ac.jp/courses/library/3/DSL/all/DSL_2_A
#include <iostream>
#include <vector>
#include <algorithm>
#include <cstdint>

struct SegmentTree{
private:
    int n;
    std::vector<int> node;
 
public:
    explicit SegmentTree(int sz){
        n=1;
        while(n<sz)n=n*2;
        node.resize(static_cast<unsigned int>(2 * n - 1), INT32_MAX);
        for (int i = 0; i <sz ; ++i) node[i+n-1]=INT32_MAX;
        for (int i = n-2; i >= 0 ; --i) node[i]=std::min(node[2*i+1],node[2*i+2]);
    }
 
    void update(int x, int val){//x:0-indexed
        x+=n-1;
        node[x]=val;
        while(x>0){
            x=(x-1)/2;
            node[x]=std::min(node[2*x+1],node[2*x+2]);
        }
    }
 
    int getmin(int p,int q,int k=0,int l=0,int r=-1){//[p,q):0-indexed
        if(r<0)r=n;
        if(r<=p||l>=q)return INT32_MAX;
        if(p<=l&&r<=q)return node[k];
 
        return std::min(getmin(p,q,2*k+1,l,(l+r)/2),getmin(p,q,2*k+2,(l+r)/2,r));
    }
};

void solve(){
    int n,q;
    std::cin>>n>>q;
    SegmentTree st(n);
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
            std::cout<<st.getmin(x,y+1)<<std::endl;
        }
    }
}

int main(){
    solve();
    return 0;
}