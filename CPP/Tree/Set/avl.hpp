#pragma once
#include<iostream>
#include<cassert>

namespace ymbx{
    // template<class K,class V>
    template<class T>
    class AVLTreeNode{
        // AVLTreeNode<K, V> *_left;
        // AVLTreeNode<K, V> *_right;
        // AVLTreeNode<K, V> *_parent;
        // pair<K, V> _kv;
        // int _bf;
        // AVLTreeNode(pair<K,V> kv=pair<K,V>()):_left(nullptr),_right(nullptr),_parent(nullptr),_bf(0),_kv(kv){}
    public:
        AVLTreeNode<T> *_left;
        AVLTreeNode<T> *_right;
        AVLTreeNode<T> *_parent;
        T _kv;
        int _bf;
        AVLTreeNode(const T &data = T()) : _left(nullptr), _right(nullptr), _parent(nullptr), _bf(0), _kv(data) {}
    };

    // template<class K,class V>
    template<class T>
    class AVLTree{
        // typedef AVLTreeNode<K, V> Node;
        // typedef pair<K, V> pKV;
        typedef AVLTreeNode<T> Node;

    private:
        Node* _root;
    public:
        AVLTree():_root(nullptr){}
        AVLTree(const T& kv){
            Node* node = new Node(kv);
            _root = node;
        }
        //不加return返回体报错
        bool insert(const T& kv){
            if(_root==nullptr){
                _root = new Node(kv);
                return true;
            }
            Node *cur = _root;
            Node *parent = nullptr;
            while (cur)
            {
                if(cur->_kv< kv){
                    parent = cur;
                    cur = cur->_right;
                }else if(cur->_kv>kv){
                    parent = cur;
                    cur = cur->_left;
                }else{
                    return false;
                }
            }
            cur = new Node(kv);
            if(kv>parent->_kv){
                parent->_right = cur;
                cur->_parent = parent;
            }
            else
            {
                parent->_left = cur;
                cur->_parent = parent;
            }
            while (parent){
                if(cur==parent->_right){
                    parent->_bf++;
                }else{
                    parent->_bf--;
                }

                if(parent->_bf==0){
                    break;
                }else if(parent->_bf==1 || parent->_bf==-1){
                    cur = cur->_parent;
                    parent = parent->_parent;
                }
                else if(parent->_bf==2 || parent->_bf==-2)
                {
                    if(parent->_bf==2 && cur->_bf==1){
                        RotateL(parent);
                    }else if(parent->_bf==-2 && cur->_bf==-1){
                        RotateR(parent);
                    }else if(parent->_bf==-2 && cur->_bf==1){
                        RotateLR(parent);
                    }else if(parent->_bf==2 && cur->_bf==-1){
                        RotateRL(parent);
                    }else{
                        assert(false);
                    }
                    break;
                }else{
                    assert(false);
                }
            }
            return true;
        }
        bool IsBalance(){
            return _IsBalance(_root);
        }
        
        void Interorder(){
            _Interorder(_root);
            std::cout << std::endl;
        }
        ~AVLTree(){
            RemoveNode(_root);
        }

    private:
        int TreeHeight(Node* root){
            if(root==nullptr){
                return 0;
            }
            return std::max(TreeHeight(root->_left), TreeHeight(root->_right)) + 1;
        }
        bool _IsBalance(Node* root){
            if(root==nullptr){
                return true;
            }
            if(TreeHeight(root->_right)-TreeHeight(root->_left)!=root->_bf){
                std::cout <<root->_kv<< "平衡因子异常"<<std::endl;
                return false;
            }
            return abs(TreeHeight(root->_left) - TreeHeight(root->_right)) <2 && _IsBalance(root->_left) && _IsBalance(root->_right);
        }
        void _Interorder(Node* n){
            if (n == nullptr)
            {
                return;
            }
            _Interorder(n->_left);
            std::cout << n->_kv << " ";
            _Interorder(n->_right);
        }
        void RemoveNode(Node* n){
            if(n==nullptr){
                return;
            }
            RemoveNode(n->_left);
            RemoveNode(n->_right);
            delete n;
        }
        void RotateL(Node *node)
        {
            Node *pnode = node->_parent;
            Node *subR = node->_right;
            Node *subRL = subR->_left;
            subR->_left = node;
            node->_parent = subR;
            node->_right = subRL;
            if (subRL)
            {
                subRL->_parent = node;
            }
            if (pnode == nullptr)
            {
                _root = subR;
                subR->_parent = nullptr;
            }
            else
            {
                if (pnode->_left == node)
                {
                    pnode->_left = subR;
                }
                else
                {
                    pnode->_right = subR;
                }
                subR->_parent = pnode;
            }
            node->_bf = subR->_bf = 0;
        }
        void RotateR(Node *node)
        {
            Node *pnode = node->_parent;
            Node *subL = node->_left;
            Node *subLR = subL->_right;
            subL->_right = node;
            node->_parent = subL;
            node->_left = subLR;
            if (subLR)
            {
                subLR->_parent = node;
            }
            if (pnode == nullptr)
            {
                _root = subL;
                subL->_parent = nullptr;
            }
            else
            {
                if (pnode->_left == node)
                {
                    pnode->_left = subL;
                }
                else
                {
                    pnode->_right = subL;
                }
                subL->_parent = pnode;
            }
            subL->_bf = 0;
            node->_bf = 0;
        }
        void RotateLR(Node *node)
        {
            Node *subL = node->_left;
            Node *subLR = subL->_right;
            int bf = subLR->_bf;
            RotateL(node->_left);
            RotateR(node);

            if (bf == 0)
            {
                node->_bf = subL->_bf = subLR->_bf = 0;
            }
            else if (bf == 1)
            {
                node->_bf = 0;
                subL->_bf = -1;
                subLR->_bf = 0;
            }
            else if (bf == -1)
            {
                node->_bf = 1;
                subL->_bf = 0;
                subLR->_bf = 0;
            }
            else
            {
                assert(false);
            }
        }
        void RotateRL(Node *node)
        {
            Node *subR = node->_right;
            Node *subRL = subR->_left;
            int bf = subRL->_bf;
            RotateR(node->_right);
            RotateL(node);
            if (bf == 0)
            {
                node->_bf = subR->_bf = subRL->_bf = 0;
            }
            else if (bf == 1)
            {
                node->_bf = -1;
                subR->_bf = 0;
                subRL->_bf = 0;
            }
            else if (bf == -1)
            {
                node->_bf = 0;
                subR->_bf = 1;
                subRL->_bf = 0;
            }
            else
            {
                assert(false);
            }
        }
    };

    void test()
    {
        AVLTree<int> avltree;
        srand(time(0));
        int a[] = {4, 2, 6, 1, 3, 5, 15, 7, 16, 14};
        // for (auto e : a)
        // {
        //     /*	if (e == 14)
        //         {
        //         int x = 0;
        //         }*/

        //     avltree.insert(e);
        //     std::cout << e << "插入：" << avltree.IsBalance() << std::endl;
        // }
        const size_t N = 100000;
        for (int i = 0; i < N; ++i)
        {
            size_t x = rand();
            avltree.insert(x);
        }
            // avltree.Interorder();
        std::cout<<avltree.IsBalance()<<std::endl;
    }
}