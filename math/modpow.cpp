//https://atcoder.jp/contests/atc002/tasks/atc002_b
#include <iostream>
//cut begin
template<typename T>
T modpow(T a,T n,T mod){
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