#include <iostream>
#include "FilterProvider.h"
#include "DataProvider.h"

int main() {
    auto dataProvider = DataProvider();
    auto data = dataProvider.prepareData(13, 10);
//    ImageGenerator imageGenerator = ImageGenerator(13, 1., lineColor);
    std::cout << data[0].first << std::endl;
    std::cout << data[0].second << std:: endl;
    return 0;
}