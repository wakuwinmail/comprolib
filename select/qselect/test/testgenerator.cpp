#include <fstream>
#include <vector>
#include <random>
#include <algorithm>

long long xorshift(){
    static long long x= 88172645463325252ULL;
    x=x^(x<<7);
    return x=x^(x>>9);
}

int main(){
    int n=100,q=1;
    int rand_min=0,rand_max=1000000000;
    std::ofstream output("testcase.in");
    output<<n<<" "<<q<<"\n";
    
    std::vector<int> a(n);
    std::iota(a.begin(),a.end(),0);
    std::random_device seed_gen;
    std::mt19937 engine(seed_gen());
    std::shuffle(a.begin(), a.end(), engine);
    for(auto i:a)output<<i<<"\n";

    //for(int i=0;i<n;++i){
        //output<<rand_min+xorshift()%rand_max<<"\n";
    //}

    output<<n/2<<"\n";
    output.close();
    return 0;
}