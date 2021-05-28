#include <cmath>
#include <cstddef>
#include <vector>
#include <cassert>

template<typename T>
struct Matrix{
    using scalarvalue=T;
private:
    std::vector<std::vector<scalarvalue>> data;
    int col,row;
public:
    constexpr Matrix(){}

    constexpr Matrix(std::vector<std::vector<scalarvalue>> data):data(data){
        col=data.size();
        row=data[0].size();
    }

    constexpr Matrix(int col,int row,scalarvalue init=0):row(row),col(col){
        data.assign(col,std::vector<scalarvalue>(row,init));
    }

    constexpr Matrix &operator+=(const Matrix rhs){
        for(int i = 0; i < col; ++i){
            for(int j = 0; j < row; ++j){
                data[i][j]+=rhs[i][j];
            }
        }
    }

    constexpr Matrix &operator-=(const Matrix rhs){
        for(int i = 0; i < col; ++i){
            for(int j = 0; j < row; ++j){
                data[i][j]-=rhs[i][j];
            }
        }
    }

    constexpr Matrix &operator*=(const scalarvalue rhs){//スカラー倍
        for(int i = 0; i < col; ++i){
            for(int j = 0; j < row; ++j){
                data[i][j]*=rhs;
            }
        }
    }

    constexpr Matrix operator*(Matrix rhs){
        Matrix<scalarvalue> ret(col,rhs.get_row(),0);
        for(int i = 0; i < col; ++i){
            for(int k = 0; k < rhs.get_row(); ++k){
                for(int j = 0; j < row; ++j){
                    ret[i][j]+=data[i][k]*rhs[k][j];
                }
            }
        }
        return ret;
    }

    constexpr std::vector<scalarvalue>& operator[](const int i){
        return data[i];
    }

    constexpr Matrix pow(long long n){
        assert(is_square());//正方形じゃないと計算が定義できない
        Matrix<scalarvalue> ret(col,row,0);
        Matrix<scalarvalue> a(*this);
        for(int i = 0; i < col; ++i){
            ret[i][i]=1;//単位行列を作る
        }
        while(n>0){
            if((n&1)!=0){//n%2==1
                ret=ret*a;
            }
            a=a*a;
            n=n/2;
        }
        return ret;
    }




    bool is_square(){return col==row;}
    int get_col(){return col;}
    int get_row(){return row;}

};
