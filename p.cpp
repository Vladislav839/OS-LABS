#include <iostream>
#include <cmath>

int main()
{
    int value;
    while (std::cin >> value)
    {
        std::cout << std::pow(value, 3) << '\n';
    }
    std::cout << '\0';

    return 0;
}