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
    std::cout<<modpow(3,45,1000000007)<<std::endl;
    return 0;
}