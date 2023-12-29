#include <iostream>
#include "package.hpp"

int main() {
    Package p1;
    Package p2;
    Package p3;

    std::cout << p1.get_id() << std::endl;
    std::cout << p2.get_id() << std::endl;
    std::cout << p3.get_id() << std::endl;
    return 0;
}
