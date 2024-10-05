#pragma once
#include<iostream>
#include<string>
#include<vector>


class Util{
    public:
        void SepStr(const std::string& ret,const std::string& sep, std::vector<std::string>* reqv){
            int start = 0;
            while(start<ret.size()){
                size_t pos = ret.find(sep, start);
                if (pos == std::string::npos)
                    break;
                reqv->push_back(ret.substr(start, pos - start));
                start = pos + sep.size();
            }
            if(start<ret.size()){
                reqv->push_back(ret.substr(start));
            }
        }
        
};