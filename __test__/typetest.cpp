#include <iostream>
#include "../type_traits.hpp"

int main(void)
{
    std::cout << std::boolalpha;
    std::cout << ft::is_integral<const volatile short>::value << std::endl;
}