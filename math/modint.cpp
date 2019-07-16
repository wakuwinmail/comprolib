#include<cstdint>
#include<iostream>

template<std::uint64_t Mod> struct ModInt{
    using u64=uint64_t;
private:
    u64 a;
public:
    constexpr ModInt(){}
    constexpr ModInt(u64 x=0):a(x%Mod){}

    constexpr u64 &value(){return a;}
    constexpr const u64 &value()const{return a;}


    constexpr ModInt &operator+=(const ModInt rhs){
        a+=rhs.value();
        if(a>=Mod){
            a-=Mod;
        }
        return *this;
    }

    constexpr ModInt &operator-=(const ModInt rhs){
        if(a<rhs.value()){
            a+=Mod;
        }
        a-=rhs.value();
        return *this;
    }

    constexpr ModInt &operator*=(const ModInt rhs){
        a=a*rhs.value()%Mod;
        return *this;
    }

    constexpr ModInt &operator/=(ModInt rhs){
        u64 n=Mod-2;
        while(n>0){
        if((n&1)!=0){//n%2==1
            *this*=rhs;
        }
        rhs*=rhs;
        n=n/2;
        }
        return *this;
    }

    constexpr ModInt operator+(const ModInt rhs){
        return ModInt(*this)+=rhs;
    }

    constexpr ModInt operator-(const ModInt rhs){
        return ModInt(*this)-=rhs;
    }

    constexpr ModInt operator*(const ModInt rhs){
        return ModInt(*this)*=rhs;
    }

    constexpr ModInt operator/(const ModInt rhs){
        return ModInt(*this)/=rhs;
    }
    constexpr ModInt &operator++(){
        ++a;
        if(a>=Mod){
            a-=Mod;
        }
        return *this;
    }

    constexpr ModInt &operator--(){
        if(a==0){
            a+=Mod;
        }
        --a;
        return *this;
    }

    
};

int main(){
    using Mi=ModInt<1000000007>;
    Mi mi(2);
    mi+=Mi(4);
    std::cout<<mi.value()<<std::endl;
    mi-=Mi(5);
    std::cout<<mi.value()<<std::endl;
    mi*=Mi(8);
    std::cout<<mi.value()<<std::endl;
    mi/=Mi(2);
    std::cout<<mi.value()<<std::endl;
}