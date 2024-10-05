#include <iostream>
#include <typeinfo>



class Date
{
public:
    /* data */
    //声明时可以给缺省值————C++11进行的补充
    // int _year =1;
    // int _month =1 ;
    // int _day=1;

    int _year;
    int _month;
    int _day;

public:
    Date();
    Date(int year, int month, int day) : _year(year), _month(month), _day(day){};
    Date(const Date& date){
        _day = date._day;
        _month = date._month;
        _year = date._year;
    }
    int getMonthday(int year,int month)
    {
        int monthday[13] = {0, 31, 28, 31, 30, 31, 30, 31, 31, 30, 31, 30, 31};
        if(month ==2 && ((year%4==0 && year%100!=0) || year%400==0)){
            return 29;
        }
        return monthday[month];
    }
    Date cuurentDay(const int& x){
        Date tmp(*this);
        tmp._day += x;
        while (tmp._day > getMonthday(tmp._year, tmp._month))
        {
            tmp._day -= getMonthday(tmp._year, tmp._month);
            tmp._month++;
            if (tmp._month == 13)
            {
                tmp._year++;
                tmp._month = 1;
            }
        }
        return tmp;
    }
    // ~demo(){};
};


// int a = 0;
// int Add(int a , int b){
//     int c = a + b;
//     return c;
// }
int main()
{
    // int ret=Add(1,2);
    // std::cout << ret << std::endl;
    // auto a = ret;
    // std::cout << typeid(a).name() << std::endl;
    // int array[] = {1,2,3,4,5};
    // //语法糖————依次取array中的元素赋值给e对象
    // for(auto e:array){
    //     std::cout << e << std::endl;
    // }
    Date d1(2023,10,26);
    // Date d2(d1);
    Date d2 = d1.cuurentDay(100);
    std::cout << d1._year << "/" << d1._month << "/" << d1._day << std::endl;
    std::cout << d2._year << "/" << d2._month << "/" << d2._day << std::endl;
    return 0;
}