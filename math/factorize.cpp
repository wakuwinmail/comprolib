//https://onlinejudge.u-aizu.ac.jp/courses/library/6/NTL/1/NTL_1_A
#include <iostream>

void solve(){
    int n;
    std::cin>>n;
    std::cout<<n<<":";
    int t=n;
    for(int i=2;i*i<= n; ++i){
        while(t%i==0){
            std::cout<<" "<<i;
            t=t/i;
        }
    }
    if(t==n)std::cout<<" "<<n;
    else if(t!=1)std::cout<<" "<<t;

    std::cout<<std::endl;
}

int main(){
    solve();
    return 0;
}