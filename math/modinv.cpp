#include <utility>

//long long mod=1000000007LL;

template<typename T>
T modinv(T a){
    T b=mod,u=1,v=0;
    while(b!=0){
        T t=a/b;
        a-=t*b;
        std::swap(a,b);
        u-=t*v;
        std::swap(u,v);
    }
    u%=mod;
    if(u<0)u+=mod;
    return u;
}
