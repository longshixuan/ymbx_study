#include "Util.hpp"
#include "epollServer.hpp"
#include "log.hpp"
#include "widget.hpp"
#include <memory>
#include <stdlib.h>

using namespace ymbx;

Response Cal(const Request &request)
{
    int x = request.getX();
    int y = request.getY();
    char op = request.getOP();
    int result = 0;
    Response resp;
    switch (op)
    {
    case '+':
        result = x + y;
        break;
    case '-':
        result = x - y;
        break;
    case '*':
        result = x * y;
        break;
    case '/':
        if (y == 0)
        {
            resp._code = 1;
        }
        result = x / y;
        break;
    default:
        resp._code = 2;
        break;
    }
    return resp;
}
int main(int args, char *argv[])
{
    if (args != 2)
    {
        Util::Usage(argv[0]);
        LogMessage(FATAL, "lack of parameter");
        exit(ERR_USAGE);
    }
    uint16_t port = atoi(argv[1]);
    std::unique_ptr<EpollServer> ep(new EpollServer(Cal, port));
    ep->InitServer();
    ep->Dispatcher();
    return 0;
}