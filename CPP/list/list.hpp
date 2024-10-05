#pragma once
#include <iostream>
#include <cassert>
#include "reverse_iterator.h"

namespace ymbx
{
    template<class T>
    struct listNode
    {
        listNode<T> *_prev;
        listNode<T> *_next;
        T _data;
        listNode(const T& val=T()):_data(val),_prev(nullptr),_next(nullptr)
        {}
    };

    //迭代器要么就是原生指针，要么就是对指针的封装
    template<class T,class Ref,class Ptr>
    struct list_iterator
    {
        typedef listNode<T> node;
        typedef list_iterator<T,Ref,Ptr> self;
        node* _node;
        list_iterator(node* n = nullptr) : _node(n)
        {}
        list_iterator(const self& s)
        {
            _node = s._node;
        }
        Ref operator*()
        {
            return _node->_data;
        }
        Ptr operator->()
        {
            return &_node->_data;
        }
        bool operator!=(const self& s)const
        {
            return _node != s._node;
        }
        bool operator==(const self &s) const
        {
            return _node == s._node;
        }
        self& operator++()
        {
            _node = _node->_next;
            return *this;
        }
        self operator++(int)
        {
            self tmp = *this;
            _node = _node->_next;
            return tmp;
        }
        self &operator--()
        {
            _node = _node->_prev;
            return *this;
        }
        self operator--(int)
        {
            self tmp = *this;
            _node = _node->_prev;
            return tmp;
        }
        ~list_iterator(){}
    };

    template<class T>
    class list
    {
        typedef listNode<T> node;
    public:
        typedef list_iterator<T,T&,T*> iterator;
        typedef list_iterator<T,const T&,const T*> const_iterator;
        //类名、函数名以及typedef定义的名不能相同
        typedef reverseIterator<iterator, T &, T *> reverse_iterator;
        typedef reverseIterator<iterator, const T &, const T *> const_reverse_iterator;

    public:
        void empty_init(){
            _head = new node;
            _head->_next = _head;
            _head->_prev = _head;
        }
        list()
        {
            empty_init();
        }
        // list(const list<T> &it){
        //     empty_init();
        //     for(auto e:it){
        //         push_back(e);
        //     }
        // }
        template <class iterator>
        list(iterator first, iterator end)
        {
            empty_init();
            while (first != end)
            {
                push_back(*first);
                ++first;
            }
        }
        void swap(list<T> &tmp)
        {
            std::swap(_head, tmp._head);
        }
        list(const list<T> &it)
        {
            empty_init();
            list<T> tmp(it.begin(), it.end());
            swap(tmp);
        }
        iterator begin()
        {
            return iterator(_head->_next);
        }
        iterator end()
        {
            return iterator(_head);
        }
        const_iterator cbegin() const
        {
            return const_iterator(_head->_next);
        }
        const_iterator cend() const
        {
            return const_iterator(_head);
        }
        reverse_iterator rbegin()
        {
            return reverse_iterator(end());
        }
        reverse_iterator rend()
        {
            return reverse_iterator(begin());
        }
        const_reverse_iterator crbegin() const
        {
            return const_reverse_iterator(cend());
        }
        const_reverse_iterator crend() const
        {
            return const_reverse_iterator(cbegin());
        }
        void push_back(const T& val)
        {
            // node *tail = _head->_prev;
            // node *tmp = new node(val);
            // tail->_next = tmp;
            // tmp->_prev = tail;
            // tmp->_next = _head;
            // _head->_prev = tmp;
            insert(end(), val);
        }
        void push_front(const T& val)
        {
            insert(begin(), val);
        }
        void pop_back()
        {
            erase(iterator(_head->_prev));
        }
        void pop_front()
        {
            erase(begin());
        }
        
        iterator insert(iterator pos,const T& val)
        {
            node *new_node = new node(val);
            node *pre = (pos._node)->_prev;
            pre->_next = new_node;
            new_node->_prev = pre;
            new_node->_next = pos._node;
            pos._node->_prev = new_node;
            return iterator(new_node);
        }
        iterator erase(iterator pos)
        {
            assert(pos != end());
            node *pre = (pos._node)->_prev;
            node *nex = (pos._node)->_next;
            pre->_next = nex;
            nex->_prev = pre;
            delete pos._node;
            return iterator(nex);
        }
        void clear(){
            while (_head->_next != _head)
            {
                node *tmp = _head->_next->_next;
                delete _head->_next;
                _head->_next = tmp;
            }
            _head->_prev = _head;
            _head->_next = _head; 
        }
        bool empty()
        {
            return begin() == end();
        }
        size_t size()
        {
            iterator tmp = begin();
            size_t cnt = 0;
            while (tmp != end())
            {
                ++tmp;
                ++cnt;
            }
            return cnt;
        }
        void resize(size_t n,const T& val=T())
        {
            if(n<size()){
                node *cur = begin()._node;
                for(size_t i=1;i<n;++i)
                {
                    cur = cur->_next;
                }
                while(cur->_next!=_head){
                    node *tmp = cur->_next->_next;
                    delete cur->_next;
                    cur->_next = tmp;
                }
            }else if(n>size())
            {
                size_t num = n - size();
                while(num--){
                    push_back(val);
                }
            }
        }
        ~list()
        {
            while(_head->_next!=_head)
            {
                node *tmp = _head->_next->_next;
                delete _head->_next;
                _head->_next = tmp;
            }
            delete _head;
        }

    private:
        node *_head;
    };



    void test()
    {
        list<int> li;
        li.push_back(1);
        li.push_back(2);
        li.push_back(3);
        li.push_back(4);
        li.push_back(5);
        // list<int> lli(li);
        // li.resize(10,3);
        // list<int> li2;
        // li.swap(li2);
        // li.clear();
        // for(auto e:lli)
        // {
        //     std::cout << e << " ";
        // }
        // std::cout << std::endl;
        // std::cout << li.size() << std::endl;
        auto rit = li.rbegin();
        while( rit !=li.rend()){
            std::cout << *rit << " ";
            ++rit;
        }
        std::cout << std::endl;
        // std::cout << li2.size() << std::endl;
        // li.pop_back();
        // // for(auto e:li)
        // // {
        // //     std::cout << e << " ";
        // // }
        // // std::cout << std::endl;
        // std::cout << li.size() << std::endl;
        // li.push_front(10);
        // // for (auto e : li)
        // // {
        // //     std::cout << e << " ";
        // // }
        // // std::cout << std::endl;
        // std::cout << li.size() << std::endl;
        // li.pop_front();
        // // for (auto e : li)
        // // {
        // //     std::cout << e << " ";
        // // }
        // // std::cout << std::endl;
        // std::cout << li.size() << std::endl;
    }
}
