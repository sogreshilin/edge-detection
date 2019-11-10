#ifndef NEURONS_DRAWINGUTILS_H
#define NEURONS_DRAWINGUTILS_H

#include <utility>
#include <vector>

int signum(int x) {
    if (x == 0) {
        return 0;
    }

    if (x > 0) {
        return 1;
    }

    return -1;
}

std::vector <std::pair<int, int>> line(int x0, int y0, int x1, int y1) {
    std::vector <std::pair<int, int>> line;
    if (x1 - x0 < 0) {
        int tmp;
        tmp = x0;
        x0 = x1;
        x1 = tmp;
        tmp = y0;
        y0 = y1;
        y1 = tmp;
    }

    int deltaX = x1 - x0;
    int deltaY = y1 - y0;

    int directionX = signum(deltaX);
    int directionY = signum(deltaY);

    deltaX = abs(deltaX);
    deltaY = abs(deltaY);

    bool isAcuteAngle = deltaX > deltaY;

    int dx = isAcuteAngle ? directionX : 0;
    int dy = isAcuteAngle ? 0 : directionY;
    int deltaError = isAcuteAngle ? deltaY : deltaX;
    int length = isAcuteAngle ? deltaX : deltaY;

    int x = x0;
    int y = y0;
    int error = 0;

    for (int i = 0; i <= length; ++i) {
        line.emplace_back(x, y);
        error += deltaError;
        if (2 * error >= length) {
            error -= length;
            y += directionY;
            x += directionX;
        } else {
            x += dx;
            y += dy;
        }
    }

    return line;
}


#endif //NEURONS_DRAWINGUTILS_H
