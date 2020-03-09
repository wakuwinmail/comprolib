//https://atcoder.jp/contests/atc002/tasks/atc002_b
#include <iostream>
//cut begin
long long mod=1000000007LL;

template<typename T>
constexpr T modpow(T a,int n){//(a^n)%MOD
    T ret=1;
    while(n>0){
        if((n&1)!=0){//n%2==1
            ret=ret*a%mod;
        }
        a=a*a%mod;
        n=n/2;
    }
    return ret;
}
//cut end
int main(){
    long long n,m,p;
    std::cin>>n>>m>>p;
    std::cout<<modpow(n,p,m)<<std::endl;
    return 0;
}
