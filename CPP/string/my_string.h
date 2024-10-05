#pragma once
#include <cassert>
#include <cstring>
#include <iostream>

namespace ymbx
{
    const size_t npos=-1;
    class string
    {
    private:
        char *_str;
        int _size;
        int _capacity;
        // 只有size_t可以这样写，其他必须在类内声明，在类外定义
        static const size_t npos;

    public:
        friend std::ostream &operator<<(std::ostream &out, const string &str);
        friend std::istream &operator>>(std::istream &in, string &str);
        typedef char *Iterator;
        typedef const char *const_Iterator;
        // string():_str(nullptr),_size(0),_capaticy(0){}
        string(const char *str = "\0") : _size(strlen(str))
        {
            _str = new char[strlen(str) + 1];
            strcpy(_str, str);
            _capacity = _size;
            _str[_size] = '\0';
        }
        size_t size() const
        {
            return _size;
        }
        size_t capacity() const
        {
            return _capacity;
        }
        string(const string &str) : _size(str._size), _capacity(str._capacity)
        {
            _str = new char[str._capacity + 1];
            strcpy(_str, str._str);
            _str[_size] = '\0';
        }
        string &operator=(const string &str)
        {
            char *tmp = new char[str._capacity + 1];
            strcpy(tmp, str._str);
            delete[] _str;
            _str = tmp;
            _size = str._size;
            _capacity = str._capacity;
            _str[_size] = '\0';
            return *this;
        }
        const char &operator[](const int &x) const
        {
            // 断言
            assert(x < _size);
            return _str[x];
        }
        char &operator[](const size_t &x)
        {
            // 断言
            assert(x < _size);
            return _str[x];
        }
        char &at(const size_t &pos)
        {
            assert(pos < _size);
            return _str[pos];
        }
        const char &at(const size_t &pos) const
        {
            assert(pos < _size);
            return _str[pos];
        }
        void reserve(const size_t &x)
        {
            if (_capacity < x)
            {
                char *tmp = new char[x + 1];
                strcpy(tmp, _str);
                _capacity = x;
                _str = tmp;
                _str[_size] = '\0';
            }
        }
        void resize(const size_t &x, char ch = '\0')
        {
            if (_size >= x)
            {
                _str[x] = '\0';
            }
            else
            {
                //没有考虑到_size和_capacity不相等的情况
                if(x>_capacity)
                {
                    reserve(x);
                }
                for (int i = _size; i < x; ++i)
                {
                    _str[i] = ch;
                }
            }
            _size = x;
            _str[_size] = '\0';
        }
        void clear()
        {
            _str[0] = '\0';
            _size = 0;
        }
        bool empty() const
        {
            return _size;
        }
        Iterator begin()
        {
            return _str;
        }
        Iterator end()
        {
            return _str + _size;
        }
        const_Iterator begin() const
        {
            return _str;
        }
        const_Iterator end() const
        {
            return _str + _size;
        }
        void push_back(const char &ch)
        {
            if (_size + 1 > _capacity)
            {
                reserve(_capacity * 2);
            }
            _str[_size] = ch;
            ++_size;
            _str[_size] = '\0';
        }
        string &append(const char *str)
        {
            if (_size + strlen(str) > _capacity)
            {
                reserve(_size + strlen(str));
            }
            strcpy(_str + _size, str);
            _size += strlen(str);
            _str[_size] = '\0';
            return *this;
        }
        string &append(const string &str)
        {
            return append(str._str);
        }
        string &operator+=(const string &str)
        {
            return append(str);
        }
        string &operator+=(const char *str)
        {
            return append(str);
        }
        string &operator+=(const char ch)
        {
            push_back(ch);
            return *this;
        }
        const char *c_str() const
        {
            return _str;
        }
        bool operator>(const string &str) const
        {
            return strcmp(_str, str._str) > 0 ? true : false;
        }
        bool operator==(const string &str) const
        {
            return strcmp(_str, str._str) == 0 ? true : false;
        }
        bool operator>=(const string &str) const
        {
            return (*this == str) || (*this > str);
        }
        // 其他比较运算符重载省略
        string &insert(size_t pos, const char &ch)
        {
            assert(pos <= _size);
            // char *tmp = new char[_size + 1];
            //比较时类型不同时会发生类型提升，int——>unsigned——int
            if (_size + 1 > _capacity)
            {
                reserve(_capacity * 2);
            }
            // strcpy(tmp, _str);
            // _str[pos] = ch;
            // strcpy(_str + pos + 1, tmp + pos);
            // _size += 1;
            size_t end = _size + 1;
            while(end>pos)
            {
                _str[end] = _str[end - 1];
                --end;
            }
            _str[pos] = ch;
            _size += 1;
            return *this;
        }
        string &insert(size_t pos, const string &str)
        {
            assert(pos <= _size);
            // char *tmp = new char[_size + 1];
            if (_size + strlen(str._str) > _capacity)
            {
                reserve(_size + strlen(str._str));
            }
            // strcpy(tmp, _str);
            // strcpy(_str + pos, str._str);
            // strcpy(_str + pos + strlen(str._str), tmp + pos);
            // _size += strlen(str._str);
            size_t end = _size + strlen(str._str);
            while(end>pos+strlen(str._str)-1)
            {
                _str[end] = _str[end - strlen(str._str)];
                --end;
            }
            strncpy(_str + pos, str._str, strlen(str._str));
            _size += strlen(str._str);
            return *this;
        }
        string &erase(size_t pos,size_t len=npos)
        {
            if(pos+len>=_size)
            {
                size_t tmp = pos;
                while (tmp < _size)
                {
                    _str[tmp++] = '\0';
                }
            }
            size_t end = pos + len;
            while(end<_size)
            {
                _str[pos++] = _str[end++];
            }
            _size = pos;
            return *this;
        }
        size_t find(const string &str, size_t pos=0) const
        {
            assert(pos < _size);
            //使用strstr(主串，子串)来实现
            for (size_t i = pos; i < _size; ++i)
            {
                if (strncmp(_str + i, str._str, str._size) == 0)
                {
                    return i;
                }
            }
            return npos;
        }
        ~string()
        {
            delete[] _str;
            _str = nullptr;
            _capacity = _size = 0;
        }

        
    };
    std::ostream &operator<<(std::ostream &out, const string &str)
    {
        //此种方法当插入字符中函数\0时会出错
        // out << str._str;
        for(auto ch:str)
        {
            out << ch;
        }
        return out;
    }
    //cin和scanf从键盘获取字符时遇到‘ ’会停止，但getline可以获取到‘ ’
    std::istream& operator>>(std::istream& in,string& str)
    {
        char ch = in.get();
        char buffer[128];
        int index=0;
        while(ch!= ' ' && ch!='\n')
        {
            buffer[index++] = ch;
            if(index==127)
            {
                buffer[127]='\0';
                str+=buffer;
                index=0;
            }
            ch = in.get();
        }
        if(index!=0)
        {
            buffer[index]='\0';
            str+=buffer;
        }       
        return in;
    }


    void testString()
    {
        string s1 = "hello world";
        string s2 = s1;
        string s3(s2);
        // s1 += '\0';
        // s1 += "aaaaa";
        string s4;
        std::cin >> s4;
        // s1.erase(0, 2);
        // s2.erase(0, 11);
        std::cout << s4 << s4.size() << std::endl;
        // std::cout << s2 << s2.size()<<std::endl;
        // string s4 = "orl";
        // string s5 = "ord";
        // std::cout << s1.find(s4, 1) << std::endl;
        // std::cout << s1.find(s4, 8) << std::endl;
        // std::cout << s2.find(s5, 1) << std::endl;
        // std::cout << s1 << std::endl;
        // std::cout << s2 << std::endl;
        // std::cout << s3 << std::endl;
        // s1 = s1;
        // std::cout << s1 << std::endl;
        // for (int i = 0; i < s1.size(); ++i)
        // {
        //     s1[i]++;
        //     std::cout << s1 << ",size:" << s1.size() << ",capacity:" << s1.capacity() << std::endl;
        // }
        // s1.insert(0, 'a');
        // s1.insert(2, 'b');
        // s1.insert(5, 'c');
        // s2.insert(0, "aaaaa");
        // s2.insert(3, "bbbbb");
        // s2.insert(5, "ccccc");
        // std::cout << s1 <<s1.size()<< std::endl;
        // std::cout << s2 << s2.size()<<std::endl;
        // for (auto ch : s1)
        // {
        //     (*ch)++;
        //     ch++;
        //     std::cout << s1 << "size:" << s1.size() << "capacity:" << s1.capacity() << std::endl;
        // }
        // s1.push_back('a');
        // s2.append(s1);
        // s3.append("aaaaa");
        // s1 += s2;
        // std::cout << s1 << std::endl;
        // std::cout << s2 << std::endl;
        // std::cout << s3 << std::endl;
        // s1.reserve(3);
        // s2.resize(2, 'a');
        // s3.resize(2);
        // s1.clear();
        // std::cout << s1 << "---" << s1.capacity() << "---" << s1.size() << std::endl;
        // std::cout << s2 << "---" << s2.capacity() << "---" << s2.size() << std::endl;
        // std::cout << s3 << "---" << s3.capacity() << "---" << s3.size() << std::endl;
    }
}
