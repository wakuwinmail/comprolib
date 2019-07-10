//https://onlinejudge.u-aizu.ac.jp/courses/lesson/1/ALDS1/14/ALDS1_14_B
#include <string>
#include <vector>
#include <tuple>
#include <iostream>
#include <set>
#include <cassert>

struct HashString{
public:
    using hashtype=long long;
    using hashcomptype=std::tuple<hashtype,hashtype,hashtype>;

    std::vector<hashtype> table1;
    std::vector<hashtype> table2;
    std::vector<hashtype> table3;
    std::vector<hashtype> powtable1;
    std::vector<hashtype> powtable2;
    std::vector<hashtype> powtable3;

    hashtype mod=1000000007ll;
    hashtype base1=1919ll;
    hashtype base2=114514ll;
    hashtype base3=334334ll;

    HashString(){}
    HashString(std::string s){
        table1.resize(s.size()+1);
        table2.resize(s.size()+1);
        table3.resize(s.size()+1);
        powtable1.resize(s.size()+1);
        powtable2.resize(s.size()+1);
        powtable3.resize(s.size()+1);

        table1[0]=0ll;
        table2[0]=0ll;
        table3[0]=0ll;
        powtable1[0]=1ll;
        powtable2[0]=1ll;
        powtable3[0]=1ll;

        for(int i=0;i < s.size(); ++i){
            table1[i+1]=table1[i]*base1+s[i];
            table2[i+1]=table2[i]*base2+s[i];
            table3[i+1]=table3[i]*base3+s[i];
            powtable1[i+1]=powtable1[i]*base1;
            powtable2[i+1]=powtable2[i]*base2;
            powtable3[i+1]=powtable3[i]*base3;

            table1[i+1]=table1[i+1]%mod;
            table2[i+1]=table2[i+1]%mod;
            table3[i+1]=table3[i+1]%mod;
            powtable1[i+1]=powtable1[i+1]%mod;
            powtable2[i+1]=powtable2[i+1]%mod;
            powtable3[i+1]=powtable3[i+1]%mod;
        }
    }

    bool operator == (HashString s){
        if(this->table1.size()!=s.table1.size())return false;
        if(this->table1.back()!=s.table1.back())return false;
        if(this->table2.back()!=s.table2.back())return false;
        if(this->table3.back()!=s.table3.back())return false;
        return true;//maybe :)
    }

    hashcomptype get_hash(int l,int r){//[l,r)でのhash
        assert(r-l>0);

        hashtype hash1=table1[r]-(powtable1[r-l]*table1[l]%mod);
        while(hash1<0)hash1+=mod;
        hash1=hash1%mod;

        hashtype hash2=table2[r]-(powtable2[r-l]*table2[l]%mod);
        while(hash2<0)hash2+=mod;
        hash2=hash2%mod;

        hashtype hash3=table3[r]-(powtable3[r-l]*table3[l]%mod);
        while(hash3<0)hash3+=mod;
        hash3=hash3%mod;

        //std::cout<<hash1<<" "<<hash2<<" "<<hash3<<std::endl;
        return std::make_tuple(hash1,hash2,hash3);
    }
};

int main(){
    std::string t,p;
    std::cin>>t>>p;
    HashString ht(t);
    HashString hp(p);
    
    /*
    int l1,r1,l2,r2;
    std::cin>>l1>>r1>>l2>>r2;
    std::cout<<t.substr(l1,r1-l1)<<" "<<p.substr(l2,r2-l2)<<std::endl;
    if(ht.get_hash(l1,r1)==hp.get_hash(l2,r2))std::cout<<"same"<<std::endl;
    else std::cout<<"different"<<std::endl;
    */
    
    
    for(int i=0;i+p.size() <= t.size(); ++i){
        if(ht.get_hash(i,i+p.size())==hp.get_hash(0,p.size())){
            std::cout<<i<<std::endl;
        }
    }
    
    
    return 0;
}
