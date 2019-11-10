#ifndef NEURONS_NETWORK_H
#define NEURONS_NETWORK_H

#include <vector>
#include <utility>
#include <Eigen/Dense>
#include "FilterProvider.h"
#include "DataProvider.h"

using Eigen::MatrixXd;

class Network {
public:
    Network(int imageSize, int deltaDegree, FilterProvider *filterProvider) :
            imageSize(imageSize),
            deltaDegree(deltaDegree),
            filterProvider(filterProvider) {

        for (int alpha = 0; alpha < 360; alpha += deltaDegree) {
            degrees.emplace_back(alpha);
            for (int beta = alpha + deltaDegree; beta < 360; beta += deltaDegree) {
                degreePairs.emplace_back(alpha, beta);
            }
        }

        DataProvider dataProvider = DataProvider();
        auto data = dataProvider.prepareData(imageSize, degreePairs);
        x = data.first;
        y = data.second;
        w1 = createW1();
        w2 = createW2();
    }

    void train(int epochCount, double eta) {
        // todo: implement
    }

private:
    int imageSize;
    int deltaDegree;
    FilterProvider *filterProvider;
    std::vector<int> degrees = std::vector<int>();
    std::vector<std::pair<int, int>> degreePairs = std::vector<std::pair<int, int>>();
    CoordinateCalculator coordinateCalculator = CoordinateCalculator(imageSize);
    std::vector<MatrixXd> x;
    std::vector<MatrixXd> y;
    MatrixXd w1;
    MatrixXd w2;
    MatrixXd createW1();
    MatrixXd createW2();
};

MatrixXd Network::createW1() {
    int height = degrees.size();
    int width = imageSize * imageSize;

    MatrixXd result = MatrixXd::Zero(height, width);

    int i = 0;
    for (int degree: degrees) {
        auto[xRayEnd, yRayEnd] = coordinateCalculator.computeRayEndCoordinates(degree);

        auto filterSize = imageSize / 2 + 1;
        auto xFilterCenter = (filterSize + xRayEnd) / 2;
        auto yFilterCenter = (filterSize + yRayEnd) / 2;

        MatrixXd matrix = MatrixXd::Zero(imageSize, imageSize);
        matrix.block(
                xFilterCenter - filterSize / 2,
                yFilterCenter - filterSize / 2,
                filterSize,
                filterSize
        ) = filterProvider->generateFilter(degree);

        matrix.transposeInPlace();
        Eigen::Map<MatrixXd> resizedMatrix(matrix.data(), 1, matrix.rows() * matrix.cols());
        result.block(i, 0, 1, resizedMatrix.cols()) = resizedMatrix;
        i++;
    }

    return result;
}

MatrixXd Network::createW2() {
    int height = degreePairs.size();
    int width = degrees.size();
    MatrixXd result = MatrixXd::Zero(height, width);

    auto onesCoordinates = std::vector<std::pair<int, int>>();
    for (int i = 0; i < width; ++i) {
        for (int j = i + 1; j < width; ++j) {
            onesCoordinates.emplace_back(i, j);
        }
    }

    int i = 0;
    for (const auto &onesCoordinate : onesCoordinates) {
        auto[x, y] = onesCoordinate;
        result(i, x) = 1.;
        result(i, y) = 1.;
    }

    return result;
}

#endif //NEURONS_NETWORK_H
