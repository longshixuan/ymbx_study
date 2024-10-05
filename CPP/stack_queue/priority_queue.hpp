#pragma once
#include<vector>
#include<iostream>
#include<functional>
#include<deque>
using namespace std;


namespace ymbx
{
    template <class T, class Container = vector<T>,class Compare = less<T>>
    class priority_queue
    {
        typedef vector<T> *Iterator;
    private:
        Compare _comp;
        struct less
        {
            bool operator()(const T& x,const T& y){
                return x < y;
            }
        };
        struct greater
        {
            bool operator()(const T &x, const T &y)
            {
                return x > y;
            }
        };
        void adjust_up(size_t idx)
        {
            while(idx>0){
                if (_comp(_con[(idx - 1) / 2],_con[idx]))
                {
                    swap(_con[idx], _con[(idx - 1) / 2]);
                }
                idx = (idx - 1) / 2;
            }
        }
        void adjust_down(size_t idx){
            while ((2 * idx + 2) < _con.size())
            {
                if (_comp(_con[idx],max(_con[2 * idx + 1], _con[2 * idx + 2])))
                {
                    if (_comp(_con[2 * idx + 2],_con[2 * idx + 1]))
                    {
                        swap(_con[idx], _con[2 * idx + 1]);
                        idx = 2 * idx + 1;
                        continue;
                    }
                    else
                    {
                        swap(_con[idx], _con[2 * idx + 2]);
                        idx = 2 * idx + 2;
                        continue;
                    }
                }
                else{
                    break;
                }
            }
        }

    public:
        priority_queue(){}
        priority_queue(Iterator& start,Iterator& end):_con(start,end){}
        size_t size(){
            return _con.size();
        }
        void push(const T& t){
            _con.push_back(t);
            adjust_up(size()-1);
        }
        void pop(){
            swap(_con[0], _con[_con.size()-1]);
            _con.pop_back();
            adjust_down(0);
        }
        bool empty(){
            return _con.empty();
        }
        const T& top(){
            return _con.front();
        }
    private:
        Container _con;
        // Compare _comp;
    };
    void test(){
        priority_queue<int,deque<int>,less<int>> pq;
        pq.push(1);
        pq.push(2);
        pq.push(3);
        pq.push(4);
        pq.push(5);
        pq.push(6);
        pq.push(7);
        while(!pq.empty()){
            cout << pq.top() << " ";
            pq.pop();
        }
        cout << endl;
    }
}