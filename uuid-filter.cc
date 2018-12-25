#include <cstdlib>
#include <iostream>

#include "uuidpp.h"

int main(int argc, char *argv[]) {
    uuid u;
    while (std::cin >> u)
        if (u)
            std::cout << u << std::endl;

    return EXIT_SUCCESS;
};

//
