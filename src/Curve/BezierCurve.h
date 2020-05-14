//BezierCurve class, store curve information and offer operations to other classes to
//manipulate them

#ifndef BezierCurve_H
#define BezierCurve_H

#include <vector>
#include "../ControlPoint/ControlPoint.h"
#include "../ConvexHull/ConvexHull.h"
#include <functional>
#include "../BlendingFunctionsGraph/BlendingFunctionsGraph.h"
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
    BlendingFunctionsGraph* blendingFunctionsGraph;
    std::vector<ControlPoint *> _controlPoints;
    std::vector<std::function<float(float)>> blendingFunctions;
    std::vector<Float3> animationColors;

    bool isSelected;
    bool drawZIndex;
    bool drawConvexHull;
    bool drawControlGraph;
    bool drawAnimation;
    bool drawControlPoints;
    Float4 highlightColor;

    void render() override;

    BezierCurve(Float3 lineColor, Float4 highlightColor,
                std::vector<ControlPoint *> controlPoints);

    ~BezierCurve();

    Float3 getCenter();

    void curveAnimation(std::vector<Float3> points, float t, int i);

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
