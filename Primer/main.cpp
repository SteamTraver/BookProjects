#include "common.h"

int main()
{
    std::cout << "How many lines do you wanna input?\n";
    int n = 0;
    std::cin >> n;

    if (n <= 0) {
        std::cout << "Line num is invalid, exit.\n";
        return 0;
    }


    std::cout << "You wanna input> " << n << " lines, so process." << std::endl;

    std::allocator<std::string> alloc;
    std::string* data = alloc.allocate(n);

    for (int i = 0; i < n; i++) {
        std::cout << "";
    }
}