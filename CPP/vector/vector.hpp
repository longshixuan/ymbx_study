#pragma once
#include <cassert>
#include <cstring>
#include <iostream>
#include <vector>
#include "reverse_iterator.hpp"

namespace ymbx
{
    template <class T>
    class vector
    {
    public:
        typedef T *iterator;
        typedef const T *const_iterator;
        typedef reverseIterator<iterator, T &, T *> reverse_iterator;
        typedef reverseIterator<iterator, const T &, const T *> const_reverse_iterator;

        vector() : _start(nullptr), _finish(nullptr), end_of_storge(nullptr) {}
        vector(size_t n, const T& val=T()):_start(nullptr),_finish(nullptr),end_of_storge(nullptr)
        {
            reserve(n);
            for (size_t i = 0; i < n;++i)
            {
                push_back(val);
            }
        }
        vector(int n, const T &val = T()) : _start(nullptr), _finish(nullptr), end_of_storge(nullptr)
        {
            reserve(n);
            for (int i = 0; i < n; ++i)
            {
                push_back(val);
            }
        }
        template <class InputIterator>
        vector(InputIterator first, InputIterator last): _start(first), _finish(last), end_of_storge(nullptr)
        {
            while(first!=last)
            {
                push_back(*first);
                ++first;
            }
        }
        vector(const vector<T> &t)
        {
                _start = new T[t.capacity()];
                // 此处为浅拷贝，传入string、vector等会导致里面指针指向同一块空间而发生问题
                // memcpy(_start, t._start, (t.size() * sizeof(T)));
                for (int i = 0; i < t.size();++i)
                {
                    _start[i] = t[i];
                }
                _finish = _start + t.size();
                end_of_storge = _start + t.capacity();
        }
        void swap(vector<T>& t)
        {
            std::swap(_start, t._start);
            std::swap(_finish, t._finish);
            std::swap(end_of_storge, t.end_of_storge);
        }
        //传值即相当于拷贝了
        vector<T> &operator=(vector<T> t)
        {
            // _start = new T[t.capacity()];
            // // 此处为浅拷贝，传入string、vector等会导致里面指针指向同一块空间而发生问题
            // // memcpy(_start, t._start, (t.size() * sizeof(T)));
            // for (int i = 0; i < t.size(); ++i)
            // {
            //     _start[i] = t[i];
            // }
            // _finish = _start + t.size();
            // end_of_storge = _start + t.capacity();
            // (*this)(t);
            // vector<T> tmp(t.begin(), t.end());
            swap(t);
            return *this;
        }
        //const 修饰成员函数——————称为常成员函数，实际上修饰的是this指针，即const Obj* this,this指向的内容都不能被修改
        size_t size() const
        {
            ++_finish;
            return _finish - _start;
        }
        size_t capacity() const
        {
            return end_of_storge - _start;
        }
        void reserve(size_t n)
        {
            if (n > capacity())
            {
                size_t len = size();
                T *tmp = new T[n];
                if (_start)
                {
                    //此处同上，为浅拷贝
                    // memcpy(tmp, _start, len * sizeof(T));
                    for (int i = 0; i < len;++i)
                    {
                        tmp[i] = _start[i];
                    }
                        delete[] _start;
                }
                _start = tmp;
                _finish = tmp + len;
                end_of_storge = tmp + n;
            }
        }
        void resize(size_t &n, T t = T())
        {
            if (n <= size())
            {
                _finish = _start + n;
            }
            else
            {
                if (n > capacity())
                {
                    reserve(n);
                }
                while (_finish != _start + n)
                {
                    *_finish = t;
                    ++_finish;
                }
            }
        }
        bool empty()
        {
            return _start == _finish;
        }
        iterator begin()
        {
            return _start;
        }
        iterator end()
        {
            return _finish;
        }
        const_iterator begin() const
        {
            return _start;
        }
        const_iterator end() const
        {
            return _finish;
        }
        // iterator &rbegin()
        // {
        //     return _finish - 1;
        // }
        // iterator &rend()
        // {
        //     return _start - 1;
        // }
        //临时对象具有常性
        reverse_iterator rbegin()
        {
            return reverse_iterator(end());
        }
        reverse_iterator rend()
        {
            return reverse_iterator(begin());
        }
        const T& operator[](const size_t &pos) const
        {
            assert(pos < size());
            return *(_start + pos);
        }
        const T& at(size_t& pos) const{
            return (*this)[pos];
        }
        void push_back(const T &t)
        {
            if (_finish == end_of_storge)
            {
                reserve(capacity() == 0 ? 4 : capacity() * 2);
            }
            *_finish = t;
            _finish++;
        }
        void pop_back()
        {
            // 断言防止出错
            assert(!empty());
            --_finish;
        }
        iterator insert(iterator pos, const T &t)
        {
            assert(pos <= _finish);
            assert(pos >= _start);
            if (_finish + 1 > end_of_storge)
            {
                // 迭代器失效:insert之后默认迭代器失效——要么指向错误的位置，要么新开辟空间后指向原来的空间
                size_t len = pos - _start;
                reserve(capacity() == 0 ? 4 : capacity() * 2);
                pos = _start + len;
            }
            iterator end = _finish + 1;
            while (end > pos)
            {
                *end = *(end - 1);
                --end;
            }
            *pos = t;
            ++_finish;
            return pos;
        }
        //同insert一样，调用后pos最好不要访问，在不同平台下实现不同
        iterator erase(iterator pos)
        {
            assert(pos < _finish);
            assert(pos >= _start);
            for (iterator i = pos + 1; i < _finish; ++i)
            {
                *(i - 1) = *i;
            }
            --_finish;
            if(pos==_finish)
            {
                --pos;

                return pos;
            }
            return pos;
        }
        void clear()
        {
            _finish = _start;
        }

        //C++11新增
        template<class... Args>
        iterator emplace(iterator pos,Args&&... args){

        }
        template <class... Args>
        iterator emplace_back(Args &&...args)
        {
        }
        ~vector()
        {
            delete[] _start;
            _start = _finish = end_of_storge = nullptr;
        }

    private:
        iterator _start;
        iterator _finish;
        iterator end_of_storge;
    };

    void Test()
    {
        vector<int> vc;
        vc.push_back(1);
        vc.push_back(2);
        vc.push_back(3);
        vc.push_back(4);
        vc.push_back(5);
        vc.push_back(6);
        auto ite = vc.rbegin();
        while(ite!=vc.rend()){
            std::cout << *ite << " ";
            ++ite;
        }
        std::cout << std::endl;
        // vector<int> vc3;
        // // vc.insert(vc.begin(), 8);
        // const vector<int> vc2(vc);
        // vector<int> vc4;
        // vc4 = vc2;
        // // vector<int> vc2(4, 5);
        // for (int i = 0; i < vc2.size(); ++i)
        // {
        //     std::cout << vc2[i] << " ";
        // }
        // std::cout << std::endl;
        // for (int i = 0; i < vc4.size(); ++i)
        // {
        //     std::cout << vc4[i] << " ";
        // }
        // std::cout << std::endl;
        // for (int i = 0; i < vc.size(); ++i)
        // {
        //     std::cout << vc[i] << " ";
        // }
        // auto pos=vc.erase(vc.begin()+5);
        // std::cout << *pos << std::endl;
        // for (int i = 0; i < vc.size(); ++i)
        // {
        //     std::cout << vc[i] << " ";
        // }
        // vector<int> vc2(3, 0);
    }
}