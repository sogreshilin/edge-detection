cmake_minimum_required(VERSION 3.15)
project(neurons)

set(CMAKE_CXX_STANDARD 17)

find_package(Eigen3 3.3 REQUIRED NO_MODULE)

add_executable(
        neurons
        AngleTranslationUtils.h
        CoordinateCalculator.h
        DrawingUtils.h
        DataProvider.h
        FilterProvider.h
        ImageGenerator.h
        Network.h
        main.cpp
)

target_link_libraries(neurons Eigen3::Eigen)
