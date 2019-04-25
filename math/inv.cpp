#include <utility>

template<typename T>
T modinv(T a,T m=1000000007){
    T b=m,u=1,v=0;
    while(b!=0){
        T t=a/b;
        a-=t*b;
        std::swap(a,b);
        u-=t*v;
        std::swap(u,v);
    }
    u%=m;
    if(u<0)u+=m;
    return u;
}