#include <iostream>
#include "FilterProvider.h"
#include "Network.h"

int main() {
    std::cout << MatrixXd::Constant(3, 2, 2) * (MatrixXd::Constant(2, 2, 3)) << std::endl;

    Network network = Network(
            13,
            10,
            new GaborFilterProvider(7, 3)
    );

    network.train(100, 0.1);
    return 0;
}