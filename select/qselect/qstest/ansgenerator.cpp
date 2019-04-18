#include <cstdio>
#include <vector>
#include <algorithm>

int main(){
    int n,q;
    scanf("%d %d",&n,&q);

    std::vector<int> a(n);
    for(int i=0;i<n;++i){
        scanf("%d",&a[i]);
    }
    for(int i=0;i<q;++i){
        int k;
        scanf("%d",&k);
        --k;
        std::nth_element(a.begin(),a.begin()+k,a.end());
        printf("%d\n",a[k]);
    }
    return 0;
}