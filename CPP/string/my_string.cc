#include <iostream>
#include <string>
#include "my_string.h"

int main()
{
    ymbx::testString();


    
    //不同编译器会对string有不同类型的实现——
        //VS下会多开辟一个十六字节的数组，所以总大小为28byte
        //Linux下为一个指针指向存放该string对象
    //ymbx::string s1;
    //std::string s2;
    //std::cout<<sizeof(s1)<<std::endl;
    //std::cout<<sizeof(s2)<<std::endl;
    return 0;
}
