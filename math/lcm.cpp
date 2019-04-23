#include <iostream>

template<typename T>
T gcd(T a,T b){
    if(b==0)return a;
    return gcd(b,a%b);
}

//GCDが必要です
//cut begin
template<typename T>
T lcm(T a,T b){
    T d=gcd(a,b);
    return (a/d)*b;
}
//cut end

void solve(){
    int a,b;
    std::cin>>a>>b;
    std::cout<<lcm(a,b)<<std::endl;
}

int main(){
    solve();
    return 0;
}