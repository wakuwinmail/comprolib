//http://judge.u-aizu.ac.jp/onlinejudge/description.jsp?id=2426
#include <iostream>
#include <vector>
#include <unordered_map>
#include <limits>
#include <algorithm>
#include <utility>
#include <map>
#include <cassert>

//cut begin
uintmax_t pow2(int n){
    uintmax_t ret=1;
    uintmax_t a=2;
    while(n>0){
        if((n&1)!=0){//n%2==1
            ret=ret*a;
        }
        a=a*a;
        n=n/2;
    }
    return ret;
}

template<typename T>
struct WaveletMatrix{
private:
    int valcnt;
    std::vector<std::vector<short>> bitmatrix;
    std::vector<std::vector<int>> bitsum;
    std::vector<std::vector<int>> bitselect0;
    std::vector<std::vector<int>> bitselect1;
    std::vector<int> bitcnt;
    std::vector<T> lastval;
    std::unordered_map<T,int> startind;
    int maxdigit;
public:
    explicit WaveletMatrix(std::vector<T> &init,uintmax_t maxv=std::numeric_limits<uintmax_t>::max()){
        maxdigit=0;
        int n=init.size();
        valcnt=n;
        int bcnt=0;
        while(maxv>0){
            if(maxv%2==1)--maxv;
            maxv=maxv/2;
            ++maxdigit;
        }
        bitmatrix.resize(maxdigit);
        bitsum.resize(maxdigit);
        bitselect0.resize(maxdigit);
        bitselect1.resize(maxdigit);

        bitmatrix[0].resize(n);
        bitsum[0].assign(n+1,0);
        bitselect0[0].emplace_back(0);
        bitselect1[0].emplace_back(0);

        bitcnt.resize(maxdigit);
        std::vector<int> pbit0;
        std::vector<int> pbit1;

        for (int i = 0; i < n; i++){
            if(init[i]&(1<<(maxdigit-1))){
                bitmatrix[0][i]=1;
                bitselect1[0].emplace_back(i);
                pbit1.emplace_back(i);
            }
            else {
                bitmatrix[0][i]=0;
                bitselect0[0].emplace_back(i);
                pbit0.emplace_back(i);
                ++bcnt;
            }
            bitsum[0][i+1]=bitsum[0][i]+bitmatrix[0][i];
        }
        bitcnt[0]=bcnt;
        for (int i = maxdigit-1; i > 0; --i){
            bcnt=0;
            bitsum[maxdigit-i].assign(n+1,0);
            bitselect0[maxdigit-i].emplace_back(0);
            bitselect1[maxdigit-i].emplace_back(0);
            std::vector<int> tbit0,tbit1;
            for (int j = 0; j < pbit0.size(); j++){
                int ind=pbit0[j];
                int tb=init[ind]&(1<<(i-1));
                bcnt+=(tb==0?1:0);
                if(tb){
                    bitselect1[maxdigit-i].emplace_back(j);
                    tbit1.emplace_back(ind);
                }
                else{
                    bitselect0[maxdigit-i].emplace_back(j);
                    tbit0.emplace_back(ind);
                }
                bitmatrix[maxdigit-i].emplace_back(tb==0?0:1);
                bitsum[maxdigit-i][j+1]=bitsum[maxdigit-i][j]+bitmatrix[maxdigit-i][j];
            }
            for (int j = 0; j < pbit1.size(); j++){
                int ind=pbit1[j];
                int tb=init[ind]&(1<<(i-1));
                bcnt+=(tb==0?1:0);
                if(tb){
                    bitselect1[maxdigit-i].emplace_back(j+pbit0.size());
                    tbit1.emplace_back(ind);
                }
                else{
                    bitselect0[maxdigit-i].emplace_back(j+pbit0.size());
                    tbit0.emplace_back(ind);
                }
                bitmatrix[maxdigit-i].emplace_back(tb==0?0:1);
                bitsum[maxdigit-i][pbit0.size()+j+1]=bitsum[maxdigit-i][pbit0.size()+j]+bitmatrix[maxdigit-i][pbit0.size()+j];
            }
            pbit0=tbit0;pbit1=tbit1;
            bitcnt[maxdigit-i]=bcnt;
        }
        lastval.emplace_back(init[pbit0[0]]);
        startind[init[pbit0[0]]]=0;
        for (int i = 1; i < pbit0.size(); i++){
            lastval.emplace_back(init[pbit0[i]]);
            if(init[pbit0[i-1]]==init[pbit0[i]])continue;
            startind[init[pbit0[i]]]=i;
        }
        lastval.emplace_back(init[pbit1[0]]);
        startind[init[pbit1[0]]]=pbit0.size();
        for (int i = 1; i < pbit1.size(); i++){
            lastval.emplace_back(init[pbit1[i]]);
            if(init[pbit1[i-1]]==init[pbit1[i]])continue;
            startind[init[pbit1[i]]]=i+pbit0.size();
        }
    }

    T access(int ind){
        int ret=0;
        int nind=ind;
        for (int i = maxdigit; i > 0; --i){
            ret+=pow2(i-1)*bitmatrix[maxdigit-i][nind];
            if(bitmatrix[maxdigit-i][nind]==0){
                nind=nind-bitsum[maxdigit-i][nind+1];
            }
            else nind=bitcnt[maxdigit-i]+bitsum[maxdigit-i][nind];
        }
        return ret;
    }

private:
    void bitcul(T c,std::vector<bool> &tbit){
        for (int i = maxdigit-1; i >= 0; --i){
            if(c%2==1){
                tbit[i]=true;
                --c;
            }
            c=c/2;
        }
    }

public:
    int rank(int t,T c){//[0,t]でcの個数,t:0-indexed
        std::vector<bool> tbit(maxdigit,false);
        bitcul(c,tbit);
        for (int i = 0; i < maxdigit; ++i){
            if(tbit[i])t=bitsum[i][t]+bitcnt[i];
            else t=t-bitsum[i][t];
            //std::cout<<"t:"<<t<<std::endl;
        }
        return t-startind[c];
    }

    int select(int t,T c){//t番目のc,0-indexed
        std::vector<bool> tbit(maxdigit,false);
        bitcul(c,tbit);
        t=startind[c]+t-1;
        for (int i = maxdigit-1; i >= 0; --i){
            std::cout<<t<<std::endl;
            if(tbit[i])t=bitselect1[i][t-bitcnt[i]+1];
            else t=bitselect0[i][t+1];
        }
        return t;
    }
public:
    T quantile(int l,int r,int k){//[l,r]で小さい方からk個目,0-indexed
        for (int i = 0; i < maxdigit; ++i){
            int bit0=r-l+1-(bitsum[i][r+1]-bitsum[i][l]);
            if(bit0<k){
                r=r+1-l-bit0;
                l=bitcnt[i]+bitsum[i][l];
                r+=l-1;
                k-=bit0;
            }
            else{
                l=l-bitsum[i][l];
                r=l+bit0-1;
            }
            //std::cout<<l<<" "<<r<<" "<<k<<std::endl;
        }
        return lastval[l+k-1];
    }

    int range_l(int l,int r,T v){//[l,r]でv以下のものの個数,0-indexed
        std::vector<bool> tbit(maxdigit,false);
        bitcul(v,tbit);
        //std::cout<<"OK"<<std::endl;
        int ret=0;
        //std::cout<<l<<" "<<r<<" "<<v<<std::endl;
        for(int i=0;i < maxdigit; ++i){
            //if(l==r)std::cout<<"hoge:"<<ret<<std::endl;
            if(r<l){
                //std::cout<<"hoge"<<std::endl;
                return ret;
            }
            int range=r+1-l;
            int bit1=bitsum[i][r+1]-bitsum[i][l];
            if(tbit[i]){//0は全部ok(1だけ残す)
                ret+=range-(bit1);//0のものは全部加算
                //std::cout<<"huga:"<<range<<" "<<bit1<<" "<<i<<std::endl;
                l=bitcnt[i]+bitsum[i][l]-bitsum[i][0];//0の個数+lより左にある1の個数
                r=l+bit1-1;//[l,r]にある1の個数
            }
            else{//1は全部ng(0だけ残す)
                l=l-(bitsum[i][l]-bitsum[i][0]);//lより左にある0の個数
                r=l+range-(bit1)-1;//[l,r]にある0の個数
            }
            //std::cout<<l<<" "<<r<<" "<<ret<<std::endl;
        }
        ret+=r+1-l;
        return ret;
    }

    int range_freq(int l,int r,T a,T b){//[l,r]で[a,b]の個数,l,r:0-indexed
        if(l<0)l=0;
        if(r>=valcnt)r=valcnt-1;
        int ret=range_l(l,r,b);
        std::cout<<ret<<std::endl;
        //std::cout<<"a:"<<a<<std::endl;
        ret-=range_l(l,r,a);
        std::cout<<ret<<std::endl;
        ret+=rank(r+1,a);
        std::cout<<ret<<std::endl;
        ret-=rank(l,a);
        return ret;
    }

public:
    void printmatrix(int k){//k段目を全部見る
        std::cout<<k<<std::endl;
        for(int i=0;i < valcnt; ++i){
            std::cout<<bitmatrix[k][i]<<" ";
        }
        std::cout<<std::endl;
    }
};

//cut end
void solve(){
    constexpr int ground=1000000000;
    int n,m;
    std::cin>>n>>m;
    std::vector<std::pair<int,int>> treasure(n);
    for(int i=0;i < n; ++i){
        int x,y;
        std::cin>>x>>y;
        treasure[i]=std::make_pair(x+ground,y+ground);
    }
    std::sort(treasure.begin(),treasure.end());
    std::vector<int> a(n);
    std::map<int,int> za;//F:x-pos,S:index
    for(int i=0;i < n; ++i){
        a[i]=treasure[i].second;
        za[treasure[i].first]=i;
        std::cout<<a[i]<<std::endl;
    }
    WaveletMatrix<int> wm(a,std::numeric_limits<int>::max());
    for(int i=0;i < m; ++i){
        int x1,y1,x2,y2;
        std::cin>>x1>>y1>>x2>>y2;
        x1+=ground;y1+=ground;x2+=ground;y2+=ground;
        //std::cout<<x1<<" "<<x2<<std::endl;
        auto itl=za.lower_bound(x1);
        auto itr=za.upper_bound(x2);
        --itr;
        int ta=itl->second,tb=itr->second;
        std::cout<<ta<<" "<<tb<<std::endl;
        //std::cout<<"OK"<<std::endl;
        std::cout<<wm.range_freq(ta,tb,y1,y2)<<std::endl;
        //wm.printmatrix(1);
    }
}

int main(){
    solve();
    return 0;
}

//111011100110101100101000000001
//111011100110101100101000000000