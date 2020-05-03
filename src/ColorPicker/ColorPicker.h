//Color picker main window, generated with a bilinear interpolation between a set of colors on the bottom
//and white on the top, making a nice gradient square, has a callback for when a color is picked using a slider or click
//system


#ifndef COLORPICKER_H
#define COLORPICKER_H

#include "../Base/CanvasObject.h"
#include "../Vectors/Float3.h"
#include <vector>
#include <functional>

class ColorPicker : public CanvasObject {
public:
    void mouse(int button, int state, int wheel, int direction, int x, int y) override;

    void render() override;

    bool pointIntersectsObject(Float3 point) override;

    ColorPicker(Float3 position, int width, int height);

    Float3 currentSample;

    void addOnValueChangedListener(std::function<void(Float3 color)> listener);

    void notifyOnValueChangedListeners();

    void translate(Float3 translationAmount);

    int width;
    int height;

private:
    bool mouseDragging;
    bool mouseInside;
    Float2 currentMousePosition;
    Float3 *texture;
    std::vector<Float3> topVertices;
    std::vector<Float3> bottomVertices;

    void generateTexture();

    Float3 sampleTexture(Float2 position);

    std::vector<std::function<void(Float3 color)>> onValueChangedListeners;
};

#endif