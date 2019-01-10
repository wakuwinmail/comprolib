#include <iostream>
#include <algorithm>
#include <cassert>

template<typename T,typename E>
struct avl_node{
private:
    typedef avl_node<T,E> node;

public:
    T key;
    E value;
    node* left;
    node* right;
    int height;
    int size;
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
struct AVLTree{
private:
    typedef avl_node<T,E> node;
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
                u->size=lmaxsize;
                u->size+=size(u->right);
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
    T min(int x=0){//return x-th minimum key
        return min(root,x)->key;
    }

private:
    node* min(node* u,int x){
        std::cout<<u->key<<" "<<u->size<<std::endl;
        assert(u!=nullptr);

        node* v=u->left;
        node* w=u->right;

        if(size(v)<x){//go right subtree
            return min(w,x-1-size(v));
        }
        else if(size(v)>x){//go left subtree
            return min(v,x);
        }
        else {
            return u;
        }
    }
};

void solve(){
    int q;
    std::cin>>q;
    AVLTree<int,int> at;
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
    for(int i=0;i<q;++i){
        int t,x;
        std::cin>>t>>x;
        if(t==1){
            at.insert(x,x);
        }
        else {
            int y=at.min(x-1);
            std::cout<<y<<std::endl;
            at.erase(y);
        }
    }
}

int main(){
    solve();
    return 0;
}