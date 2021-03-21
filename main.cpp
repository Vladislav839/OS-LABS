#include <iostream>

#include "VectorLib.h"

int main()
{
    Vector vector = Vector(Number(10), Number(15));
    std::cout << "First vector coordinates:\n";
    std::cout << "x: " << vector.getX().getValue() << '\n';
    std::cout << "y: " << vector.getY().getValue() << '\n';

    std::cout << "First vector polar coordinates" << '\n';
    std::cout << "R: " << vector.get_Polar_R().getValue() << '\n';
    std::cout << "PHI: " << vector.get_Polar_Phi().getValue() << '\n';

    Vector vector2 = Vector(CreateNumber(15), CreateNumber(20));
    std::cout << "Second vector coordinates:\n";
    std::cout << "x: " << vector2.getX().getValue() << '\n';
    std::cout << "y: " << vector2.getY().getValue() << '\n';

    Vector s = vector.add(vector2);
    std::cout << "Sum of vectors" << '\n';
    std::cout << "X: " << s.getX().getValue() << '\n';
    std::cout << "Y:" << s.getY().getValue() << '\n';
    return 0;
}