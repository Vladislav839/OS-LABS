#include <iostream>
#include <sstream>

int main()
{
    int value;
    while (std::cin >> value)
    {
        std::cout << value * 7 << '\n';
    }
    std::cout << '\0';

    return 0;
}