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

    std::pair<std::vector<MatrixXd>, std::vector<MatrixXd>> prepareData(
            int imageSize,
            const std::vector<std::pair<int, int>> &degreePairs
    ) {
        auto xs = std::vector<MatrixXd>();
        auto ys = std::vector<MatrixXd>();
        ImageGenerator imageGenerator = ImageGenerator(imageSize, backgroundColor, lineColor);
        auto degreePairsSize = degreePairs.size();

        int i = 0;
        for (const auto&[alpha, beta] : degreePairs) {
            xs.emplace_back(imageGenerator.generateImage(alpha, beta));
            MatrixXd y = MatrixXd::Zero(degreePairsSize, 1);
            y(i, 0) = 1.;
            ys.emplace_back(y);
            i++;
        }

        return std::make_pair(xs, ys);
    }

private:
    double backgroundColor;
    double lineColor;
};


#endif //NEURONS_DATAPROVIDER_H
