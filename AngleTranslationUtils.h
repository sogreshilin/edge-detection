#ifndef NEURONS_ANGLETRANSLATIONUTILS_H
#define NEURONS_ANGLETRANSLATIONUTILS_H

#include <cmath>

static double degreeToRadian(double degree) {
    return M_PI * degree / 180;
}

static double radianToDegree(double radian) {
    return 180 * radian / M_PI;
}

#endif //NEURONS_ANGLETRANSLATIONUTILS_H
