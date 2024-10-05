#pragma once
#include "hashtableCopy.hpp"

namespace ymbx{
    template <class T>
    class HashKey;

    template<class T,class HashKEY=HashKey<T>>
    class UnorderedSet{
    public:
        struct SetKeyOfT{
            const T & operator()(const T &data){
                return data;
            }
        };
        typedef typename HashTable<T, T, SetKeyOfT, HashKEY>::const_iterator iterator;
        typedef typename HashTable<T, T, SetKeyOfT, HashKEY>::const_iterator const_iterator;

    public:
        
        iterator begin(){
            return _ht.begin();
        }
        iterator end(){
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
        pair<iterator,bool> insert(const T&data){
            return _ht.Insert(data);
        }
        bool erase(const T& data){
            return _ht.Erase(data);
        }
        iterator find(const T& data){
            return _ht.Find(data);
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
        HashTable<T, T, SetKeyOfT,HashKEY> _ht;
    };

    void Test_Set(){
        UnorderedSet<int> us;
        us.insert(1);
        us.insert(3);
        us.insert(43);
        us.insert(51);
        us.insert(7);
        std::cout << us.Count() << std::endl;
        auto it = us.begin();
        std::cout << "this correct:" <<*it<< std::endl;
        std::cout << "MaxBucket:" << us.MaxBucket() << std::endl;
        while (it != us.end())
        {
            // *it = 1;
            std::cout << *it << " ";
            ++it;
        }
        // for (auto &e : us)
        // {
        //     std::cout << e << " ";
        // }
        std::cout << std::endl;
    }
}