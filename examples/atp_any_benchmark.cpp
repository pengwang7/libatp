#include "app/atp_any.hpp"

#include <iostream>

int main() {
    atp::any a = 1;
    int b = atp::any_cast<int>(a);
    std::cout << "b: " << b << std::endl;

    return 0;
}
