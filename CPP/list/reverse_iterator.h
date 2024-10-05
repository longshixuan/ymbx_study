#pragma once

namespace ymbx{
    template<class Iterator,class Ref,class Ptr>
    struct reverseIterator
    {
        typedef reverseIterator<Iterator, Ref, Ptr> rSelf;

        Iterator _it;
        reverseIterator(Iterator it) : _it(it) {}
        Ref operator*(){
            Iterator tmp = _it;
            --tmp;
            return *tmp;
        }
        Ptr operator->()
        {
            return &(_it._node->_data);
        }
        bool operator!=(const rSelf &s) const
        {
            return _it != s._it;
        }
        bool operator==(const rSelf &s) const
        {
            return _it == s._it;
        }
        rSelf &operator++()
        {
            --_it;
            return *this;
        }
        rSelf operator++(int)
        {
            rSelf tmp = *this;
            --_it;
            return tmp;
        }
        rSelf &operator--()
        {
            ++_it;
            return *this;
        }
        rSelf operator--(int)
        {
            rSelf tmp = *this;
            ++_it;
            return tmp;
        }
        ~reverseIterator() {}
    };
}