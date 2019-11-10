#include <iostream>
#include "FilterProvider.h"
#include "Network.h"

int main() {
    Network network = Network(
            13,
            10,
            new GaborFilterProvider(7, 3)
    );
    return 0;
}