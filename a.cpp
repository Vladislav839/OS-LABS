#include <iostream>

int main()
{
    int value;
    while (std::cin >> value)
    {
        std::cout << value + 12 << '\n';
    }
    std::cout << '\0';

    return 0;
}