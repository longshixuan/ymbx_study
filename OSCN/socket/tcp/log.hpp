#pragma once

#include <cstdio>
#include <iostream>
#include <stdarg.h>
#include <string>
#include <sys/types.h>
#include <time.h>
#include <unistd.h>

namespace ymbx
{
    std::string filename = "log.txt";

    // 日志等级
    enum logLevel
    {
        Debug = 0,
        Info = 1,
        Warning,
        Error,
        Fatal,
        Unkown
    };

    std::string getTime()
    {
        time_t cur = time(nullptr);
        struct tm *loctm = localtime(&cur);
        // int tm_sec;   /* Seconds.	[0-60] (1 leap second) */
        // int tm_min;   /* Minutes.	[0-59] */
        // int tm_hour;  /* Hours.	[0-23] */
        // int tm_mday;  /* Day.		[1-31] */
        // int tm_mon;   /* Month.	[0-11] */
        // int tm_year;  /* Year	- 1900.  */
        // int tm_wday;  /* Day of week.	[0-6] */
        // int tm_yday;  /* Days in year.[0-365]	*/
        // int tm_isdst; /* DST.		[-1/0/1]*/

        char buffer[128];
        snprintf(buffer, sizeof(buffer), "%d-%d-%d %d:%d:%d", loctm->tm_year + 1900, loctm->tm_mon + 1, loctm->tm_mday, loctm->tm_hour, loctm->tm_min, loctm->tm_sec);
        return buffer;
    }
    // 可变参数
    void logMessage(logLevel level, const char *format, ...)
    {
        // 打印格式：错误类型，时间，进程，错误信息
        std::string logMess;
        logMess = "[" + std::to_string(level) + "] " + getTime() + " " + std::to_string(getpid()) + "---";
        // C语言获取可变参数列表的操作
        char buffer[1024];
        va_list p;
        va_start(p, format);
        vsnprintf(buffer, sizeof(buffer), format, p);
        va_end(p);
        logMess += buffer;
        logMess += '\n';
        // 文件重定向到日志文件中
        FILE *log = fopen(filename.c_str(), "a");
        fprintf(log, logMess.c_str());
        fclose(log);
        // std::cout << logMess << std::endl;
    }
};