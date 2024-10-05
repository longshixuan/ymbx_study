#pragma once
#include<iostream>
#include<mutex>

namespace ymbx{

// 管理权转移——————————C++98智能指针实现方式
    template<class T>
    class auto_ptr{
    private:
        T *_ptr;
    public:
        auto_ptr(T* ptr):_ptr(ptr){}
        auto_ptr(auto_ptr<T>& p):_ptr(p._ptr){
            p._ptr = nullptr;
        }
        auto_ptr<T>& operator=(auto_ptr<T>& p){
            if(this!=&p){
                if(_ptr){
                    delete _ptr;
                }
                _ptr = p._ptr;
                p._ptr = nullptr;
            }
            return *this;
        }
        T* get() const {
            return _ptr;
        }
        T* release(){
            T *tmp = _ptr;
            _ptr = nullptr;
            return tmp;
        }
        void reset(T* ptr=0){
            if(_ptr){
                delete _ptr;
            }
            _ptr = ptr;
            return;
        }
        T& operator*(){
            return *_ptr;
        }
        T* operator->(){
            return _ptr;
        }
        ~auto_ptr(){
           if(_ptr){
                delete _ptr;
            }
        }
    };

//C++11中智能指针的实现，从BOOST库中演化发展而来，包括unique_ptr,shared_ptr,weak_ptr
    //unique_ptr————简单防拷贝
    template<class T>
    class unique_ptr{
    private:
        T *_ptr;
    public:
        unique_ptr(T* ptr):_ptr(ptr){}
        unique_ptr(const unique_ptr<T> &t) = delete;
        unique_ptr<T>& operator=(const unique_ptr<T> &t) = delete;
        T* get(){
            return _ptr;
        }
        T* release(){
            if(_ptr){
                T *tmp = _ptr;
                _ptr = nullptr;
                return tmp;
            }
        }
        void reset(T* ptr=0){
            if(_ptr){
                delete _ptr;
            }
            _ptr = ptr;
            return;
        }
        T& operator*(){
            return *_ptr;
        }
        T* operator->(){
            return _ptr;
        }
        ~unique_ptr(){
            if(_ptr){
                delete _ptr;
            }
        }
    };
    //shared_ptr————通过引用计数的方式来控制
    template<class T>
    class shared_ptr{
        //模板类的友元
        template<class X> friend class weak_ptr;

    private:
        T *_ptr;
        int *_cnt;
        std::mutex *_pmtx;

    public:
        shared_ptr(T* ptr):_ptr(ptr),_cnt(new int(1)),_pmtx(new std::mutex){}
        shared_ptr(shared_ptr<T>& sp):_ptr(sp._ptr),_cnt(sp._cnt),_pmtx(sp._pmtx)
        {
            Obtain();
        }
        shared_ptr<T> &operator=(shared_ptr<T> &sp)
        {
            if(_ptr!=sp._ptr){
                Relief();
                _ptr = sp._ptr;
                _cnt = sp._cnt;
                _pmtx = sp._pmtx;
                Obtain();
            }
            return *this;
        }
        void Obtain(){
            Lock();
            ++(*_cnt);
            UnLock();
        }
        void Relief(){
            Lock();
            bool flag = false;
            if ((--(*_cnt)) == 0 && _ptr)
            {
                std::cout << "delete" << *_cnt << std::endl;
                delete _cnt;
                delete _ptr;
                flag = true;
            }
            UnLock();
            if(flag){
                delete _pmtx;
            }
        }
        void Lock(){
            _pmtx->lock();
        }
        void UnLock(){
            _pmtx->unlock();
        }
        int use_count(){
            return *_cnt;
        }
        T* get()
        {
            return _ptr;
        }
        T* operator->(){
            return _ptr;
        }
        T& operator*(){
            return *_ptr;
        }
        ~shared_ptr(){
            Relief();
        }
    };

    // template <class T>
    // class shared_ptr
    // {
    // public:
    //     shared_ptr(T *ptr = nullptr)
    //         : _ptr(ptr), _pRefCount(new int(1)), _pmtx(new std::mutex)
    //     {
    //     }
    //     shared_ptr(const shared_ptr<T> &sp)
    //         : _ptr(sp._ptr), _pRefCount(sp._pRefCount), _pmtx(sp._pmtx)
    //     {
    //         AddRef();
    //     }
    //     void Release()
    //     {
    //         _pmtx->lock();
    //         bool flag = false;
    //         if (--(*_pRefCount) == 0 && _ptr)
    //         {
    //             std::cout << "delete:" << _ptr << std::endl;
    //             delete _ptr;
    //             delete _pRefCount;
    //             flag = true;
    //         }
    //         _pmtx->unlock();
    //         if (flag == true)
    //         {
    //             delete _pmtx;
    //         }
    //     }
    //     void AddRef()
    //     {
    //         _pmtx->lock();
    //         ++(*_pRefCount);
    //         _pmtx->unlock();
    //     }
    //     shared_ptr<T> &operator=(const shared_ptr<T> &sp)
    //     {
    //         // if (this != &sp)
    //         if (_ptr != sp._ptr)
    //         {
    //             Release();
    //             _ptr = sp._ptr;
    //             _pRefCount = sp._pRefCount;
    //             _pmtx = sp._pmtx;
    //             AddRef();
    //         }
    //         return *this;
    //     }
    //     int use_count()
    //     {
    //         return *_pRefCount;
    //     }
    //     ~shared_ptr()
    //     {
    //         Release();
    //     }
    //     // 像指针一样使用
    //     T &operator*()
    //     {
    //         return *_ptr;
    //     }
    //     T *operator->()
    //     {
    //         return _ptr;
    //     }
    //     T *get() const
    //     {
    //         return _ptr;
    //     }

    // private:
    //     T *_ptr;
    //     int *_pRefCount;
    //     std::mutex *_pmtx;
    // };

    // weak_ptr————解决shared_ptr存在的循环引用的问题
    template<class T>
    class weak_ptr{
    private:
        T *_ptr;
    public:
        weak_ptr(T* ptr):_ptr(ptr){}
        weak_ptr(const shared_ptr<T>& sp):_ptr(sp._ptr){}
        weak_ptr<T>& operator=(const shared_ptr<T>& sp){
            _ptr = sp._ptr;
            return *this;
        }
        T* operator->(){
            return _ptr;
        }
        T& operator*(){
            return *_ptr;
        }
        ~weak_ptr() = default;
    };
}