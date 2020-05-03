#include "ColorPicker.h"
#include "ColorSlider.h"
#include "ColorPickerPanel.h"
#include "../Base/CanvasObject.h"
#include "../Panel/Panel.h"
#include "../Vectors/Float3.h"
#include "../Canvas/gl_canvas2d.h"
#include <vector>
#include <functional>

void ColorPickerPanel::mouse(int button, int state, int wheel, int direction, int x, int y) {
    bool pointInside = isMouseInsideObject();
    currentMousePosition = {x, y};
    if (leftMouseDown(button, state) && pointInside) {
        mouseDragging = true;
    } else if (leftMouseUp(button, state)) {
        mouseDragging = false;
    }

    if (mouseDragging) {
        translate(Float3(currentMousePosition - lastMousePosition, 0));
    }
    lastMousePosition = currentMousePosition;
}

void ColorPickerPanel::render() {
    color(backgroundColor.x, backgroundColor.y, backgroundColor.z);
    rectFill(position.x, position.y, position.x + scale.x, position.y + scale.y);

    color(colorPreview.x, colorPreview.y, colorPreview.z);
    rectFill(position.x + scale.x / 4, position.y + 10, (position.x + scale.x / 4) + 30, position.y + 40);
}

ColorPickerPanel::ColorPickerPanel(Float3 position, Float3 scale, Float3 backgroundColor) : Panel(position, scale,
                                                                                                  backgroundColor) {
    colorPicker = new ColorPicker(position + Float3(5, scale.y / 4, 0), scale.x * 2 / 3, scale.y * 2 / 3);
    colorPicker->setZIndex(getZIndex() + 10);
    colorSlider = new ColorSlider(colorPicker->position + Float3(colorPicker->width + 20, 0.0, 0.0), 10,
                                  colorPicker->height);
    colorSlider->setZIndex(getZIndex() + 10);
    colorPicker->addOnValueChangedListener([this](Float3 currentSample) {
        this->colorSlider->setColors({0, 0, 0}, currentSample);
    });
    colorSlider->addOnValueChangedListener([this](Float3 currentSample) {
        this->colorPreview = currentSample;
        notifyOnValueChangedListeners();
    });

    closeButton = new Button(position + scale - Float3(scale.x / 3, scale.y - 10, 0),
                             Float3(scale.x / 4, scale.y / 6, 0), Float3(0, 0, 0), "Close", Float3(1, 1, 1));
    closeButton->addListener([this] {
        this->setActive(false);
        this->notifyOnActivateListeners();
    });
    mouseDragging = false;
    colorPicker->notifyOnValueChangedListeners();
    colorSlider->notifyOnValueChangedListeners();
    children.push_back(colorSlider);
    children.push_back(closeButton);
    children.push_back(colorPicker);
}

void ColorPickerPanel::addOnValueChangedListener(std::function<void(Float3 color)> listener) {
    this->onValueChangedListeners.push_back(listener);
}

void ColorPickerPanel::notifyOnValueChangedListeners() {
    for (int i = 0; i < onValueChangedListeners.size(); i++) {
        this->onValueChangedListeners[i](colorPreview);
    }
}

bool ColorPickerPanel::pointIntersectsObject(Float3 point) {
    return isPointInsideBounds({point.x, point.y}, {position.x, position.y}, {scale.x, scale.y});
}
