#include <iostream>
using namespace std;
int main()
{
    int array_a[10], array_b[10];
    for (int i = 1; i <= 10; i++)
        *(&array_a[9] + 10 + i) = 5;
    for (auto n : array_b)
        cout << n << endl;
    *(&array_a[9] + 35) = 10;
    return 0;
}