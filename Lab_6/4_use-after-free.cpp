#include <iostream>
using namespace std;
int main()
{
    int *array = new int[5];
    array[4] = 10;
    delete[] array;
    cout << array[4] << endl;
    return 0;
}