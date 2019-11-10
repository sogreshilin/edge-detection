#ifndef NEURONS_COORDINATECALCULATOR_H
#define NEURONS_COORDINATECALCULATOR_H

#include <utility>
#include <cmath>
#include "AngleTranslationUtils.h"

class CoordinateCalculator {
public:
    explicit CoordinateCalculator(int imageSize) : imageSize(imageSize) {};

    std::pair<int, int> computeRayEndCoordinates(int angleDegree) {
        auto a = floor(imageSize / 2.);
        auto normalizedAngleDegree = angleDegree % 360;

        if (0 <= normalizedAngleDegree && normalizedAngleDegree < 45) {
            return std::make_pair(a - a * tan(degreeToRadian(normalizedAngleDegree)), 0);
        }

        if (45 <= normalizedAngleDegree && normalizedAngleDegree < 90) {
            return std::make_pair(0, a - a * tan(degreeToRadian(90 - normalizedAngleDegree)));
        }

        if (90 <= normalizedAngleDegree && normalizedAngleDegree < 135) {
            return std::make_pair(0, a + a * tan(degreeToRadian(normalizedAngleDegree - 90)));
        }

        if (135 <= normalizedAngleDegree && normalizedAngleDegree < 190) {
            return std::make_pair(a - a * tan(degreeToRadian(180 - normalizedAngleDegree)), imageSize - 1);
        }

        if (180 <= normalizedAngleDegree && normalizedAngleDegree < 225) {
            return std::make_pair(a + a * tan(degreeToRadian(normalizedAngleDegree - 180)), imageSize - 1);
        }

        if (225 <= normalizedAngleDegree && normalizedAngleDegree < 270) {
            return std::make_pair(imageSize - 1, a + a * tan(degreeToRadian(270 - normalizedAngleDegree)));
        }

        if (270 <= normalizedAngleDegree && normalizedAngleDegree < 315) {
            return std::make_pair(imageSize - 1, a - a * tan(degreeToRadian(normalizedAngleDegree - 270)));
        }

        return std::make_pair(a + a * tan(degreeToRadian(360 - normalizedAngleDegree)), 0);
    }

private:
    int imageSize;
};

#endif //NEURONS_COORDINATECALCULATOR_H
