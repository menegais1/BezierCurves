//BezierCurve class, store curve information and offer operations to other classes to
//manipulate them

#ifndef curve_H
#define curve_H

#include <vector>
#include <ControlPoint/ControlPoint.h>
#include <ConvexHull/ConvexHull.h>
#include <functional>
#include "../Base/CanvasObject.h"
#include "../Base/ITransformable.h"
#include "../Vectors/Float2.h"
#include "../Vectors/Float4.h"
#include "../Bounds/Bounds.h"

enum curveRender {
    FillOnly = 0,
    LineOnly = 1,
    FillLine = 2
};

class BezierCurve : public CanvasObject {
public:
    BezierCurve();

    Float3 lineColor;
    ConvexHull convexHull;
    Float3 centroid;
    std::vector<ControlPoint *> _controlPoints;
    std::vector<std::function<float(float)>> blendingFunctions;

    bool isSelected;
    bool drawZIndex;
    bool drawBounds;
    Float4 highlightColor;

    void render() override;

    BezierCurve(Float3 backgroundColor, Float3 lineColor, Float4 highlightColor,
                std::vector<ControlPoint *> controlPoints);

    ~BezierCurve() = default;

    Float3 getCenter();

    void translate(Float3 translationAmount) override;

    void rotate(float angle, Float3 center) override;

    void rescale(Float3 scale, Float3 center) override;

    bool pointIntersectsObject(Float3 point) override;

    void computeCentroid();

    void calculateConvexHull();

    std::vector<Float3> getPoints();

    std::function<float(float)> calculateBlendingFunction(int n, int i);
};

#endif
