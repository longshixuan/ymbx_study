#pragma once
#include<iostream>
#include<functional>
#include<string>

//BST————————K模型
namespace ymbx{
    template<class K>
    struct BSTreeNode{
        BSTreeNode(const K& key=K()):_left(nullptr),_right(nullptr),_key(key){}
        BSTreeNode<K>* _left;
        BSTreeNode<K>* _right;
        K _key;
    };

    template <class K>
    class BSTree
    {
        typedef BSTreeNode<K> Node;

    private:
        Node *_root;
    public:
        BSTree():_root(nullptr)
        {}
        // BSTree() = default;//强制生成默认构造
        BSTree(const BSTree<K>& t){
            _root=Copy(t._root);
        }
        BSTree<K>& operator=(const BSTree<K>& t){
            _root=Copy(t._root);
            return *this;
        }
        bool InsertR(const K &key)
        {
            return _InsertR(_root, key);
        }
        bool Insert(const K& key){
            // if(_root==nullptr){
            //     _root = new Node(key);
            //     return true;
            // }
            Node *parent = nullptr;
            Node *cur = _root;
            while(cur){
                if(cur->_key > key){
                    parent = cur;
                    cur = cur->_left;
                }
                else if(cur->_key < key){
                    parent = cur;
                    cur = cur->_right;
                }else{
                    return false;
                }
            }
            cur = new Node(key);
            if(parent==nullptr){
                _root = cur;
            }else{
                if (parent->_key > key)
                {
                    parent->_left = cur;
                }
                else
                {
                    parent->_right = cur;
                }
            }
            return true;
        }
        bool EraseR(const K& key){
            return _EraseR(_root, key);
        }
        bool Erase(const K& key){
            Node *parent = nullptr;
            Node *cur = _root;
            while(cur)
            {
                if(cur->_key>key){
                    parent = cur;
                    cur = cur->_left;
                }else if(cur->_key<key){
                    parent = cur;
                    cur = cur->_right;
                }else{
                    Node *pminRight;
                    Node *minRight;
                    if(cur->_left==nullptr){
                        // pminRight = cur;
                        // minRight = cur->_right;
                        // while(minRight){
                        //     pminRight = minRight;
                        //     minRight = minRight->_left;
                        // }
                        // cur->_key = pminRight->_key;
                        // delete pminRight;
                        // return true;
                        if(cur==_root){
                            _root = cur->_right;
                            return true;
                        }
                        if(cur==parent->_right){
                            parent->_right = cur->_right;
                        }else{
                            parent->_left = cur->_right;
                        }
                        return true;
                    }
                    else if (cur->_right == nullptr)
                    {
                        // pminRight = cur;
                        // minRight = cur->_left;
                        // while(minRight){
                        //     pminRight = minRight;
                        //     minRight = minRight->_right;
                        // }
                        // cur->_key = pminRight->_key;
                        // delete pminRight;
                        // return true;
                        if(cur==_root){
                            _root = cur->_left;
                            return true;
                        }
                        if (cur == parent->_right)
                        {
                            parent->_right = cur->_left;
                        }
                        else
                        {
                            parent->_left = cur->_left;
                        }
                        delete cur;
                        return true;
                    }
                    else
                    {
                        pminRight = cur;
                        minRight = cur->_right;
                        if(minRight->_left==nullptr){
                            cur->_key = minRight->_key;
                            pminRight->_right = minRight->_right;
                            delete minRight;
                            return true;
                        }
                        while (minRight)
                        {
                            pminRight = minRight;
                            minRight = minRight->_left;
                        }
                        cur->_key = pminRight->_key;
                        delete pminRight;
                        return true;
                    }
                }
            }
            return false;
        }
        bool FindR(const K& key){
            return _FindR(_root, key);
        }
        bool Find(const K& key){
            Node *cur = _root;
            while(cur){
                if(cur->_key>key){
                    cur = cur->_left;
                }else if(cur->_key<key){
                    cur = cur->_right;
                }else{
                    return true;
                }
            }
            return false;
        }
        void Scan()
        {
            _Scan(_root);
        }
        ~BSTree(){
            RemoveNode();
        }
    protected:
        Node* Copy(const Node* n){
            if(n==nullptr){
                return nullptr;
            }
            Node *newn = new Node(n->_key);
            newn->_left = Copy(n->_left);
            newn->_right = Copy(n->_right);
            return newn;
        }
        bool _InsertR(Node *&n, const K &key)
        {
            if (n == nullptr)
            {
                Node *tmp = new Node(key);
                return true;
            }
            if (n->_key > key)
            {
                return _InsertR(n->_left, key);
            }
            else if (n->_key < key)
            {
                return _InsertR(n->_right, key);
            }
            else
            {
                return false;
            }
        }
        bool _EraseR(Node *&n, const K &key)
        {
            if (n == nullptr)
            {
                return false;
            }
            if (n->_key == key)
            {
                Node *tmp = n;
                if (n->_left == nullptr)
                {
                    n = n->_right;
                }
                else if (n->_right == nullptr)
                {
                    n = n->_left;
                }
                else
                {
                    Node *minRight = n->_right;
                    while (minRight->_left)
                    {
                        minRight = minRight->_left;
                    }
                    std::swap(n->_key, minRight->_key);
                    _EraseR(_root->_right, key);
                }
                delete tmp;
                return true;
            }
            if (n->_key > key)
            {
                return _EraseR(n->_left, key);
            }
            else
            {
                return _EraseR(n->_right, key);
            }
        }
        bool _FindR(Node *n, const K &key)
        {
            if (n == nullptr)
            {
                return false;
            }
            if (n->_key > key)
            {
                return _FindR(n->_left, key);
            }
            else if (n->_key < key)
            {
                return _FindR(n->_right, key);
            }
            else
            {
                return true;
            }
        }
        // 遍历BST
        void _Scan(Node *n)
        {
            if (n == nullptr)
            {
                return;
            }
            _Scan(n->_left);
            std::cout << n->_key << " ";
            _Scan(n->_right);
        }
        // 清空BST
        void RemoveNode()
        {
            _RemoveNode(_root);
        }
        void _RemoveNode(Node *n)
        {
            if (n == nullptr)
            {
                return;
            }
            _RemoveNode(n->_left);
            _RemoveNode(n->_right);
            delete n;
        }
    };


    void Test()
    {
        BSTree<int> bst;
        int a[] = {8, 3, 1, 10, 6, 4, 7, 14, 13};
        for(auto e:a){
            bst.Insert(e);
            // std::cout << e << " ";
        }
        BSTree<int> bst2(bst);
        BSTree<int> bst3;
        bst3 = bst2;
        // bst.Erase(14);
        // bst.Erase(13);
        // bst.Erase(10);
        // // bst.Erase(8);
        // // bst.Erase(14);
        // bst.Scan();
        // std::cout<<bst.Find(8);
        // bst.EraseR(14);
        // bst.EraseR(13);
        // bst.EraseR(10);
        // bst.Erase(8);
        // bst.Erase(14);
        bst.Scan();
        bst2.Scan();
        bst3.Scan();
        // std::cout << bst.FindR(8);
    }
}

//BST————————K,V模型
namespace ymbx{
    template <class K,class V>
    struct BSTreeNodeKV
    {
        BSTreeNodeKV(const K &key = K(), const V &value = V()) : _left(nullptr), _right(nullptr), _key(key), _value(value) {}
        BSTreeNodeKV<K,V> *_left;
        BSTreeNodeKV<K,V> *_right;
        K _key;
        V _value;
    };

    template <class K,class V>
    class BSTreeKV
    {
        typedef BSTreeNodeKV<K, V> Node;

    private:
        Node *_root;

    public:
        BSTreeKV() : _root(nullptr)
        {
        }
        // BSTree() = default;//强制生成默认构造
        BSTreeKV(const BSTreeKV<K, V> &t)
        {
            _root = Copy(t._root);
        }
        BSTreeKV<K, V> &operator=(const BSTreeKV<K, V> &t)
        {
            _root = Copy(t._root);
            return *this;
        }
        bool InsertR(const K &key,const V& value)
        {
            return _InsertR(_root, key,value);
        }
        bool Insert(const K &key,const V& value)
        {
            // if(_root==nullptr){
            //     _root = new Node(key);
            //     return true;
            // }
            Node *parent = nullptr;
            Node *cur = _root;
            while (cur)
            {
                if (cur->_key > key)
                {
                    parent = cur;
                    cur = cur->_left;
                }
                else if (cur->_key < key)
                {
                    parent = cur;
                    cur = cur->_right;
                }
                else
                {
                    return false;
                }
            }
            cur = new Node(key,value);
            if (parent == nullptr)
            {
                _root = cur;
            }
            else
            {
                if (parent->_key > key)
                {
                    parent->_left = cur;
                }
                else
                {
                    parent->_right = cur;
                }
            }
            return true;
        }
        bool EraseR(const K &key,const V& value)
        {
            return _EraseR(_root, key,value);
        }
        bool Erase(const K &key)
        {
            Node *parent = nullptr;
            Node *cur = _root;
            while (cur)
            {
                if (cur->_key > key)
                {
                    parent = cur;
                    cur = cur->_left;
                }
                else if (cur->_key < key)
                {
                    parent = cur;
                    cur = cur->_right;
                }
                else
                {
                    Node *pminRight;
                    Node *minRight;
                    if (cur->_left == nullptr)
                    {
                        // pminRight = cur;
                        // minRight = cur->_right;
                        // while(minRight){
                        //     pminRight = minRight;
                        //     minRight = minRight->_left;
                        // }
                        // cur->_key = pminRight->_key;
                        // delete pminRight;
                        // return true;
                        if (cur == _root)
                        {
                            _root = cur->_right;
                            return true;
                        }
                        if (cur == parent->_right)
                        {
                            parent->_right = cur->_right;
                        }
                        else
                        {
                            parent->_left = cur->_right;
                        }
                        return true;
                    }
                    else if (cur->_right == nullptr)
                    {
                        // pminRight = cur;
                        // minRight = cur->_left;
                        // while(minRight){
                        //     pminRight = minRight;
                        //     minRight = minRight->_right;
                        // }
                        // cur->_key = pminRight->_key;
                        // delete pminRight;
                        // return true;
                        if (cur == _root)
                        {
                            _root = cur->_left;
                            return true;
                        }
                        if (cur == parent->_right)
                        {
                            parent->_right = cur->_left;
                        }
                        else
                        {
                            parent->_left = cur->_left;
                        }
                        delete cur;
                        return true;
                    }
                    else
                    {
                        pminRight = cur;
                        minRight = cur->_right;
                        if (minRight->_left == nullptr)
                        {
                            cur->_key = minRight->_key;
                            pminRight->_right = minRight->_right;
                            delete minRight;
                            return true;
                        }
                        while (minRight)
                        {
                            pminRight = minRight;
                            minRight = minRight->_left;
                        }
                        cur->_key = pminRight->_key;
                        delete pminRight;
                        return true;
                    }
                }
            }
            return false;
        }
        bool FindR(const K &key)
        {
            return _FindR(_root, key);
        }
        Node* Find(const K &key)
        {
            Node *cur = _root;
            while (cur)
            {
                if (cur->_key > key)
                {
                    cur = cur->_left;
                }
                else if (cur->_key < key)
                {
                    cur = cur->_right;
                }
                else
                {
                    return cur;
                }
            }
            return nullptr;
        }
        void Scan()
        {
            _Scan(_root);
        }
        ~BSTreeKV()
        {
            RemoveNode();
        }

    protected:
        Node *Copy(const Node *n)
        {
            if (n == nullptr)
            {
                return nullptr;
            }
            Node *newn = new Node(n->_key,n->_value);
            newn->_left = Copy(n->_left);
            newn->_right = Copy(n->_right);
            return newn;
        }
        bool _InsertR(Node *&n, const K &key,const V& value)
        {
            if (n == nullptr)
            {
                Node *tmp = new Node(key,value);
                return true;
            }
            if (n->_key > key)
            {
                return _InsertR(n->_left, key,value);
            }
            else if (n->_key < key)
            {
                return _InsertR(n->_right, key,value);
            }
            else
            {
                n->_value++;
            }
        }
        bool _EraseR(Node *&n, const K &key,const V& value)
        {
            if (n == nullptr)
            {
                return false;
            }
            if (n->_key == key)
            {
                if(n->_value>=value){
                    n->_value -= value;
                    return true;
                }
                else
                {
                Node *tmp = n;
                if (n->_left == nullptr)
                {
                    n = n->_right;
                }
                else if (n->_right == nullptr)
                {
                    n = n->_left;
                }
                else
                {
                    Node *minRight = n->_right;
                    while (minRight->_left)
                    {
                        minRight = minRight->_left;
                    }
                    std::swap(n->_key, minRight->_key);
                    std::swap(n->_value, minRight->_value);
                    _EraseR(_root->_right, key,value);
                }
                delete tmp;
                return true;
                }
            }
            if (n->_key > key)
            {
                return _EraseR(n->_left, key,value);
            }
            else
            {
                return _EraseR(n->_right, key,value);
            }
        }
        bool _FindR(Node *n, const K &key)
        {
            if (n == nullptr)
            {
                return false;
            }
            if (n->_key > key)
            {
                return _FindR(n->_left, key);
            }
            else if (n->_key < key)
            {
                return _FindR(n->_right, key);
            }
            else
            {
                return true;
            }
        }
        // 遍历BST
        void _Scan(Node *n)
        {
            if (n == nullptr)
            {
                return;
            }
            _Scan(n->_left);
            std::cout << n->_key << ":"<<n->_value<<std::endl;
            _Scan(n->_right);
        }
        // 清空BST
        void RemoveNode()
        {
            _RemoveNode(_root);
        }
        void _RemoveNode(Node *n)
        {
            if (n == nullptr)
            {
                return;
            }
            _RemoveNode(n->_left);
            _RemoveNode(n->_right);
            delete n;
        }
    };
    void Test2(){
        BSTreeKV<std::string, int> bstkv;
        bstkv.Insert("apple", 1);
        bstkv.Insert("banana",2);
        bstkv.Insert("water",3);
        bstkv.Insert("bb",4);
        bstkv.Insert("dd",5);
        bstkv.Insert("ccc", 6);
        bstkv.Scan();
        std::cout << std::endl;
        // std::cout<<bstkv.Find("banana")<<std::endl;
        bstkv.EraseR("banana", 3);
        bstkv.EraseR("apple", 3);
        bstkv.EraseR("bb", 1);
        // std::cout << bstkv.Find("banana") << std::endl;
        bstkv.Scan();
    }
}
