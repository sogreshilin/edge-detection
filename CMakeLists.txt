cmake_minimum_required(VERSION 3.15)
project(neurons)

set(CMAKE_CXX_STANDARD 17)

find_package(Eigen3 3.3 REQUIRED NO_MODULE)

add_executable(neurons main.cpp FilterProvider.h ImageGenerator.h CoordinateCalculator.h AngleTranslationUtils.h DrawingUtils.h)
target_link_libraries(neurons Eigen3::Eigen)
