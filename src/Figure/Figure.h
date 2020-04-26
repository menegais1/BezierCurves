//Figure class, store figure information and offer operations to other classes to
//manipulate them

#ifndef FIGURE_H
#define FIGURE_H

#include <vector>
#include "../Base/CanvasObject.h"
#include "../Base/ITransformable.h"
#include "../Vectors/Float2.h"
#include "../Vectors/Float4.h"
#include "../Bounds/Bounds.h"

enum FigureRender{
    FillOnly = 0 ,
    LineOnly = 1,
    FillLine = 2
};

class Figure : public CanvasObject {
public:
    Figure();

    Float3 backgroundColor;
    Float3 lineColor;
    Bounds bounds;
    Float3 centroid;
    float backgroundAlpha;
    float lineAlpha;
    std::vector<Float3> vertices;

    bool isSelected;
    bool drawZIndex;
    bool drawBounds;
    Float4 highlightColor;

    void render() override;

    Figure(Float3 backgroundColor, Float3 lineColor, Float4 highlightColor, std::vector<Float3> vertices);

    ~Figure() = default;

    Float3 getCenter();

    void translate(Float3 translationAmount) override;

    void rotate(float angle, Float3 center) override;

    void rescale(Float3 scale, Float3 center) override;

    bool pointIntersectsObject(Float3 point) override;

    void computeCentroid();

    void initializeBounds();

};

#endif
