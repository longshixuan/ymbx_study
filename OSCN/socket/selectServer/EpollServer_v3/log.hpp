#pragma once
#include "widget.hpp"
#include <cstdarg>
#include<cstdio>
#include<iostream>
#include<string>
#include<ctime>

namespace ymbx
{
    //日志等级 时间 内容
    std::string getTime(){
        time_t rtime = time(nullptr);
        struct tm *rettime = localtime(&rtime);
        std::string retstr;
        retstr = std::to_string(rettime->tm_year + 1970) + "/" + std::to_string(rettime->tm_mon+1) + "/" + std::to_string(rettime->tm_mday) + " " + std::to_string(rettime->tm_hour) + ":" + std::to_string(rettime->tm_min) + ":" + std::to_string(rettime->tm_sec);
        return retstr;
    }
    void LogMessage(LOG_LEVEL level, const char *format, ...)
    {
        char buffer[1024];
        va_list vargs;
        va_start(vargs,format);
        vsnprintf(buffer,sizeof(buffer),format,vargs);
        std::string retstr;
        retstr = "[" + std::to_string(level) + "]"+ getTime() + " " + buffer;
        std::cout << retstr << std::endl;
        va_end(vargs);
    }
}