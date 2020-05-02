//
// Created by menegais1 on 02/05/2020.
//

#include <iostream>
#include "ControlPoint.h"
#include "../Canvas/gl_canvas2d.h"

void ControlPoint::mouse(int button, int state, int wheel, int direction, int x, int y) {
    curMousePosition = Float3(x, y, 0);
    if (leftMouseDown(button, state)) {
        if (pointIntersectsObject(Float3(x, y, 0))) {
            dragging = true;
        }
    }

    if (leftMouseUp(button, state)) {
        dragging = false;
    }

    if (dragging) {
        Float3 delta = curMousePosition - lastMousePosition;
        translate(curMousePosition - lastMousePosition);
    }
    lastMousePosition = curMousePosition;
}

void ControlPoint::render() {
    circleFill(vertex.x, vertex.y, radius, 20);
}

bool ControlPoint::pointIntersectsObject(Float3 point) {
    return isPointInsideCircle(point, vertex, radius);
}

void ControlPoint::translate(Float3 translationAmount) {
    vertex = vertex + translationAmount;
}

ControlPoint::ControlPoint(const Float3 &vertex, float radius) : vertex(vertex), radius(radius) {
    curMousePosition = Float3(0, 0, 0);
    lastMousePosition = Float3(0, 0, 0);
}
