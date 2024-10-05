#pragma once
#include "hashtableCopy.hpp"
#include<utility>

namespace ymbx
{
    template <class T>
    class HashKey;

    template <class K, class T, class HashKEY = HashKey<T>>
    class UnorderedMap
    {
    public:
        typedef pair<const K, T> ValueType;
        struct MapKeyOfT
        {
            const T &operator()(const pair<const K, T> &data)
            {
                return data.first;
            }
        };
        typedef typename HashTable<T, pair<const K, T>, MapKeyOfT, HashKEY>::iterator iterator;
        typedef typename HashTable<T, pair<const K, T>, MapKeyOfT, HashKEY>::const_iterator const_iterator;

    public:
        iterator begin()
        {
            return _ht.begin();
        }
        iterator end()
        {
            return _ht.end();
        }
        const_iterator cbegin() const
        {
            return _ht.cbegin();
        }
        const_iterator cend() const
        {
            return _ht.cend();
        }
        pair<iterator,bool> insert(const ValueType &data)
        {
            return _ht.Insert(data);
        }
        bool erase(const ValueType &data)
        {
            return _ht.Erase(data);
        }
        T& operator[](const K& k){
            auto ret = find(k);
            return ret->second;
        }
        iterator find(const K &k)
        {
            return _ht.Find(k);
        }
        size_t &Count()
        {
            return _ht.Count();
        }
        size_t MaxBucket()
        {
            return _ht.MaxBucket();
        }

    private:
        HashTable<T, pair<const K, T>, MapKeyOfT, HashKEY> _ht;
    };

    void Test_Map(){
        UnorderedMap<int, int> um;
        um.insert(make_pair(1, 1));
        um.insert(make_pair(15, 15));
        um.insert(make_pair(21, 21));
        um.insert(make_pair(63, 63));
        um.insert(make_pair(77, 77));
        // std::cout << um.Count() << std::endl;
        // auto it = um.begin();
        // // std::cout << "this correct:" << it->first << std::endl;
        // // std::cout << "MaxBucket:" << um.MaxBucket() << std::endl;
        // // while (it != um.end())
        // // {
        // //     // std::cout << "ssss";
        // //     std::cout << it->first << ": " << it->second << " ";
        // //     ++it;
        // // }
        // for (auto &e : um)
        // {
        //     std::cout << e.first << ": "<<e.second<< " ";
        // }
        // std::cout << std::endl;
        // std::cout << um.find(1)->second << std::endl;
        std::cout << um[1] << std::endl;
    };
}