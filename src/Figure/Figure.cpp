
#include <vector>
#include <iostream>
#include <cmath>
#include <sstream>
#include "../Vectors/Float2.h"
#include "../Vectors/Float4.h"
#include "../Canvas/gl_canvas2d.h"
#include "Figure.h"
#include "../Bounds/Bounds.h"

void Figure::render() {
    if (vertices.size() < 0)
        return;

    color(lineColor.x, lineColor.y, lineColor.z, lineAlpha);
    polygon(vertices.data(), vertices.size());

    color(backgroundColor.x, backgroundColor.y, backgroundColor.z, backgroundAlpha);

    polygonFill(vertices.data(), vertices.size());

    color(1, 1, 1);

    if (drawZIndex) {
        std::stringstream stream;
        stream << getZIndex();
        text(centroid.x, centroid.y, stream.str().c_str());
    }

    if (drawBounds) {
        line(bounds.corners[0].x, bounds.corners[0].y, bounds.corners[1].x, bounds.corners[1].y);
        line(bounds.corners[1].x, bounds.corners[1].y, bounds.corners[2].x, bounds.corners[2].y);
        line(bounds.corners[2].x, bounds.corners[2].y, bounds.corners[3].x, bounds.corners[3].y);
        line(bounds.corners[3].x, bounds.corners[3].y, bounds.corners[0].x, bounds.corners[0].y);
    }

    if (isSelected) {
        color(highlightColor.x, highlightColor.y, highlightColor.z, highlightColor.w);
        polygonFill(vertices.data(), vertices.size());
    }
}

//Function for calculating the polygon centroid, as it's center of mass
//Taken from https://bell0bytes.eu/centroid-convex/
void Figure::computeCentroid() {
    float centroidX = 0, centroidY = 0;
    float det = 0, tempDet = 0;
    int j = 0;
    int nVertices = vertices.size();

    for (unsigned int i = 0; i < nVertices; i++) {
        // closed polygon
        if (i + 1 == nVertices)
            j = 0;
        else
            j = i + 1;

        // compute the determinant
        tempDet = vertices[i].x * vertices[j].y - vertices[j].x * vertices[i].y;
        det += tempDet;

        centroidX += (vertices[i].x + vertices[j].x) * tempDet;
        centroidY += (vertices[i].y + vertices[j].y) * tempDet;
    }

    // divide by the total mass of the polygon
    centroidX /= 3 * det;
    centroidY /= 3 * det;

    centroid = {centroidX, centroidY, 0};
}

Float3 Figure::getCenter() {
    return bounds.center;
}

void Figure::translate(Float3 translationAmount) {
    for (int i = 0; i < vertices.size(); i++) {
        vertices[i].x += translationAmount.x;
        vertices[i].y += translationAmount.y;
        vertices[i].z += translationAmount.z;
    }
    bounds.translate(translationAmount);
    computeCentroid();
}

void Figure::rotate(float angle, Float3 center) {
    translate({-center.x, -center.y, 0});
    for (int i = 0; i < vertices.size(); i++) {
        float x = vertices[i].x * cos(angle) - vertices[i].y * sin(angle);
        float y = vertices[i].x * sin(angle) + vertices[i].y * cos(angle);
        vertices[i].x = x;
        vertices[i].y = y;
    }
    translate({center.x, center.y, 0});
    bounds.rotate(angle, center);
    computeCentroid();
}

void Figure::rescale(Float3 scale, Float3 center) {
    translate({-center.x, -center.y, 0});
    for (int i = 0; i < vertices.size(); i++) {
        float x = vertices[i].x * scale.x;
        float y = vertices[i].y * scale.y;
        vertices[i].x = x;
        vertices[i].y = y;
    }
    translate({center.x, center.y, 0});
    bounds.rescale(scale, center);
    computeCentroid();
}

void Figure::initializeBounds() {
    Float3 minBound;
    Float3 maxBound;
    float minX = vertices[0].x, minY = vertices[0].y, maxX = vertices[0].x, maxY = vertices[0].y;
    for (int i = 0; i < vertices.size(); i++) {
        if (vertices[i].x < minX)
            minX = vertices[i].x;
        if (vertices[i].x > maxX)
            maxX = vertices[i].x;
        if (vertices[i].y < minY)
            minY = vertices[i].y;
        if (vertices[i].y > maxY)
            maxY = vertices[i].y;
    }
    minBound = {minX, minY, 0};
    maxBound = {maxX, maxY, 0};
    bounds = {minBound, maxBound};
}

Figure::Figure(Float3 backgroundColor, Float3 lineColor, Float4 highlightColor, std::vector<Float3> vertices) {
    this->backgroundColor = backgroundColor;
    this->lineColor = lineColor;
    this->highlightColor = highlightColor;
    this->vertices = vertices;
    initializeBounds();
    isSelected = false;
    drawBounds = false;
    drawZIndex = false;
    backgroundAlpha = lineAlpha = 1.0;
    computeCentroid();
}

bool Figure::pointIntersectsObject(Float3 point) {
    return isPointInsidePolygon({point.x, point.y}, vertices, vertices.size() - 1);
}

Figure::Figure() {
    drawZIndex = false;
    drawBounds = false;
    isSelected = false;
}
