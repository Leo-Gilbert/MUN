#include <iostream>

/*
An example of this function's output is listed below. As can be seen, the 
variables allocated in the main function have higher memory addresses than those allocated in func,
thus the stack grows downwards.
&var1 = 0x16b4eafd8
&var2 = 0x16b4eafd4
&var3 = 0x16b4eafd0
&x = 0x16b4eaf90
&y = 0x16b4eaf8c
&z = 0x16b4eaf88 
*/

void func(int a, int b, int c)
{
    int x, y, z;
    std::cout << "&x = " << &x << std::endl;
    std::cout << "&y = " << &y << std::endl;
    std::cout << "&z = " << &z << std::endl;
}

int main()
{
    int var1, var2, var3;
    std::cout << "&var1 = " << &var1 << std::endl;
    std::cout << "&var2 = " << &var2 << std::endl;
    std::cout << "&var3 = " << &var3 << std::endl;

    func(1, 2, 3);

    return 0;
}