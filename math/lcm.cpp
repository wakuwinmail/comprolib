#include "gcd.cpp"
//GCDが必要
template<typename T>
T lcm(T a,T b){
    T d=gcd(a,b);
    return (a/d)*b;
}