#pragma once
#include<iostream>
#include<string>
#include<sys/types.h>
#include<unistd.h>
#include<cstdarg>
#include<cstdio>
#include<time.h>
#include "err.hpp"

enum logLevel
{
    Debug=1,
    Info,
    Warning,
    Error,
    Fatal
};

class LogMessage{
    public:
        LogMessage(logLevel loglev,const std::string filename):_loglev(loglev),_filename(filename) {}
        std::string getTime(){
            time_t rettime = time(nullptr);
            // int tm_sec;   /* Seconds.	[0-60] (1 leap second) */
            // int tm_min;   /* Minutes.	[0-59] */
            // int tm_hour;  /* Hours.	[0-23] */
            // int tm_mday;  /* Day.		[1-31] */
            // int tm_mon;   /* Month.	[0-11] */
            // int tm_year;  /* Year	- 1900.  */
            // int tm_wday;  /* Day of week.	[0-6] */
            // int tm_yday;  /* Days in year.[0-365]	*/
            // int tm_isdst; /* DST.		[-1/0/1]*/
            struct tm *ret = localtime(&rettime);
            std::string retstr;
            retstr = std::to_string(ret->tm_year + 1900) + "/" + std::to_string(ret->tm_mon + 1) + "/" + std::to_string(ret->tm_mday) + " " + std::to_string(ret->tm_hour) + ":" + std::to_string(ret->tm_min)+":"+std::to_string(ret->tm_sec);
            // std::cout << retstr << std::endl;
            return retstr;
        }
        std::string PrintLevel(){
            switch (_loglev)
            {
            case Debug:
                return "Debug";
                break;
            case Info:
                return "Info";
                break;
            case Warning:
                return "Warning";
                break;
            case Error:
                return "Error";
                break;
            case Fatal:
                return "Fatal";
                break;
            default:
                break;
            }
        }
        //日志打印：错误等级 pid 时间 详细错误信息
        //va_start只能出现在具有变长参数的函数中
        void RecordMessage(const char* errmes,...){
            // std::string buffer;
            char buffer[1024];
            va_list p;
            va_start(p, errmes);
            // vsnprintf(const_cast<char *>(buffer.c_str()), buffer.size(), errmes, p);
            vsnprintf(buffer, sizeof(buffer), errmes, p);
            va_end(p);
            _message = "[" + PrintLevel() + "]" + " " + std::to_string(getpid()) + " " + getTime() + "--->>>concrete message:" + buffer;
            // std::cout << _message << std::endl;
            FILE *fd = fopen(_filename.c_str(), "a");
            if(fd==nullptr){
                exit(OPEN_FILE_ERR);
            }
            // std::cout << "open" << std::endl;
            fprintf(fd, _message.c_str());
            fclose(fd);
            // std::cout << "close" << std::endl;
        }
    public:
        void operator()(const char* errmes,...){
            char buffer[1024];
            va_list p;
            va_start(p, errmes);
            // vsnprintf(const_cast<char *>(buffer.c_str()), buffer.size(), errmes, p);
            vsnprintf(buffer, sizeof(buffer), errmes, p);
            va_end(p);
            _message = "[" + PrintLevel() + "]" + " " + std::to_string(getpid()) + " " + getTime() + "--->>>concrete message:" + buffer+'\n';
            // std::cout << _message << std::endl;
            FILE *fd = fopen(_filename.c_str(), "a");
            if (fd == nullptr)
            {
                exit(OPEN_FILE_ERR);
            }
            // std::cout << "open" << std::endl;
            fprintf(fd, _message.c_str());
            fclose(fd);
            // std::cout << "close" << std::endl;
        }
        ~LogMessage() {}
    private:
        logLevel _loglev;
        std::string _message;
        std::string _filename;
};
