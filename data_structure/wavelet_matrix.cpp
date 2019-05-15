#include <iostream>
#include <vector>
#include <unordered_map>
#include <limits>

size_t pow2(int n){
    size_t ret=1;
    size_t a=2;
    while(n>0){
        if((n&1)!=0){//n%2==1
            ret=ret*a;
        }
        a=a*a;
        n=n/2;
    }
    return ret;
}

//cut begin
template<typename T>
struct WaveletMatrix{
private:
    std::vector<std::vector<short>> bitmatrix;
    std::vector<std::vector<int>> bitsum;
    std::vector<int> bitcnt;
    std::unordered_map<T,int> startind;
public:
    explicit WaveletMatrix(std::vector<T> &init,size_t maxv=std::numeric_limits<size_t>::max()){
        int maxdigit=0;
        int n=init.size();
        int bcnt=0;
        while(maxv>0){
            if(maxv%2==1)--maxv;
            maxv=maxv/2;
            ++maxdigit;
        }
        bitmatrix.resize(maxdigit);
        bitsum.resize(maxdigit);
        bitmatrix[0].resize(n);
        bitsum[0].assign(n+1,0);
        bitcnt.resize(maxdigit);
        std::vector<int> pbit0;
        std::vector<int> pbit1;
        for (int i = 0; i < n; i++){
            if(init[i]&(1<<(maxdigit-1))){
                bitmatrix[0][i]=1;
                pbit1.emplace_back(i);
            }
            else {
                bitmatrix[0][i]=0;
                pbit0.emplace_back(i);
                ++bcnt;
            }
            bitsum[0][i+1]=bitsum[0][i]+bitmatrix[0][i];
        }
        bitcnt[0]=bcnt;
        for (int i = maxdigit-1; i > 0; --i){
            bcnt=0;
            bitsum[maxdigit-i].assign(n+1,0);
            std::vector<int> tbit0,tbit1;
            for (int j = 0; j < pbit0.size(); j++){
                int ind=pbit0[j];
                int tb=init[ind]&(1<<(i-1));
                bcnt+=(tb==0?1:0);
                if(tb)tbit1.emplace_back(ind);
                else tbit0.emplace_back(ind);
                bitmatrix[maxdigit-i].emplace_back(tb==0?0:1);
                bitsum[maxdigit-i][j+1]=bitsum[maxdigit-i][j]+bitmatrix[maxdigit-i][j];
            }
            for (int j = 0; j < pbit1.size(); j++){
                int ind=pbit1[j];
                int tb=init[ind]&(1<<(i-1));
                bcnt+=(tb==0?1:0);
                if(tb)tbit1.emplace_back(ind);
                else tbit0.emplace_back(ind);
                bitmatrix[maxdigit-i].emplace_back(tb==0?0:1);
                bitsum[maxdigit-i][pbit0.size()+j+1]=bitsum[maxdigit-i][pbit0.size()+j]+bitmatrix[maxdigit-i][pbit0.size()+j];
            }
            pbit0=tbit0;pbit1=tbit1;
            bitcnt[maxdigit-i]=bcnt;
        }
        startind[pbit0[0]]=0;
        for (int i = 1; i < pbit0.size(); i++){
            if(pbit0[i-1]==pbit0[i])continue;
            startind[pbit0[i]]=i;
        }
        startind[pbit1[0]]=pbit0.size();
        for (int i = 1; i < pbit1.size(); i++){
            if(pbit1[i-1]==pbit1[i])continue;
            startind[pbit1[i]]=i+pbit0.size();
        }
    }

    T access(int ind){
        int ret=0;
        int maxdigit=bitmatrix.size();
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

    int rank(int t,T c){//t:0-indexed,[0,t]

    }

};

//cut end
void solve(){
    int n;
    std::cin>>n;
    std::vector<int> a(n);
    for(int i=0;i<n;++i)std::cin>>a[i];
    WaveletMatrix<int> wm(a,6);
    int q,k;
    std::cin>>q;
    for(int i=0;i<q;++i){
        std::cin>>k;
        std::cout<<wm.access(k)<<std::endl;
    }
}

int main(){
    solve();
    return 0;
}