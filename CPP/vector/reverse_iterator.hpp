#pragma once

namespace ymbx{

    template<class iterator,class Ref,class Ptr>
    struct reverseIterator
    {
        typedef reverseIterator<iterator, Ref, Ptr> Self;
        iterator _it;
        reverseIterator(iterator it):_it(it){}
        Ref operator*(){
            iterator tmp = _it;
            --tmp;
            return *tmp;
        }
        Ptr operator->()
        {
            return &(_it._node->_data);
        }
        bool operator!=(const Self &self) const
        {
            return _it != self._it;
        }
        bool operator==(const Self &self) const
        {
            return _it == self._it;
        }
        Self &operator++()
        {
            --_it;
            return *this;
        }
        Self operator++(int)
        {
            Self tmp = *this;
            --_it;
            return tmp;
        }
        Self &operator--()
        {
            ++_it;
            return *this;
        }
        Self operator--(int)
        {
            Self tmp = *this;
            ++_it;
            return tmp;
        }
        ~reverseIterator() {}
    };
}