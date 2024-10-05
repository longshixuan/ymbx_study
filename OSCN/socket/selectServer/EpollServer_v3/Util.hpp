#pragma once
#include <iostream>
#include <string>
#include<unistd.h>
#include<fcntl.h>
#include "log.hpp"
#include<errno.h>
#include<cstring>
#include<vector>


namespace ymbx
{
    namespace Util{
    void Usage(std::string mes)
    {
        std::cout << "prompt:\n\t" << mes << " port" << std::endl;
    }

    void SetNoBlock(int fd){
        int n = fcntl(fd, F_GETFL);
        if(n<0){
            LogMessage(WARNING, "set no block error,error code:%d,error message:%s", errno, strerror(errno));
            exit(ERR_SETNOBLOCK);
        }
        fcntl(fd, F_SETFL, n | O_NONBLOCK);
    }

    void SepStr(const std::string &ret, const std::string &sep, std::vector<std::string> *reqv)
    {
        int start = 0;
        while (start < ret.size())
        {
            size_t pos = ret.find(sep, start);
            if (pos == std::string::npos)
                break;
            reqv->push_back(ret.substr(start, pos - start));
            start = pos + sep.size();
        }
        if (start < ret.size())
        {
            reqv->push_back(ret.substr(start));
        }
    }
    }
}