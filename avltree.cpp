#include <iostream>
#include <algorithm>

template<typename T,typename E>
struct avl_node{
private:
    typedef avl_node<T,E> node;

public:
    T key;
    E value;
    node* parent;
    node* left;
    node* right;
    int height;
    explicit avl_node(T k,E v,node* p){
        key=k;
        value=v;
        parent=p;
        left=nullptr;
        right=nullptr;
        height=1;
    }
};

template<typename T,typename E>
struct AVLTree{
private:
    typedef avl_node<T,E> node;
    node* root;

    void updateheight(node *u){
        int lh=0,rh=0;
        if(u->left!= nullptr)lh=u->left->height;
        if(u->right!= nullptr)rh=u->right->height;
        u->height=std::max(lh,rh)+1;
    }

    void rotateR(node *u){
        node* p=u->parent;
        node* v=u->left;
        v->parent=p;
        u->left=v->right;
        v->right=u;
        if(p!= nullptr){
            if(p->left==u)p->left=v;
            else p->right=v;
        }
        updateheight(u);
        updateheight(v);
    }

    void rotateL(node* u){
        node* p=u->parent;
        node* v=u->right;
        v->parent=p;
        u->right=v->left;
        v->left=u;
        if(p!= nullptr){
            if(p->left==u)p->left=v;
            else p->right=v;
        }
        updateheight(u);
        updateheight(v);
    }

    void rotateLR(node* u){
        rotateL(u->left);
        rotateR(u);
    }

    void rotateRL(node* u){
        rotateR(u->right);
        rotateL(u);
    }

    int bias(node* u){
        int lh=0,rh=0;
        if(u->left!= nullptr)lh=u->left->height;
        if(u->right!= nullptr)rh=u->right->height;
        return lh-rh;
    }

    bool adjust1(node* u){//insert form left, erace from right
        int h=u->height;
        if(bias(u)==2){
            if(bias(u)>= 0)rotateR(u);
            else rotateLR(u);
        }
        updateheight(u);
        return h!=u->height;
    }

    bool adjust2(node* u){//insert form right, erace from left
        int h=u->height;
        std::cout<<"in adjust2"<<std::endl;
        if(bias(u)==-2){
            if(bias(u) <= 0)rotateL(u);
            else rotateRL(u);
        }
        std::cout<<"upd h not complete"<<std::endl;
        updateheight(u);

        std::cout<<"upd h complete"<<std::endl;
        return h!=u->height;
    }
public:
    explicit AVLTree(){
        root=nullptr;
    }

    E* find(T k){
        node* u=root;
        while(true){
            if(!u)return nullptr;
            if(u->key==k)return &u->value;
            else if(u->key<k)u=u->right;
            else u=u->left;
        }
    }

    void insert(T k,E v){
        node* u=root;
        bool froml=false;
        while(true){
            if(!u){
                node t=node(k,v,u);
                root=&t;
                u=root;
                break;
            }
            if(u->key==k){
                u->value=v;
                break;
            }
            else if(u->key<k){
                std::cout<<"u->key < k"<<std::endl;
                if(u->right== nullptr){
                    std::cout<<"right null"<<std::endl;
                    node t=node(k,v,u);
                    u->right=&t;
                    break;
                }
                u=u->right;
            }
            else{
                if(u->left== nullptr){
                    node t=node(k,v,u);
                    u->left=&t;
                    u->left->parent=u;
                    froml=true;
                    break;
                }
                u=u->left;
            }
        }
        bool change=true;
        while(change){
            if(froml)change=adjust1(u);
            else change=adjust2(u);

            if(u->parent== nullptr)break;
            else {
                if(u->parent->left==u)froml=true;
                else froml=false;
            }
            u=u->parent;
        }
        std::cout<<"insert complete"<<std::endl;
    }

    void erase(T k){
        node* u=root,cu;
        if(!u)return;
        bool froml=false;
        while(true){
            if(u->key==k){
                node* p=u->parent;
                if(!u->left&&!u->right){//u is leaf
                    if(!u->parent){root=nullptr;return;}

                    if(p->left==u){p->left=nullptr;froml=true;}
                    else p->right=nullptr;
                    cu=p;
                }
                else if(!u->left||!u->right){
                    if(!u->left){//left is nullptr
                        if(!p){root=u->right;return;}

                        if(p->left==u){p->left=u->right;froml=true;}
                        else p->right=u->right;
                    }
                    else{//right is nullptr
                        if(!p){root=u->left;return;}

                        if(p->left==u){p->left=u->left;froml=true;}
                        else p->right=u->leaf;
                    }
                    cu=p;
                }
                else{//u have left&right subtree
                    node* v=u->left;
                    while(v->right){
                        v=v->right;
                    }

                    node* vp=v->parent;
                    if(!v->left){//v doesn't have left subtree
                        if(vp->left==v){vp->left=nullptr;froml=true;}
                        else vp->right=nullptr;
                    }
                    else {
                        if(vp->left==v){vp->left=v->left;froml=true;}
                        else vp->right=v->left;
                    }
                    v->left=u->left;
                    v->right=u->right;
                    v->parent=u->parent;

                    if(p->left==u)p->left=v;
                    else p->right=v;
                    cu=vp;
                }
                break;
            }
            else if(u->key<k){
                if(!u->right){
                    return;
                }
                u=u->right;
                froml=false;
            }
            else{
                if(!u->left){
                    return;
                }
                u=u->left;
                froml=true;
            }
        }
        //cu is root node to change
        bool change=true;
        while(change){
            if(!froml)change=adjust1(cu);
            else change=adjust2(cu);

            if(!cu->parent)break;
            else {
                if(cu->parent->left==cu)froml=true;
                else froml=false;   
            }
            cu=cu->parent;
        }

    }
};

void solve(){
    int m,q;
    std::cin>>m>>q;
    AVLTree<int,int> at;
    for(int i=0;i<m;++i){
        int k,v;
        std::cin>>k>>v;
        at.insert(k,v);
    }
    for(int i=0;i<q;++i){
        int k;
        std::cin>>k;
        auto it=at.find(k);
        if(!it){
            std::cout<<"not exist"<<std::endl;
        }
        else{
            std::cout<<"key:"<<k<<" value:"<<*it<<std::endl;
        }
    }
}

int main(){
    solve();
    return 0;
}