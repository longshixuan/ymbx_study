#pragma once
#include "rbtree2.hpp"

namespace ymbx
{
    template <class Key>
    class Set
    {
        typedef Key ValueType;
        struct KeyOfValue
        {
            const Key &operator()(const ValueType &data)
            {
                return data;
            }
        };
    private:
        typedef typename RBTree<Key, ValueType, KeyOfValue>::iterator iterator;
        typedef RBTree<Key, ValueType, KeyOfValue> rbtSet;
        rbtSet _t;


    public:
        iterator begin(){
            return _t.begin();
        }
        iterator end(){
            return _t.end();
        }
        pair<iterator,bool> insert(const ValueType& data){
            return _t.insert(data);
        }
        bool IsValidRBTree(){
            return _t.IsValidRBTree();
        }
        void InOrder(){
            _t.InOrder();
        }
    };
    void test_Set(){
        ymbx::Set<int> s;
        s.insert(1);
        s.insert(3);
        s.insert(2);
        s.insert(6);
        s.insert(8);
        std::cout << s.IsValidRBTree() << std::endl;
        s.InOrder();
        std::cout << std::endl;
        auto it = s.begin();
        while(it!=s.end())
        {
            std::cout << *it << " ";
            ++it;
        }
        // for(auto e:s)
        // {
        //     std::cout << e << " ";
        // }
        std::cout << std::endl;
    };
}