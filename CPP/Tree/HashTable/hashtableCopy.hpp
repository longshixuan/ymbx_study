#pragma once
#include<vector>
#include<utility>
#include<iostream>
#include<string>
#include<array>

using namespace std;

namespace ymbx
{
    //前置声明
    template <class K, class V, class KeyOfT, class HashKEY>
    class HashTable;
    template <class K, class V, class Ref, class Ptr, class KeyOfT, class HashKEY>
    class HashIterator;
   
    template <class T>
    struct HashNode
    {
        HashNode<T> *_next;
        T _data;
        HashNode(const T &data = T()) : _next(nullptr), _data(data) {}
    };

    template <class T>
    struct HashKey
    {
        size_t operator()(const T &data)
        {
            return data;
        }
    };

    template <>
    struct HashKey<string>
    {
        // BKDR字符串转整型算法————最常用
        size_t operator()(const string &str)
        {
            size_t ret = 0;
            for (auto &s : str)
            {
                ret = ret * 31 + (size_t)s;
            }
            return ret;
        }
    };

    template<class K,class V,class Ref,class Ptr,class KeyOfT, class HashKEY>
    class HashIterator{
    public:
        typedef HashNode<V> *pNode;
        typedef HashTable<K, V, KeyOfT, HashKEY>* HT;
        typedef HashIterator<K, V, Ref, Ptr,KeyOfT,HashKEY> Self;
        typedef HashIterator<K, V, V&, V*, KeyOfT, HashKEY> iterator;
        KeyOfT kot;
        HashKEY hk;
        HashIterator(const pNode &node, const HT& ht) : _node(node), _ht(ht) {}
        HashIterator(const iterator& it){
            _node = it.GetNode();
            _ht = it.GetHT();
        }
        Ref operator*(){
            return _node->_data;
        }
        Ptr operator->(){
            return &_node->_data;
        }
        Self& operator++(){
            pNode cur = _node;
            if(cur->_next){
                _node = _node->_next;
            }else{
                size_t hashi = hk(kot(cur->_data)) % (_ht->_tables.size());
                while(++hashi<_ht->_tables.size() &&_ht->_tables[hashi]==nullptr){}
                _node = _ht->_tables[hashi];
            }
            return *this;
        }
        Self &operator--()
        {
            size_t hashi = hk(kot(_node->_data)) % _ht->_tables.size();
            pNode cur = _ht->_tables[hashi];
            if(cur && cur==_node){
                while (_ht->_tables[hashi] == nullptr)
                {
                    --hashi;
                }
                cur = _ht->_tables[hashi];
                while (cur->_next && cur->_next != _node)
                {
                    cur = cur->_next;
                }
                _node = cur;
            }else{
                while(cur->_next && cur->_next !=_node){
                    cur = cur->_next;
                }
                _node = cur;
            }
            return *this;
        }
        bool operator!=(const Self& s){
            return _node != s._node && _ht == s._ht;
        }
        bool operator==(const Self &s)
        {
            return _node == s._node && _ht == s._ht;
        }
        const pNode& GetNode() const{
            return _node;
        }
        const HT& GetHT() const {
            return _ht;
        }

    private:
        pNode _node;
        HT _ht;
    };

    //不一定每次都是操作整型，因此需要一个仿函数将其他类型转换为整型
    
    template <class K, class V,class KeyOfT,class HashKEY>
    class HashTable
    {
    public:
        template <class k, class t, class ref, class ptr, class keyOfT, class hashKEY>
        friend class HashIterator;

    public:
        typedef HashNode<V> *pNode;
        typedef HashIterator<K, V, V &, V *, KeyOfT, HashKEY> iterator;
        typedef HashIterator<K, V, const V &, const V *, KeyOfT, HashKEY> const_iterator;
        HashKEY hashk;
        KeyOfT kot;
        iterator begin(){
            for (size_t idx = 0; idx < _tables.size();++idx){
                if(_tables[idx]){
                    return iterator(_tables[idx], this);
                }
            }
            return iterator(nullptr, this);
        }
        iterator end(){
            return iterator(nullptr, this);
        }
        const_iterator cbegin() const
        {
            for (size_t idx = 0; idx < _tables.size(); ++idx)
            {
                if (_tables[idx])
                {
                    return const_iterator(_tables[idx], this);
                }
            }
            return const_iterator(nullptr, this);
        }
        const_iterator cend() const
        {
            return const_iterator(nullptr, this);
        }
        pair<iterator,bool> Insert(const V &data)
        {
            if(_tables.size()==0 || _n/_tables.size()>=1){
                size_t newSize = _tables.size() == 0 ? 10 : _tables.size() * 2;
                // size_t newSize = GetNextPrime(_tables.size());
                vector<pNode> newTables(newSize);
                for(auto& node:_tables){
                    pNode cur=node;
                    while(cur){
                        size_t ret = hashk(kot(cur->_data)) % newTables.size();
                        pNode tmp = cur;
                        cur = cur->_next;
                        tmp->_next = newTables[ret];
                        newTables[ret] = tmp;
                    }
                }
                _tables.swap(newTables);
            }
            size_t ret = hashk(kot(data)) % _tables.size();
            pNode newHN = new HashNode<V>(data);
            pNode cur = _tables[ret];
            newHN->_next = cur;
            _tables[ret] = newHN;
            ++_n;
            return make_pair(iterator(newHN,this),true);
        }
        iterator Find(const K& k){
            size_t ret = k % _tables.size();
            pNode cur = _tables[ret];
            while(cur){
                if(hashk(kot(cur->_data))==k){
                    return iterator(cur,this);
                }else{
                    cur = cur->_next;
                }
            }
            return iterator(nullptr,this);
        }
        bool Erase(const V& key){
            size_t ret = hashk(kot(key)) % _tables.size();
            pNode parent = nullptr;
            pNode cur = _tables[ret];
            while (cur)
            {
                if (hashk(kot(cur->_data)) == hashk(kot(key)))
                {
                    if(cur==_tables[ret]){
                        _tables[ret] = cur->_next;
                    }
                    if(parent){
                        parent->_next = cur->_next;
                    }
                    --_n;
                    delete cur;
                    return true;
                }
                else
                {
                    parent = cur;
                    cur = cur->_next;
                }
            }
            return false;
        }
        // size_t GetNextPrime(size_t prime){
        //     static size_t nums = 28;
        //     size_t num[nums] = {
        //         53, 97, 193, 389, 769,
        //         1543, 3079, 6151, 12289, 24593,
        //         49157, 98317, 196613, 393241, 786433,
        //         1572869, 3145739, 6291469, 12582917, 25165843,
        //         50331653, 100663319, 201326611, 402653189, 805306457,
        //         1610612741, 3221225473, 4294967291
        //     };
        //     size_t i = 0;
        //     for (; i < nums; ++i)
        //     {
        //         if(num[i]>prime){
        //             return num[i];
        //         }
        //     }
        //     return num[i];
        // }
        ~HashTable(){
            for(auto& node:_tables){
                pNode cur = node;
                while(cur){
                    pNode tmp = cur;
                    cur = cur->_next;
                    delete tmp;
                    tmp = nullptr;
                }
            }
        }
        size_t& Count(){
            return _n;
        }
        size_t MaxBucket(){
            size_t maxsize = 0;
            for (size_t idx = 0; idx < _tables.size();++idx){
                if(_tables[idx]){
                    size_t len = 0;
                    pNode cur = _tables[idx];
                    while(cur){
                        cur = cur->_next;
                        ++len;
                    }
                    maxsize = max(len, maxsize);
                }
            }
            return maxsize;
        }

    private:
        vector<pNode> _tables;
        size_t _n = 0;
    };
    void Test_Hash(){
        // HashTable<int, int> hashtable;
        // hashtable.Insert(std::make_pair(1, 1));
        // hashtable.Insert(std::make_pair(2, 2));
        // hashtable.Insert(std::make_pair(4, 4));
        // hashtable.Insert(std::make_pair(6, 6));
        // hashtable.Insert(std::make_pair(44, 44));
        // hashtable.Insert(std::make_pair(66, 66));
        // hashtable.Insert(std::make_pair(67, 67));
        // hashtable.Insert(std::make_pair(68, 68));
        // hashtable.Insert(std::make_pair(69, 69));
        // hashtable.Insert(std::make_pair(60, 60));
        // hashtable.Insert(std::make_pair(61, 61));
        // hashtable.Insert(std::make_pair(62, 62));
        // hashtable.Insert(std::make_pair(63, 63));
        // hashtable.Erase(66);
        // hashtable.Erase(60);
        // hashtable.Erase(61);
        // std::cout << hashtable.Count() << std::endl;
    }

    // void HashStr(){
    //     HashTable<string, string> ht;
    //     ht.Insert(make_pair("sort", "排序"));
    //     ht.Insert(make_pair("list", "列表"));
    //     ht.Insert(make_pair("opera", "操作"));
    //     ht.Insert(make_pair("do", "做"));
    //     ht.Insert(make_pair("small", "小"));
    //     std::cout << ht.Count() << std::endl;
    // }
}