#include <iostream>
#include <algorithm>

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
    explicit avl_node(T k,E v,int h){
        key=k;
        value=v;
        left=nullptr;
        right=nullptr;
        height=h;
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

public:
    explicit AVLTree(){
        root=nullptr;
        change=false;
    }

private:
    void updateheight(node *u){
        int lh=0,rh=0;
        if(u->left!= nullptr)lh=u->left->height;
        if(u->right!= nullptr)rh=u->right->height;
        u->height=std::max(lh,rh)+1;
    }

    node* rotateR(node *u){
        node* v=u->left;
        u->left=v->right;
        v->right=u;
        updateheight(u);
        updateheight(v);
        return v;
    }

    node* rotateL(node* u){
        node* v=u->right;
        u->right=v->left;
        v->left=u;
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
            return adjustL(u);
        }
        else if(u->key>k){
            u->left=insert(u->left,k,v);
            return adjustR(u);
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
            u->left=erase(u->left,k);
            return adjustR;
        }
        else if(u->key>k){
            u->right=erase(u->right,k);
            return adjustL;
        }
        else{
            if(u->left==nullptr){
                change=true;
                return u->right;
            }
            else{
                u->left=erasemax(u->left);
                u->key=lmaxkey;
                u->value=lmaxvalue;
                return adjustR(u);
            }
        }
    }

    node* erasemax(node* u){
        if(u->right){
            u->right=erasemax(u->right);
            return adjustL(u);
        }
        else{
            change=true;
            lmaxkey=u->key;
            lmaxvalue=u->value;
            return u->left;
        }
    }
};

void solve(){
    int q;
    //std::cin>>q;
    AVLTree<int,int> at;
    while(true){
        char c;
        std::cin>>c;
        if(c=='i'){
            int k,v;
            std::cin>>k>>v;
            at.insert(k,v);
        }
        else if(c=='f'){
            int k;
            std::cin>>k;
            auto it=at.find(k);
            if(it==nullptr){
                std::cout<<"key:"<<k<<" is not exist"<<std::endl;
            }
            else{
                std::cout<<"key:"<<k<<" value:"<<*it<<std::endl;
            }
        }
    }
}

int main(){
    solve();
    return 0;
}