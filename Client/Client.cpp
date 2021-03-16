#include <iostream>
#include <Windows.h>

#include "VectorLib.h"

typedef VectorApi* (__cdecl* Vector_factory)(Number x, Number y);


int main()
{
    HINSTANCE dll_handle = ::LoadLibrary(TEXT("VectorLib.dll"));
    if (!dll_handle) {
        std::cout << "Unable to load DLL!\n";
        return 1;
    }

    Vector_factory factory_func = reinterpret_cast<Vector_factory>(
        ::GetProcAddress(dll_handle, "CreateInstance"));
    if (!factory_func) {
        std::cout << "Unable to load create_klass from DLL!\n";
        ::FreeLibrary(dll_handle);
        return 1;
    }

    auto vector_1 = factory_func(Number(10), Number(15));
    std::cout << "First vector coordinates\n";
    std::cout << "x coordinate: " << vector_1->getX().getValue() << '\n';
    std::cout << "y coordinate: " << vector_1->getY().getValue() << '\n';

    std::cout << "Polar radius\n";
    std::cout << vector_1->get_Polar_R().getValue() << '\n';
    std::cout << "Polar angle\n";
    std::cout << vector_1->get_Polar_Phi().getValue() << "\n\n";

    auto vector_2 = factory_func(Number(5), Number(7));
    void* p = vector_1->add(vector_2);
    auto res = static_cast<VectorApi*>(p);

    std::cout << "Second vector coordinates\n";
    std::cout << "x coordinate: " << vector_2->getX().getValue() << '\n';
    std::cout << "y coordinate: " << vector_2->getY().getValue() << "\n\n";

    std::cout << "Sum of two vectors is a vector with coordinates\n";
    std::cout << "x coordinate: " << res->getX().getValue() << '\n';
    std::cout << "y coordinate: " << res->getY().getValue() << '\n';

    res->destroy();
    vector_1->destroy();
    vector_2->destroy();

    ::FreeLibrary(dll_handle);
}

