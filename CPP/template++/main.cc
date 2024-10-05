// 模板参数包括类型模板参数和非类型模板参数
// 非类型模板参数只能是整型变量————int,size_t,char,short,long,long long
#include "date.hpp"
#include <array>
#include <iostream>
using namespace std;

// //模板相关知识
// //函数模板特化不允许这样实现————这可以通过对函数overload实现
// // template<class Date>
// // bool Less(const Date& d1,const Date& d2){
// //     return d1 < d2;
// // }

// // template <>
// // bool Less<Date *>(Date *d1, Date *d2)
// // {
// //     return *d1 < *d2;
// // }

// //仿函数类模板
// template<class T>
// struct Less{
//     bool operator()(const T& d1,const T& d2) const
//     {
//         return d1 < d2;
//     }
// };
// //模板全特化
// template<>
// struct Less<Date*>{
//     bool operator()(const Date* pd1,const Date* pd2) const
//     {
//         return *pd1 < *pd2;
//     }
// };
// // 模板半特化
// template <class T>
// struct Less<T *>
// {
//     bool operator()(const T *pd1, const T *pd2) const
//     {
//         return *pd1 < *pd2;
//     }
// };
// int main()
// {
//     // //C语言中的数组对越界等只进行抽查，而C++11中新增的会对越界等进行全部严格检查
//     // int arr[10];
//     // arr[15];
//     // arr[10] = 1;
//     ////非类型模板参数的实例
//     // array<int, 10> a1;
//     // a1[10];
//     // a1[15] = 1;

//     //模板特化————对某些类型进行特殊化处理
//         //分为全特化（某一具体类型）和偏特化（限制某一类类型）
//     Date d1(2024,3,1);
//     Date d2(2034,4,1);
//     cout << Less<Date>()(d1, d2) << endl;
//     Date *pd1 = &d1;
//     Date *pd2 = &d2;
//     cout << Less<Date*>()(pd1, pd2) << endl;
//     //模板不能分离编译————因编译时没有实例化而不会写进符号表，在链接时会报错
//         //解决方式：①显示实例化：在模板文件下显示声明一下，但麻烦
//                 //②将声明和定义放在一个文件中
//     return 0;
// }

////////////////////////////////////////////

// 继承

// 子类和基类对象之间的赋值转换————子类可以赋值给父类，但父类不能赋值给子类
// 子类和基类有独立的作用域，子类可以对父类的成员进行重定义，也可以通过添加域限定符访问基类成员
// 子类和基类同名函数构成隐藏————不建议子类和基类函数同名
// 子类析构函数调用之后会默认调用父类析构函数
//
// 实现一个不能被继承的类————将构造和析构函数设为private

// 继承过程中可能会出现菱形继承，可能会导致二义性和数据冗余
// 初始化顺序与继承顺序有关，与初始化列表的顺序无关
// 组合的耦合程度低，继承与组合优先选择组合实现

////////////////////////////////////////////////
// 多态：不同的对象完成同一种行为会有不同的结果
// 条件————1.虚函数的重写，2.去调用父类的指针或引用

// 底层是通过给类添加一个虚表指针指向一个虚函数表来实现的————在VS下虚函数表以nullptr结束，g++下则不是
// 虚表在编译过程中生成
// 虚表指针在初始化列表里被初始化
// 虚表被存放在代码段
// 多继承中子类的虚函数重写后只会放在第一个声明的位置
// 虚基表指针指向的虚基表中存储了虚表的偏移量和相对于基类的偏移量

// class Person{
// public:
//     virtual void Buyticket(){
//         cout << "全票" << endl;
//     }
// };
// class Student:public Person{
// public:
//     virtual void Buyticket(){
//         cout << "半票" << endl;
//     }
// };

// void Func(Person& p){
//     p.Buyticket();
// }
// int main()
// {
//     Person p;
//     Student s;
//     p.Buyticket();
//     s.Buyticket();
//     cout << sizeof(p) << endl;
//     return 0;
// }

class A
{
public:
    virtual void func(int val = 1) { std::cout << "A->" << val << std::endl; }
    virtual void test() { func(); }
};

class B : public A

{
public:
    void func(int val = 0) { std::cout << "B->" << val << std::endl; }
};

int main(int argc, char *argv[])

{
    B *p = new B;
    p->test();
    return 0;
}
