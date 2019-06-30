std::vector<long long> fac;
std::vector<long long> ifac;

template <typename T>
T modcomb(T a, T b,T MOD=1000000007){
    if(a == 0 && b == 0)return 1;
    if(a < b || a < 0)return 0;
    long long tmp = ifac[a-b]* ifac[b] % MOD;
    return tmp * fac[a] % MOD;
}

void combinit(int maxn){
    fac.resize(maxn);
    ifac.resize(maxn);
    fac[0] = 1;
    ifac[0] = 1;
    for(long long i = 0; i<maxn; i++){
        fac[i+1] = fac[i]*(i+1) % MOD;
        ifac[i+1] = ifac[i]*modpow(i+1, MOD-2) % MOD;
    }
}