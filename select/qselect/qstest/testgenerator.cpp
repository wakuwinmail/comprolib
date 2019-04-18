#include <fstream>
#include <vector>
#include <random>

long long xorshift(){
    static long long x= 88172645463325252ULL;
    x=x^(x<<7);
    return x=x^(x>>9);
}

int main(){
    int n=100000000,q=1;
    int rand_min=0,rand_max=1000000000;
    std::ofstream output("testcase.in");
    output<<n<<" "<<q<<"\n";

    for(int i=0;i<n;++i){
        output<<rand_min+xorshift()%rand_max<<"\n";
    }
    output<<n/2;
    output.close();
    return 0;
}