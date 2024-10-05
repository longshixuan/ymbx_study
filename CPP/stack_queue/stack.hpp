#pragma once
#include<iostream>
#include<vector>


namespace ymbx
{
    template<class T,class Container>
    class stack{
    public:
        size_t size(){
            return _con.size();
        }
        const T& top(){
            return _con.back();
        }
        void push(const T& tmp){
            _con.push_back(tmp);
        }
        void pop(){
            _con.pop_back();
        }
        bool empty(){
            return _con.empty();
        }
    private:
        Container _con;
    };

    void test(){
        stack<int,std::vector<int>> st;
        st.push(1);
        st.push(2);
        st.push(3);
        st.push(4);
        st.push(5);
        while(!st.empty()){
            std::cout<<st.top()<<" ";
            st.pop();
        }
    }
}