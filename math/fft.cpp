//https://atcoder.jp/contests/atc001/tasks/fft_c
#include <vector>
#include <complex>
#include <iostream>

using dcomplex = std::complex<double>;
double pi = M_PI;

std::vector<dcomplex> dft(std::vector<dcomplex> f, int sign = 1) {
    int n = f.size();
    if(n==1)return f;

    std::vector<dcomplex> a(n/2), b(n/2);

    for(int i = 0; i < n/2; ++i) {
        a[i] = f[2*i];
        b[i] = f[2*i + 1];
    }

    a = dft(a, sign);
    b = dft(b, sign);
        
    for(int i = 0; i < n; ++i) {
        double x = (i * 2 * pi) / n;
        f[i] = a[i % (n/2)] + std::polar(1.0, -x * sign) * b[i % (n/2)];
    }

    return f;
}

std::vector<dcomplex> idft(std::vector<dcomplex> g) {
    int n = g.size();
    g = dft(g, -1);
    for(int i = 0; i < n; ++i) {
        g[i] /= n;
    }
    return g;
}

std::vector<dcomplex> multiply(std::vector<dcomplex> a, std::vector<dcomplex>b) {
    int p  = a.size();
    int q = b.size();
    int sz = 1;
    while(sz<p+q-1)sz*=2;

    a.resize(sz);
    b.resize(sz);

    a = dft(a);
    b = dft(b);

    std::vector<dcomplex> c(sz);
    for(int i = 0; i < sz; ++i) {
        c[i] = a[i] * b[i];
    }
    c = idft(c);
    return c;
}

int main() {
    int n;
    std::cin>>n;
    std::vector<dcomplex> f(n+1),g(n+1);
    for(int i = 1; i <= n; ++i) {
        double a,b;
        std::cin>>a>>b;
        f[i] = dcomplex(a, 0.0);
        g[i] = dcomplex(b, 0.0);
    }
    std::vector<dcomplex> ans = multiply(f, g);
    for(int i = 1; i <= n*2; ++i) {
        std::cout<<(int)(ans[i].real()+0.5)<<std::endl;
    }
    return 0;
}