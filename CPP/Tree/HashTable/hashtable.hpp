#pragma once
#include<vector>
#include<utility>
#include<iostream>
#include<string>
using namespace std;

namespace ymbx
{
    template<class K,class V>
    struct HashNode{
        HashNode<K, V> *_next;
        pair<K, V> _kv;
        HashNode(const pair<K, V> &kv = pair<K, V>()) : _next(nullptr), _kv(kv){}
    };

    //不一定每次都是操作整型，因此需要一个仿函数将其他类型转换为整型
    template<class T>
    struct HashKey{
        size_t operator()(const T& data){
            return data;
        }
    };

    template<>
    struct HashKey<string>{
        //BKDR字符串转整型算法————最常用
        size_t operator()(const string& str){
            size_t ret = 0;
            for(auto& s:str){
                ret = ret * 31 + (size_t)s;
            }
            return ret;
        }
    };
    template <class K, class V,class HashKEY=HashKey<K>>
    class HashTable
    {
    public:
        typedef HashNode<K, V> *pNode;
        HashKEY hashk;
        void Insert(const pair<K, V> &kv)
        {
            if(_tables.size()==0 || _n/_tables.size()>=1){
                // size_t newSize = _tables.size() == 0 ? 10 : _tables.size() * 2;
                size_t newSize = GetNextPrime(_tables.size());
                vector<pNode> newTables(newSize);
                for(auto& node:_tables){
                    pNode cur=node;
                    while(cur){
                        size_t ret = hashk(cur->_kv.first) % newTables.size();
                        pNode tmp = cur;
                        cur = cur->_next;
                        tmp->_next = newTables[ret];
                        newTables[ret] = tmp;
                    }
                }
                _tables.swap(newTables);
            }
            size_t ret = hashk(kv.first) % _tables.size();
            pNode newHN = new HashNode<K,V>(kv);
            pNode cur = _tables[ret];
            newHN->_next = cur;
            _tables[ret] = newHN;
            ++_n;
        }
        pNode& Find(const pair<K,V>& kv){
            size_t ret = hashk(kv.first) % _tables.size();
            pNode cur = _tables[ret];
            while(cur){
                if(hashk(cur->_kv.first)==hashk(kv.first)){
                    return cur;
                }else{
                    cur = cur->_next;
                }
            }
            return nullptr;
        }
        bool Erase(const K& key){
            size_t ret = hashk(key) % _tables.size();
            pNode parent = nullptr;
            pNode cur = _tables[ret];
            while (cur)
            {
                if (hashk(cur->_kv.first) == hashk(key))
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
        size_t GetNextPrime(size_t& prime){
            static size_t nums = 28;
            static size_t num[nums] = {
                53, 97, 193, 389, 769,
                1543, 3079, 6151, 12289, 24593,
                49157, 98317, 196613, 393241, 786433,
                1572869, 3145739, 6291469, 12582917, 25165843,
                50331653, 100663319, 201326611, 402653189, 805306457,
                1610612741, 3221225473, 4294967291
            };
            size_t i = 0;
            for (; i < nums; ++i)
            {
                if(num[i]>prime){
                    return num[i];
                }
            }
            return num[i];
        }
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

    void HashStr(){
        HashTable<string, string> ht;
        ht.Insert(make_pair("sort", "排序"));
        ht.Insert(make_pair("list", "列表"));
        ht.Insert(make_pair("opera", "操作"));
        ht.Insert(make_pair("do", "做"));
        ht.Insert(make_pair("small", "小"));
        std::cout << ht.Count() << std::endl;
    }
}