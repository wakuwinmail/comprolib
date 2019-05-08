#include <iostream>
#include <algorithm>
#include <cassert>
#include <functional>
#include <cstdio>
//cut begin
template<typename,typename>
class AVLTree;

template<typename T,typename E>
class avl_node{
private:
    typedef avl_node<T,E> node;
    T key;
    E value;
    node* left;
    node* right;
    int height;
    int size;
public:
    //template <typename,typename>
    friend class AVLTree<T,E>;
    friend void solve();
    explicit avl_node(T k,E v,int h){
        key=k;
        value=v;
        left=nullptr;
        right=nullptr;
        height=h;
        size=1;
    }
};

template<typename T,typename E>
class AVLTree{
private:
    typedef avl_node<T,E> node;
    typedef std::function<node*(node*,node*)> F;
    typedef std::function<node*(node*)> G;
    node* root;
    bool change;
    T lmaxkey;
    E lmaxvalue;
    int lmaxsize;
public:
    explicit AVLTree(){
        root=nullptr;
        change=false;
    }

private:
    int height(node* u){
        return u==nullptr?0:u->height;
    }

    int size(node* u){
        return u==nullptr?0:u->size;
    }

    void updateheight(node *u){
        u->height=std::max(height(u->left),height(u->right))+1;
    }

    node* rotateR(node *u){
        node* v=u->left;
        u->left=v->right;
        v->right=u;

        u->size-=size(v->left)+1;
        v->size+=size(u->right)+1;

        updateheight(u);
        updateheight(v);
        return v;
    }

    node* rotateL(node* u){
        node* v=u->right;
        u->right=v->left;
        v->left=u;

        u->size-=size(v->right)+1;
        v->size+=size(u->left)+1;

        updateheight(u);
        updateheight(v);
        return v;
    }

    node* rotateLR(node* u){
        u->left=rotateL(u->left);
        return rotateR(u);
    }

    node* rotateRL(node* u){
        u->right=rotateR(u->right);
        return rotateL(u);
    }

    int bias(node* u){
        int lh=0,rh=0;
        if(u->left!= nullptr)lh=u->left->height;
        if(u->right!= nullptr)rh=u->right->height;
        return lh-rh;
    }

    node* adjustL(node* u){//insert from left, erase from right
        if(!change)return u;
        int h=u->height;
        if(bias(u)==2){
            if(bias(u->left)>= 0)u=rotateR(u);
            else u=rotateLR(u);
        }
        updateheight(u);
        change=h!=u->height;
        return u;
    }

    node* adjustR(node* u){//insert from right, erase from left
        if(!change)return u;
        int h=u->height;
        //std::cout<<"in adjust2"<<std::endl;
        if(bias(u)==-2){
            if(bias(u->right) <= 0)u=rotateL(u);
            else u=rotateRL(u);
        }
        //std::cout<<"upd h not complete"<<std::endl;
        updateheight(u);
        change=h!=u->height;
        //std::cout<<"upd h complete"<<std::endl;
        return u;
    }
public:
    E* find(T k){
        node* u=root;
        while(u!=nullptr){
            if(u->key<k)u=u->right;
            else if(u->key>k)u=u->left;
            else return &u->value;
        }
        //std::cout<<"key:"<<k<<" is nullptr"<<std::endl;
        return nullptr;
    }

public:
    void insert(T k,E v){
        root=insert(root,k,v);
    }

private:
    node* insert(node* u,T k,E v){
        if(u==nullptr){
            change=true;
            //std::cout<<"key:"<<k<<" value:"<<v<<" inserted"<<std::endl;
            return new node(k,v,1);
        }
        else if(u->key<k){
            u->right=insert(u->right,k,v);
            ++u->size;
            return adjustR(u);
        }
        else if(u->key>k){
            u->left=insert(u->left,k,v);
            ++u->size;
            return adjustL(u);
        }
        else{
            change=false;
            u->value=v;
            return u;
        }
    }

public:
    void erase(T k){
        root=erase(root,k);
    }

private:
    node* erase(node* u,T k){
        if(u==nullptr){
            change=false;
            return nullptr;
        }
        else if(u->key<k){
            u->right=erase(u->right,k);
            --u->size;
            return adjustL(u);
        }
        else if(u->key>k){
            u->left=erase(u->left,k);
            --u->size;
            return adjustR(u);
        }
        else{
            if(u->left==nullptr){
                //std::cout<<"erase"<<std::endl;
                change=true;
                return u->right;
            }
            else{
                u->left=erasemax(u->left);
                u->key=lmaxkey;
                u->value=lmaxvalue;
                --u->size;
                //std::cout<<"lmaxsize:"<<lmaxsize<<std::endl;
                return adjustR(u);
            }
        }
    }

    node* erasemax(node* u){
        if(u->right!=nullptr){
            u->right=erasemax(u->right);
            --u->size;
            return adjustL(u);
        }
        else{
            change=true;
            lmaxkey=u->key;
            lmaxvalue=u->value;
            lmaxsize=u->size;
            return u->left;
        }
    }

public:
    T nth_min(int x=0){//return x-th minimum key, x is 0 indexed
        return nth_min(root,x)->key;
    }

private:
    node* nth_min(node* u,int x){
        //std::cout<<u->key<<" "<<u->size<<std::endl;
        assert(u!=nullptr);

        node* v=u->left;
        node* w=u->right;

        if(size(v)<x){//go right subtree
            return nth_min(w,x-1-size(v));
        }
        else if(size(v)>x){//go left subtree
            return nth_min(v,x);
        }
        else {
            return u;
        }
    }

public:
    node* range_query(T l,T r,F f){//[l,r) 2項演算(最小値など) 可換であるもの
        assert(root!=nullptr);
        return range_query(root,l,r,f);
    }

    node* range_query(T l,T r,G g){//[l,r) 単項演算
        return range_query(root,l,r,g);
    }
private:
    node* range_query(node* u,T l,T r,F f){
        if(u==nullptr)return nullptr;
        
        if(u->key<l)return range_query(u->right,l,r,f);
        else if(u->key>=r)return range_query(u->left,l,r,f);
        else{
            node* v=range_query(u->left,l,r,f);
            node* w=range_query(u->right,l,r,f);
            if(v==nullptr){
                if(w==nullptr)return u;
                else return f(u,w);
            }
            else if(w==nullptr){
                return f(u,v);
            }
            else return f(f(u,v),w);
        }
    }

    node* range_query(node* u,T l,T r,G g){
        if(u==nullptr)return nullptr;
        
        if(u->key<l)return range_query(u->right,l,r,g);
        else if(u->key>=r)return range_query(u->left,l,r,g);
        else{
            range_query(u->left,l,r,g);
            range_query(u->right,l,r,g);
            return g(u);
        }
    }
};
//cut end
void solve(){
    /*
    //https://atcoder.jp/contests/arc033/tasks/arc033_3
    int q;
    std::cin>>q;
    AVLTree<int,int> at;
    for(int i=0;i<q;++i){
        int t,x;
        std::cin>>t>>x;
        if(t==1){
            at.insert(x,x);
        }
        else {
            int y=at.nth_min(x-1);
            std::cout<<y<<std::endl;
            at.erase(y);
        }
    }
    */

    
    //http://judge.u-aizu.ac.jp/onlinejudge/description.jsp?id=1508
    int n,q;
    std::cin>>n>>q;
    AVLTree<int,int> at;
    for(int i = 0; i < n; ++i)
    {
        int a;
        scanf("%d",&a);
        at.insert(i,a);
    }
    
    for(int i = 0; i < q; ++i)
    {
        int x,l,r;
        scanf("%d%d%d",&x,&l,&r);
        if(x==0){
            int* v=at.find(r);
            int t=*v;
            at.erase(r);
            at.range_query(l,r,[](avl_node<int,int>* a){
                    ++(a->key);
                    //std::cout<<a->key<<" "<<a->value<<std::endl;
                    return a;
            });
            //std::cout<<l<<" "<<t<<std::endl;
            at.insert(l,t);
        }
        else if(x==1){
            auto ret=at.range_query(l,r+1,[](avl_node<int,int>* a,avl_node<int,int>* b){return a->value<b->value ? a : b;});
            assert(ret!=nullptr);
            printf("%d\n",ret->value);
        }
        else{
            at.insert(l,r);
        }
    }
    
    

    /*
    for(int i=0;i<q;++i){
        char c;
        std::cin>>c;
        if(c=='i'){
            int x,y;
            std::cin>>x>>y;
            at.insert(x,y);
        }
        else if(c=='e'){
            int x;
            std::cin>>x;
            at.erase(x);
        }
        else if(c=='f'){
            int x;
            std::cin>>x;
            auto pt=at.find(x);
            std::cout<<"key:"<<x<<" value:"<<*pt<<std::endl;
        }
    }
    */
    

}

int main(){
    solve();
    return 0;
}