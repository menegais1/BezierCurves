
#include <vector>
#include <iostream>
#include <cmath>
#include <sstream>
#include "../Vectors/Float2.h"
#include "../Vectors/Float4.h"
#include "../Canvas/gl_canvas2d.h"
#include "BezierCurve.h"
#include <functional>
#include "../Bounds/Bounds.h"
#include "../Fatorial.h"


BezierCurve::BezierCurve() {
    drawZIndex = false;
    drawBounds = false;
    isSelected = false;
}

BezierCurve::BezierCurve(Float3 backgroundColor, Float3 lineColor, Float4 highlightColor,
                         std::vector<ControlPoint *> controlPoints) {
    this->lineColor = lineColor;
    this->highlightColor = highlightColor;
    this->_controlPoints = controlPoints;
    calculateConvexHull();
    isSelected = false;
    drawBounds = false;
    drawZIndex = false;
    computeCentroid();
    int n = controlPoints.size() - 1;
    for (int i = 0; i <= n; ++i) {
        blendingFunctions.push_back(calculateBlendingFunction(n, i));
    }
}

void BezierCurve::render() {
    if (_controlPoints.size() < 0)
        return;

    color(lineColor.x, lineColor.y, lineColor.z, 1);

    for (int i = 0; i < _controlPoints.size() - 1; ++i) {
        ControlPoint *p0 = _controlPoints[i];
        ControlPoint *p1 = _controlPoints[i + 1];

        line(p0->vertex.x, p0->vertex.y, p1->vertex.x, p1->vertex.y);
    }

    for (float t = 0; t <= 1; t += 0.01) {
        Float3 p;
        for (int i = 0; i < _controlPoints.size(); ++i) {
            p.x += _controlPoints[i]->vertex.x * blendingFunctions[i](t);
            p.y += _controlPoints[i]->vertex.y * blendingFunctions[i](t);
        }
        point(p.x, p.y);
    }


    color(1, 1, 1);

    if (drawZIndex) {
        std::stringstream stream;
        stream << getZIndex();
        text(centroid.x, centroid.y, stream.str().c_str());
    }

    if (drawBounds) {
        calculateConvexHull();
        for (int i = 0; i < convexHull.hull.size() - 1; ++i) {
            Float3 p0 = convexHull.hull[i];
            Float3 p1 = convexHull.hull[i + 1];
            line(p0.x, p0.y, p1.x, p1.y);
        }
    }

    if (isSelected) {
        color(highlightColor.x, highlightColor.y, highlightColor.z, highlightColor.w);
    }
}

//Function for calculating the polygon centroid, as it's center of mass
//Taken from https://bell0bytes.eu/centroid-convex/
void BezierCurve::computeCentroid() {
    float centroidX = 0, centroidY = 0;
    float det = 0, tempDet = 0;
    int j = 0;
    int nVertices = _controlPoints.size();

    for (unsigned int i = 0; i < nVertices; i++) {
        // closed polygon
        if (i + 1 == nVertices)
            j = 0;
        else
            j = i + 1;

        // compute the determinant
        tempDet = _controlPoints[i]->vertex.x * _controlPoints[j]->vertex.y -
                  _controlPoints[j]->vertex.x * _controlPoints[i]->vertex.y;
        det += tempDet;

        centroidX += (_controlPoints[i]->vertex.x + _controlPoints[j]->vertex.x) * tempDet;
        centroidY += (_controlPoints[i]->vertex.y + _controlPoints[j]->vertex.y) * tempDet;
    }

    // divide by the total mass of the polygon
    centroidX /= 3 * det;
    centroidY /= 3 * det;

    centroid = {centroidX, centroidY, 0};
}

std::function<float(float)> BezierCurve::calculateBlendingFunction(int n, int i) {
    float binomial = Fatorial::fatorial(n) / (Fatorial::fatorial(i) * Fatorial::fatorial(n - i));
    return [binomial, n, i](float t) -> float {
        return binomial * std::pow(t, i) * std::pow(1 - t, n - i);
    };
}

Float3 BezierCurve::getCenter() {
    computeCentroid();
    return centroid;
}

void BezierCurve::translate(Float3 translationAmount) {
    for (int i = 0; i < _controlPoints.size(); i++) {
        _controlPoints[i]->vertex.x += translationAmount.x;
        _controlPoints[i]->vertex.y += translationAmount.y;
        _controlPoints[i]->vertex.z += translationAmount.z;
    }
    computeCentroid();
}

void BezierCurve::rotate(float angle, Float3 center) {
    translate({-center.x, -center.y, 0});
    for (int i = 0; i < _controlPoints.size(); i++) {
        float x = _controlPoints[i]->vertex.x * cos(angle) - _controlPoints[i]->vertex.y * sin(angle);
        float y = _controlPoints[i]->vertex.x * sin(angle) + _controlPoints[i]->vertex.y * cos(angle);
        _controlPoints[i]->vertex.x = x;
        _controlPoints[i]->vertex.y = y;
    }
    translate({center.x, center.y, 0});
    computeCentroid();
}

void BezierCurve::rescale(Float3 scale, Float3 center) {
    translate({-center.x, -center.y, 0});
    for (int i = 0; i < _controlPoints.size(); i++) {
        float x = _controlPoints[i]->vertex.x * scale.x;
        float y = _controlPoints[i]->vertex.y * scale.y;
        _controlPoints[i]->vertex.x = x;
        _controlPoints[i]->vertex.y = y;
    }
    translate({center.x, center.y, 0});
    computeCentroid();
}

bool BezierCurve::pointIntersectsObject(Float3 point) {
    return isPointInsidePolygon({point.x, point.y}, getPoints(), _controlPoints.size() - 1);
}

std::vector<Float3> BezierCurve::getPoints() {
    std::vector<Float3> points;
    for (int i = 0; i < _controlPoints.size(); ++i) {
        points.push_back(_controlPoints[i]->vertex);
    }
    return points;
}

void BezierCurve::calculateConvexHull() {
    convexHull.calculateConvexHull(getPoints());
}