#ifndef NEURONS_FILTERPROVIDER_H
#define NEURONS_FILTERPROVIDER_H


#include <utility>
#include <cmath>
#include <Eigen/Dense>
#include "AngleTranslationUtils.h"

using Eigen::MatrixXd;

class FilterProvider {
public:
    explicit FilterProvider(int filterSize) : filterSize(filterSize) {
        assert(filterSize % 2 == 1);
    }

    virtual double filterAt(int x, int y) = 0;

    MatrixXd generateFilter(int rotationAngleDegrees) {
        MatrixXd result(filterSize, filterSize);

        int halfFilterSize = filterSize / 2;
        int xCenter = halfFilterSize;
        int yCenter = halfFilterSize;

        for (int x = -halfFilterSize; x <= halfFilterSize; ++x) {
            for (int y = -halfFilterSize; y <= halfFilterSize; ++y) {
                auto[xRotated, yRotated] = rotate(x, y, rotationAngleDegrees);
                result(xCenter + x, yCenter + y) = filterAt(xRotated, yRotated);
            }
        }

        return result;
    };

    static std::pair<int, int> rotate(int x, int y, int rotationAngleDegrees) {
        double rotationAngleRadians = degreeToRadian(rotationAngleDegrees);
        int xRotated = (int) (x * cos(rotationAngleRadians) - y * sin(rotationAngleRadians));
        int yRotated = (int) (x * sin(rotationAngleRadians) + y * cos(rotationAngleRadians));
        return {xRotated, yRotated};
    };

private:
    int filterSize;
};

class GaborFilterProvider : public FilterProvider {
public:
    GaborFilterProvider(int filterSize, double lambdaParameter) :
            FilterProvider(filterSize),
            lambdaParameter(lambdaParameter) {};

    double filterAt(int x, int y) override {
        return exp(-((x * x + GAMMA * GAMMA * y * y) / (2 * SIGMA * SIGMA))) *
               cos(2 * M_PI * (x / lambdaParameter) + PHI);
    }

private:
    constexpr static const double GAMMA = .1;
    constexpr static const double SIGMA = 1.2;
    constexpr static const double PHI = .0;
    double lambdaParameter;
};

class MexicanHatFilterProvider : public FilterProvider {
public:
    MexicanHatFilterProvider(int filterSize, double lambdaParameter) :
            FilterProvider(filterSize),
            lambdaParameter(lambdaParameter) {};

    double filterAt(int x, int y) override {
        auto tmp = (x * x + GAMMA * GAMMA * y * y) / (2 * SIGMA * SIGMA);
        return exp(-tmp) * (1 - tmp);
    }

private:
    constexpr static const double GAMMA = .01;
    constexpr static const double SIGMA = .71;
    double lambdaParameter;
};


#endif //NEURONS_FILTERPROVIDER_H
