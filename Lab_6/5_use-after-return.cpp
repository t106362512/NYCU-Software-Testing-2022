#include <iostream>
using namespace std;
int *ptr;
void gg()
{
    int lc = 5;
    ptr = &lc;
}
int main()
{
    gg();
    cout << *ptr << endl;
    return 0;
}