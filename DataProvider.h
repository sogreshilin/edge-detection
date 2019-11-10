#ifndef NEURONS_DATAPROVIDER_H
#define NEURONS_DATAPROVIDER_H

#include <utility>
#include <vector>
#include <numeric>
#include <Eigen/Dense>
#include "ImageGenerator.h"

using Eigen::MatrixXd;

class DataProvider {
public:
    DataProvider() :
            backgroundColor(0.),
            lineColor(1.) {};

    DataProvider(double backgroundColor, double lineColor) :
            backgroundColor(backgroundColor),
            lineColor(lineColor) {}

    std::vector<std::pair<MatrixXd, MatrixXd>> prepareData(int imageSize, int degreeDelta) {
        auto result = std::vector<std::pair<MatrixXd, MatrixXd>>();

        ImageGenerator imageGenerator = ImageGenerator(imageSize, backgroundColor, lineColor);
        auto degreePairs = std::vector<std::pair<int, int>>();
        for (int alpha = 0; alpha < 360; alpha += degreeDelta) {
            for (int beta = alpha + degreeDelta; beta < 360; beta += degreeDelta) {
                degreePairs.emplace_back(alpha, beta);
            }
        }

        auto degreePairsSize = degreePairs.size();
        int i = 0;
        for (const auto& [alpha, beta] : degreePairs) {
            auto x = imageGenerator.generateImage(alpha, beta);
            MatrixXd y = MatrixXd::Zero(degreePairsSize, 1);
            y(i, 0) = 1.;
            result.emplace_back(x, y);
        }

        return result;
    }

private:
    double backgroundColor;
    double lineColor;
};


#endif //NEURONS_DATAPROVIDER_H
