#include<iostream>
using namespace std;

class A
{
public:
    A(){}
    void Print()
    {
        cout << _a1 << " " << _a2 << endl;
    }

private:
    int _a2;
    int _a1;
};
int main()
{
    A aa;
    aa.Print();
}