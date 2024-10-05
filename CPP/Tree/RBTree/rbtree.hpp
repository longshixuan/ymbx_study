#pragma once
#include <cassert>
#include <iostream>
#include <utility>
using namespace std;

namespace ymbx
{
    enum Col
    {
        RED,
        BLACK
    };
    template <class K, class V>
    struct rbtreeNode
    {
        rbtreeNode<K, V> *_pparent;
        rbtreeNode<K, V> *_pleft;
        rbtreeNode<K, V> *_pright;
        std::pair<K,V> _kv;
        Col _col;
        rbtreeNode(const std::pair<K,V>& kv):_pparent(nullptr),_pleft(nullptr),_pright(nullptr),_kv(kv),_col(RED){}
    };

    template<class K,class V>
    class RBTree{
        typedef rbtreeNode<K, V> Node;
    private:
        Node *_root;
    public:
        RBTree(Node *node=nullptr) : _root(node){};
        RBTree(const RBTree<K,V>& rbt){
            _root = _Copy(rbt._root);
        }
        Node* find(const std::pair<K,V>& kv){
            Node *pcur = _root;
            while(pcur){
                if(pcur->_kv.first > kv.first){
                    pcur = pcur->_pleft;
                }else if(pcur->_kv.first< kv.first){
                    pcur = pcur->_pright;
                }else{
                    return pcur;
                }
            }
            return nullptr;
        }
        bool insert(const std::pair<K,V>& kv){
            if(_root==nullptr){
                _root = new Node(kv);
                _root->_col = BLACK;
                return true;
            }
            Node *cur = _root;
            Node *parent = nullptr;
            while(cur){
                if(cur->_kv.first>kv.first){
                    parent = cur;
                    cur = cur->_pleft;
                }else{
                    parent = cur;
                    cur = cur->_pright;
                }
            }
            cur = new Node(kv);
            if (parent->_kv.first > kv.first){
                parent->_pleft = cur;
                cur->_pparent = parent;
            }
            else
            {
                parent->_pright = cur;
                cur->_pparent = parent;
            }
            while(parent && parent->_col==RED){
                Node *grandparent = parent->_pparent;
                Node *uncle;
                if (parent == grandparent->_pleft)
                {
                    uncle = grandparent->_pright;
                }
                else
                {
                    uncle = grandparent->_pleft;
                }
                if(uncle && uncle->_col==RED){
                    uncle->_col = BLACK;
                    parent->_col = BLACK;
                    grandparent->_col = RED;
                    cur = grandparent;
                    parent = cur->_pparent;
                }
                else
                {
                    if (parent == grandparent->_pleft && cur==parent->_pleft)
                    {
                        RotateR(grandparent);
                        parent->_col = BLACK;
                        grandparent->_col = RED;
                    }
                    else if (parent == grandparent->_pright && cur==parent->_pright)
                    {
                        RotateL(grandparent);
                        parent->_col = BLACK;
                        grandparent->_col = RED;
                    }else if(parent==grandparent->_pleft && cur==parent->_pright){
                        RotateL(parent);
                        RotateR(grandparent);
                        cur->_col = BLACK;
                        grandparent->_col = RED;
                        parent->_col = RED;
                    }
                    else if (parent == grandparent->_pright && cur == parent->_pleft)
                    {
                        RotateR(parent);
                        RotateL(grandparent);
                        cur->_col = BLACK;
                        grandparent->_col = RED;
                        parent->_col = RED;
                    }
                }
                _root->_col = BLACK;
            }
            return true;
        }
        Node* GetRoot(){
            return _root;
        }

        bool IsValidRBTree()
        {
            Node* pRoot = GetRoot();
            // 空树也是红黑树
            if (nullptr == pRoot)
                return true;
            // 检测根节点是否满足情况
            if (BLACK != pRoot->_col)
            {
                cout << "违反红黑树性质二：根节点必须为黑色" << endl;
                return false;
            }
            // 获取任意一条路径中黑色节点的个数
            size_t blackCount = 0;
            Node* pCur = pRoot;
            while (pCur)
            {
                if (BLACK == pCur->_col)
                    blackCount++;
                pCur = pCur->_pleft;
            }
            // 检测是否满足红黑树的性质，k用来记录路径中黑色节点的个数
            size_t k = 0;
            return _IsValidRBTree(pRoot, k, blackCount);
        }
        void InOrder()
        {
            _InOrder(_root);
        }
        ~RBTree(){
            _Destory(_root);
            _root = nullptr;
        }

    private:
        Node* _Copy(const Node* node){
            if(node==nullptr){
                return nullptr;
            }
            Node *root = new Node(node->_kv);
            root->_col = node->_col;
            root->_pparent = node->_pparent;
            if (node->_pleft)
            {
                root->_pleft = _Copy(node->_pleft);
            }
            if(node->_pright){
                root->_pright = _Copy(node->_pright);
            }
            return root;
        }
        void _Destory(Node* root){
            if(root==nullptr){
                return;
            }
            _Destory(root->_pleft);
            _Destory(root->_pright);
            delete root;
        }
        bool _IsValidRBTree(Node* pRoot, size_t k, const size_t blackCount)
        {
            //走到null之后，判断k和black是否相等
            if (nullptr == pRoot)
            {
                if (k != blackCount)
                {
                    cout << "违反性质四：每条路径中黑色节点的个数必须相同" << endl;
                    return false;
                }
                return true;
            }
            // 统计黑色节点的个数
            if (BLACK == pRoot->_col)
                k++;
            // 检测当前节点与其双亲是否都为红色
            Node* pParent = pRoot->_pparent;
            if (pParent && RED == pParent->_col && RED == pRoot->_col)
            {
                cout << "违反性质三：没有连在一起的红色节点" << endl;
                return false;
            }
            return _IsValidRBTree(pRoot->_pleft, k, blackCount) && _IsValidRBTree(pRoot->_pright, k, blackCount);
        }
        

    protected:
        void RotateR(Node * node)
        {
            Node *pnode = node->_pparent;
            Node *subL = node->_pleft;
            Node *subLR = subL->_pright;
            node->_pleft=subLR;
            node->_pparent = subL;
            subL->_pright = node;
            if (subLR)
            {
                subLR->_pparent = node;
            }
            subL->_pparent=pnode;
            if(pnode==nullptr){
                _root = subL;
                subL->_pparent = nullptr;
                _root->_col = BLACK;
            }else{
                if (node == pnode->_pleft)
                {
                    pnode->_pleft = subL;
                }
                else
                {
                    pnode->_pright = subL;
                }
            }
            return;
        }
        void RotateL(Node * node)
        {
            Node *pnode = node->_pparent;
            Node *subR = node->_pright;
            Node *subRL = subR->_pleft;
            node->_pright=subRL;
            node->_pparent = subR;
            subR->_pleft = node;
            if (subRL)
            {
                subRL->_pparent = node;
            }
            subR->_pparent = pnode;
            if (pnode == nullptr)
            {
                _root = subR;
                subR->_pparent = nullptr;
                _root->_col = BLACK;
            }
            else
            {
                if (node == pnode->_pleft)
                {
                    pnode->_pleft = subR;
                }
                else
                {
                    pnode->_pright = subR;
                }
            }
            return;
        }
        void _InOrder(Node* root){
            if(root==nullptr){
                return;
            }
            _InOrder(root->_pleft);
            std::cout << root->_kv.first << " ";
            _InOrder(root->_pright);
        }
    };
}