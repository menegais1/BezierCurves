
#include "Bounds.h"
#include "../Vectors/Float3.h"
#include <cmath>

Bounds::Bounds(Float3 minBound, Float3 maxBound) : minBound(minBound), maxBound(maxBound) {
    extents = maxBound - minBound;
    center = minBound + extents * 0.5;
    corners[0] = minBound;
    corners[1] = {minBound.x + extents.x, minBound.y, 0};
    corners[2] = maxBound;
    corners[3] = {minBound.x, maxBound.y, 0};
}

Bounds::Bounds() {
}

void Bounds::translate(Float3 translationAmount) {
    for (int i = 0; i < 4; i++) {
        corners[i] = corners[i] + translationAmount;
    }

    minBound = corners[0];
    maxBound = corners[2];
    extents = maxBound - minBound;
    center = minBound + extents * 0.5;
}

void Bounds::rotate(float angle, Float3 center) {
    translate(-center);

    for (int i = 0; i < 4; i++) {
        float x = corners[i].x * cos(angle) - corners[i].y * sin(angle);
        float y = corners[i].x * sin(angle) + corners[i].y * cos(angle);
        corners[i] = {x, y, corners[i].z};
    }

    translate(center);

    minBound = corners[0];
    maxBound = corners[2];
    extents = maxBound - minBound;
    this->center = minBound + extents * 0.5;
}

void Bounds::rescale(Float3 scale, Float3 center) {
    translate(-center);
    for (int i = 0; i < 4; i++) {
        float x = corners[i].x * scale.x;
        float y = corners[i].y * scale.y;
        corners[i].x = x;
        corners[i].y = y;
    }
    translate(center);

    minBound = corners[0];
    maxBound = corners[2];
    extents = maxBound - minBound;
    this->center = minBound + extents * 0.5;
}
