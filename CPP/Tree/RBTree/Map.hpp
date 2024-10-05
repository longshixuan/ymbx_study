#pragma once
#include "rbtree2.hpp"


namespace ymbx{
    template<class Key,class Value>
    class Map{
    private:
        typedef pair<const Key, Value> ValueType;
        struct KeyOfValue
        {
            const Key &operator()(const ValueType &data)
            {
                return data.first;
            }
        };
        typedef typename RBTree<Key, ValueType, KeyOfValue>::iterator iterator;
        typedef RBTree<Key, ValueType,KeyOfValue> rbtMap;
        rbtMap _t;
        

    public:
        iterator begin()
        {
            return _t.begin();
        }
        iterator end()
        {
            return _t.end();
        }
        Value& operator[](const Key& k){
            pair<iterator, bool> ret = _t.insert(make_pair(k, Value()));
            return ret.first->second;
        }
        pair<iterator,bool> insert(const ValueType& data){
            return _t.insert(data);
        }
        bool IsValidRBTree()
        {
            return _t.IsValidRBTree();
        }
        void InOrder()
        {
            _t.InOrder();
        }
    };
    void test_Map()
    {
        ymbx::Map<int,int> m;
        m.insert(make_pair(1,1));
        m.insert(make_pair(3, 3));
        m.insert(make_pair(2, 2));
        m.insert(make_pair(6, 6));
        m.insert(make_pair(8, 8));
        std::cout << m.IsValidRBTree() << std::endl;
        m.InOrder();
        std::cout << std::endl;
        auto it = m.begin();
        while (it != m.end())
        {
            std::cout << (*it).first <<" "<< (*it).second<<" ";
            ++it;
        }
        // for(auto e:m)
        // {
        //     std::cout << e.first << " " << e.second << " ";
        // }
        std::cout << std::endl;
    }
}