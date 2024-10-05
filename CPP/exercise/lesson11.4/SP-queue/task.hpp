#pragma once

class Task
{
public:
    Task() {}
    Task(int x, int y, char op) : _x(x), _y(y), _op(op), _result(0), _exitcode(0)
    {
    }
    void operator()()
    {
        switch (_op)
        {
        case '+':
            _result = _x + _y;
            break;
        case '-':
            _result = _x - _y;
            break;
        case '*':
            _result = _x * _y;
            break;
        case '/':
            if (_y == 0)
            {
                _exitcode = 1;
                return;
            }
            else
            {
                _result = _x / _y;
            }
            break;
        case '%':
            if (_y == 0)
            {
                _exitcode = 1;
                return;
            }
            else
            {
                _result = _x % _y;
            }
            break;
        default:
            break;
        }
    }
    ~Task() {}

public:
    int _x;
    int _y;
    char _op;
    int _result;
    int _exitcode;
};