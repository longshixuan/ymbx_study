#include<iostream>
#include<typeinfo>
#include "list.hpp"
using namespace std;

class Date
{
public:
    Date():_year(1998),_month(1),_day(1){}
    //添加关键字后不能进行隐式类型转换
    explicit Date(int year,int month,int day):_year(year),_month(month),_day(day)
    {
        // cout << _year << "-" << _month << "-" << _day << endl;
    }
    //只有在成员函数中才能这样使用，该函数中的变量不能被修改，但使用mutable的除外
    void OutPut() const {
        cout << _cnt++<<":"<<_year << "-" << _month << "-" << _day << endl;
    }
    void OutPut2()
    {
        cout << num++ << ":" << _year << "-" << _month << "-" << _day << endl;
    }

private:
    int _year;
    int _month;
    int _day;
    //mutable只能用在类中，
    mutable int _cnt=0;
    int num = 0;
};

// 万能引用（引用折叠）：既可以引用左值，又可以引用右值

// //在传递过程中可能会丢失属性，可以通过完美转发保留对象原生类型属性——————forward<T>(x)
// template <typename T>
// void func(T &&t)
// {
//     fun(t);
// }
// int main()
// {
//     //支持列表初始化——————直接使用花括号进行初始化
//     //initializer_list——————容器构造函数调用的对象
//     int x{2};
//     Date d1(2024, 2, 4);
//     Date d2{2024, 4, 24};
//     // Date d3 = {2024, 4, 30};

//     int y(2.2);
//     //将变量类型声明为表达式指定的类型
//     decltype(x * y) ret = x * y;
//     cout << typeid(ret).name() << endl;

//     //左值——————可以获取地址，可以出现在赋值符号的左边或右边
//     //右值——————只能出现在赋值符号的右边，不能出现在左边，不能对他取地址
    
//      //左值引用只能引用左值
//      //const修饰的左值引用可以引用左值和右值
//      //右值引用只能引用右值
//      //右值引用可以引用move修饰的左值
//      //左值引用减少拷贝次数——————右值引用搭配移动构造使用


//      //右值引用解决的问题————局部对象返回问题，插入接口对象拷贝问题

//      //default————强制生成默认函数
//      //delete————禁止生成默认函数
//      //emplace系列比push_back更高效
     
//     int a = 0;
//     int b = 2;
//     int &ret1 = a;
//     int &&ret2 = a + b;
//     int &&ret3 = move(a);
//     cout << &ret1 << &ret2 << endl;

    
//     return 0;
// }

int main(){
    // int x = 1;
    // // ymbx::auto_ptr<Date> ap1;
    // ymbx::auto_ptr<int> api1(&x);
    // ymbx::auto_ptr<int> api2(api1);
    // cout << "api1:"<<api1.get() << endl;
    // cout << "api2:"<<api2.get() << endl;
    // cout << api1.release() << endl;
    // cout << "api1:" << *api1 << endl;
    // cout << "api2:" << *api2 << endl;
    // api2.reset(new int(2));
    // cout << "api2:" << *api2 << endl;
/////////////////////////////////////////////////////////////////////////
    // int x = 1;
    // // ymbx::auto_ptr<Date> ap1;
    // ymbx::unique_ptr<int> upi1(&x);
    // // ymbx::unique_ptr<int> upi2(upi1);
    // cout << "upi1:" << upi1.get() << endl;
    // // cout << "upi2:" << upi2.get() << endl;
    // cout << "upi1:" << *upi1 << endl;
    // cout << upi1.release() << endl;
    // // cout << "upi1:" << *upi1 << endl;
    // // cout << "upi2:" << *upi2 << endl;
    // upi1.reset(new int(2));
    // cout << "upi1:" << *upi1 << endl;
///////////////////////////////////////////////////////////////////////
    // int* x = new int(1);
    // // ymbx::auto_ptr<Date> ap1;
    // ymbx::shared_ptr<int> spi1(x);
    // ymbx::shared_ptr<int> spi2(spi1);
    // ymbx::shared_ptr<int> spi3 = spi2;
    // cout << "spi1:" << spi1.get() << " cnt:" << spi1.use_count() << endl;
    // cout << "spi2:" << spi2.get() << " cnt:" << spi2.use_count() << endl;
    // // cout << spi1.release() << endl;
    // // cout << "spi1:" << *spi1 << " cnt:" << spi1.use_count() << endl;
    // cout << "spi2:" << *spi2 << " cnt:" << spi2.use_count() << endl;
    // // spi2.reset(new int(2));
    // // cout << "spi2:" << *spi2 << " cnt:" << spi2.use_count() << endl;

/////////////////////////////////////////////////////////////////////////////////
    ymbx::shared_ptr<ymbx::listNode<int>> spl1(new ymbx::listNode<int>);
    ymbx::shared_ptr<ymbx::listNode<int>> spl2(new ymbx::listNode<int>);
    cout << spl1.use_count() << endl;
    cout << spl2.use_count() << endl;
    spl1->_next = spl2;
    spl2->_prev = spl1;
    cout << spl1.use_count() << endl;
    cout << spl2.use_count() << endl;
    return 0;
}
