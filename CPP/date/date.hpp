#include <iostream>
using namespace std;



class Date
{
    friend ostream &operator<<(ostream &out, const Date &d);
    friend istream &operator>>(istream &in, Date &d);

private:
    int _year;
    int _month;
    int _day;

public:
    Date(){}
    Date(int year, int month, int day) : _year(year), _month(month), _day(day){}
    Date(const Date& d){
        _year = d._year;
        _month = d._month;
        _day = d._day;
    }
    int getMonthday(int year, int month) const
    {
        int monthday[13] = {0, 31, 28, 31, 30, 31, 30, 31, 31, 30, 31, 30, 31};
        if (month == 2 && ((year % 4 == 0 && year % 100 != 0) || year % 400 == 0))
        {
            return 29;
        }
        return monthday[month];
    }
    int days(const Date& d) const{
        int cnt = 0;
        if(d._year>2000){
            for (int i = 2000; i < d._year; i++)
            {
                if ((i % 4 == 0 && i % 100 != 0) || i % 400 == 0)
                {
                    cnt += 366;
                }
                else
                {
                    cnt += 365;
                }
            }
        }
        if(d._month>1){
            for (int i = 1; i < d._month;i++){
                cnt += getMonthday(d._year, i);
            }
        }
        if(d._day>1){
            cnt += (d._day - 1);
        }
        return cnt;
    }
    void printDate() const{
        cout << _year << "/" << _month << "/" << _day << endl;
    }
    //对基本运算符的重载
    bool operator>(const Date& d) const{
        return (_year > d._year) || (_year == d._year && _month > d._month) || (_year == d._year && _month == d._month && _day > d._day);
    }
    bool operator==(const Date& d) const{
        return _year == d._year && _month == d._month && _day == d._day;
    }
    bool operator>=(const Date& d)const{
        return (*this > d) || (*this == d);
    }
    bool operator<(const Date &d)const
    {
        return !(*this>=d);
    }
    bool operator<=(const Date &d)const
    {
        return !(*this > d);
    }
    bool operator!=(const Date &d)const
    {
        return !(*this == d);
    }
    Date& operator+=(const int &x)
    {
        _day += x;
        while (_day > getMonthday(_year, _month))
        {
            _day -= getMonthday(_year, _month);
            _month++;
            if (_month == 13)
            {
                _year++;
                _month = 1;
            }
        }
        return *this;
    }
    Date operator+(const int& x)const
    {
        Date tmp(*this);
        //代码复用
        tmp += x;
        // tmp._day += x;
        // while (tmp._day > getMonthday(tmp._year, tmp._month))
        // {
        //     tmp._day -= getMonthday(tmp._year, tmp._month);
        //     tmp._month++;
        //     if (tmp._month == 13)
        //     {
        //         tmp._year++;
        //         tmp._month = 1;
        //     }
        // }
        return tmp;
    }
    //++a
    Date& operator++(){
        (*this) += 1;
        return *this;
    }
    //a++
    Date operator++(int){
        Date tmp(*this);
        ++(*this);
        return tmp;
    }
    Date& operator-=(const int& x){
        _day -= x;
        while (_day <= 0)
        {
            _month--;
            if (_month == 0)
            {
                _year--;
                _month = 12;
            }
            _day += getMonthday(_year, _month);
        }
        return *this;
    }
    Date operator-(const int& x)const{
        Date tmp(*this);
        tmp -= x;
        return tmp;
    }
    //--a
    Date& operator--(){
        (*this) -= 1;
        return *this;
    }
    //a--
    Date operator--(int){
        Date tmp(*this);
        --(*this);
        return tmp;
    }
    int operator-(const Date& d)const{
        int gapdays = 0;
        if (*this > d)
        {
            gapdays = days(*this) - days(d);
        }else{
            cout << "输入非法" << endl;
        }
        return gapdays;
    }

};
ostream& operator<<(ostream& out,const Date& d)
{
    out << d._year << "/" << d._month << "/" << d._day << endl;
}
istream& operator>>(istream& in,Date& d)
{
    in >> d._year >> d._month >> d._day;
}