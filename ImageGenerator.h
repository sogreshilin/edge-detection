#ifndef NEURONS_IMAGEGENERATOR_H
#define NEURONS_IMAGEGENERATOR_H


#include <vector>
#include "CoordinateCalculator.h"
#include "DrawingUtils.h"

class ImageGenerator {

public:
    explicit ImageGenerator(int imageSize) :
            imageSize(imageSize),
            backgroundColor(0.),
            lineColor(1.) {
        assert(imageSize % 4 == 1);
    };

    ImageGenerator(int imageSize, double backgroundColor, double lineColor) :
            imageSize(imageSize),
            backgroundColor(backgroundColor),
            lineColor(lineColor) {};

    MatrixXd generateImage(int alphaAngleDegrees, int betaAngleDegrees) {
        MatrixXd image = MatrixXd::Constant(imageSize, imageSize, backgroundColor);
        auto x0 = imageSize / 2;
        auto y0 = imageSize / 2;
        auto[x1, y1] = coordinateCalculator.computeRayEndCoordinates(alphaAngleDegrees);
        auto[x2, y2] = coordinateCalculator.computeRayEndCoordinates(betaAngleDegrees);
        auto line1 = line(x0, y0, x1, y1);
        for (const auto &coordinate: line1) {
            image(coordinate.first, coordinate.second) = lineColor;
        }
        auto line2 = line(x0, y0, x2, y2);
        for (const auto &coordinate: line2) {
            image(coordinate.first, coordinate.second) = lineColor;
        }
        return image;
    }

private:
    int imageSize;
    double backgroundColor;
    double lineColor;
    CoordinateCalculator coordinateCalculator = CoordinateCalculator(imageSize);
};


#endif //NEURONS_IMAGEGENERATOR_H
