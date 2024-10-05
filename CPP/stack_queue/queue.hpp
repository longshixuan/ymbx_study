#pragma once
#include <iostream>
#include <vector>
#include<list>


namespace ymbx
{
    template <class T, class Container=std::vector<T>>
    class queue
    {
    public:
        size_t size()
        {
            return _con.size();
        }
        const T &front()
        {
            return _con.front();
        }
        const T& back(){
            return _con.back();
        }
        void push(const T &tmp)
        {
            _con.push_back(tmp);
        }
        void pop()
        {
            // _con.pop_front();
            _con.erase(_con.begin()); 
        }
        bool empty()
        {
            return _con.empty();
        }

    private:
        Container _con;
    };

    void test()
    {
        queue<int> st;
        st.push(1);
        st.push(2);
        st.push(3);
        st.push(4);
        st.push(5);
        while (!st.empty())
        {
            std::cout << st.front() << " ";
            st.pop();
        }
    }
}