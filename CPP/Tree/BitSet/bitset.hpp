#pragma once
#include <iostream>
#include<string>
#include<vector>

namespace ymbx{
    template <size_t N>
    class BitSet
    {
    public: 
        BitSet(){
            _bs.resize(N / 8 + 1, 0);
        }
        void set(const size_t& x){
            size_t a = x / 8;
            size_t b = x % 8;
            _bs[a] |= (1 << b);
        }
        
        void reset(const size_t& x){
            size_t a = x / 8;
            size_t b = x % 8;
            (_bs[a] >> b) &= (0xFE);
        }
        bool isExist(const size_t & x){
            size_t a = x / 8;
            size_t b = x % 8;
            if(((_bs[a]>>b) & 1)==1){
                return true;
            }
            return false;
        }

    private:
        std::vector<char> _bs;
    };

    void test(){
        BitSet<100> bs;
        bs.set(22);
        bs.set(50);
        bs.set(12);
        bs.set(20);
        std::cout << "20:" << bs.isExist(20) << std::endl;
        std::cout << "21:" << bs.isExist(21) << std::endl;
    }
}