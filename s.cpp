#include <iostream>

int main()
{
    int sum = 0;
    int value;
    while (std::cin >> value)
    {
        sum += value;
    }

    std::cout << "Sum = " << sum << '\n';

    return 0;
}