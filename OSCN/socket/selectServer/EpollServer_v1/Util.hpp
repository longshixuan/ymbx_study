#pragma once
#include <iostream>
#include <string>

namespace ymbx
{
    void Usage(std::string mes)
    {
        std::cout << "prompt:\n\t" << mes << " port" << std::endl;
    }
}