#include <iostream>
#include "FilterProvider.h"
#include "ImageGenerator.h"

int main() {
    auto imageGenerator = ImageGenerator(13);
    std::cout << imageGenerator.generateImage(115, 0) << std:: endl;
    return 0;
}