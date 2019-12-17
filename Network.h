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

    static MatrixXd activationFunction(MatrixXd matrix) {
        return matrix;
    }

    static MatrixXd costFunctionDerivative(MatrixXd expected, MatrixXd actual) {
        return actual - expected;
    }

    static MatrixXd activationFunctionDerivative(MatrixXd x) {
        return MatrixXd::Constant(x.rows(), x.cols(), 1.);
    }

    void train(int epochCount, double eta) {
        auto mask = createW2();

        for (int currentEpoch = 0; currentEpoch < epochCount; ++currentEpoch) {
            int hitCount = 0;
            MatrixXd deltaW2 = MatrixXd::Zero(w2.rows(), w2.cols());

            for (int i = 0; i < (int) x.size(); ++i) {
                auto xCurrent = x[i];
                auto yCurrent = y[i];
                Eigen::Map<MatrixXd> a0(xCurrent.data(), xCurrent.rows() * xCurrent.cols(), 1);

                auto z1 = w1 * a0;
                auto a1 = activationFunction(z1);
                auto z2 = w2 * a1;
                auto a2 = activationFunction(z2);

                MatrixXd::Index expectedMaxRow, expectedMaxCol;
                yCurrent.maxCoeff(&expectedMaxRow, &expectedMaxCol);
                int expectedMax = expectedMaxRow;

                MatrixXd::Index actualMaxRow, actualMaxCol;
                a2.maxCoeff(&actualMaxRow, &actualMaxCol);
                int actualMax = actualMaxRow;

                if (expectedMax == actualMax) {
                    hitCount++;
                }

                MatrixXd eVector = MatrixXd::Zero(yCurrent.rows(), yCurrent.cols());
                eVector(actualMax) = 1.;
                MatrixXd delta = costFunctionDerivative(yCurrent, eVector).cwiseProduct(activationFunction(z2));
                Eigen::Map<MatrixXd> reshapedDelta(delta.data(), delta.rows() * delta.cols(), 1);
                Eigen::Map<MatrixXd> reshapedA1(a1.data(), 1, a1.rows() * a1.cols());
                deltaW2 += reshapedDelta * reshapedA1;
            }

            w2 -= deltaW2.cwiseProduct(MatrixXd::Constant(deltaW2.rows(), deltaW2.cols(), eta / x.size()));
            w2 = w2.cwiseProduct(mask);
            auto accuracy = (double) hitCount / x.size();
            std::cout << "accuracy: " << accuracy << std::endl;
        }
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
        i++;
    }

    return result;
}

#endif //NEURONS_NETWORK_H
