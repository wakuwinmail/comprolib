//https://en.wikipedia.org/wiki/Median_of_medians
#include <vector>

template<typename V>
V select(std::vector<V>,int,int,int);

template<typename V>
V pivot(std::vector<V>,int,int);



//dataの中からk番目に大きい要素を計算する
//探索範囲は[start,end]
template<typename V>
V select(std::vector<V> data,int k,int start,int end){
    int pivotIndex=-1;
    while(pivotIndex!=k){
        V pivot=pivot(data,start,end);

        pivotIndex=partition(data,start,end,pivot);

        if(pivotIndex<k){
            start=pivotIndex+1;
        }
        else if(pivotIndex>k){
            end=pivotIndex-1;
        }
    }
    return data[k];
}

template<typename V>
V partition(std::vector<V> data,int start,int end,int pivot){
    V pivotValue=data[pivot];

}

//dataの[start,end]からpivot値を計算する
template<typename V>
V pivot(std::vector<V> data,int start,int end){
    std::vector<V> medians;

    for(int i=start;i<end;i+=5){
        int substart=i;
        int subend=i+4<end?end:i+4;//max(i+4,end)

        V median=median5(data,substart,subend);

        int j=(i-start)/5;
        medians[j]=median;
    }

    int n=(end-start+(5-1))/5;
    start=0;
    end=n-1;
    int k=n/2;
    return select(medians,k,start,end);
}