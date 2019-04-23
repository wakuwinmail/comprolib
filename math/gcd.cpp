#include <iostream>

template<typename T>
T gcd(T a,T b){
    if(b==0)return a;
    return gcd(b,a%b);
}

void solve(){
    int a,b;
    std::cin>>a>>b;
    std::cout<<gcd(a,b)<<std::endl;
}
int main(){
    solve();
    return 0;
}