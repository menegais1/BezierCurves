
#include "ColorSlider.h"
#include "../Vectors/Float3.h"
#include "../Canvas/gl_canvas2d.h"
#include "../Utilities.h"
#include <iostream>

void ColorSlider::mouse(int button, int state, int wheel, int direction, int x, int y) {
    bool pointInside = isMouseInsideObject();
    if (leftMouseDown(button, state) && pointInside) {
        mouseDragging = true;
    } else if (leftMouseUp(button, state)) {
        mouseDragging = false;
    }

    if (mouseDragging) {
        currentMousePosition = {x, y};
        if (x > position.x + width)
            currentMousePosition.x = position.x + width - 1;
        if (y > position.y + height)
            currentMousePosition.y = position.y + height - 1;
        if (x < position.x)
            currentMousePosition.x = position.x;
        if (y < position.y)
            currentMousePosition.y = position.y;

        Float3 sample = sampleTexture(currentMousePosition);
        if (sample.x != currentSample.x || sample.y != currentSample.y || sample.z != currentSample.z) {
            currentSample = sample;
            notifyOnValueChangedListeners();
        }
    }
}

void ColorSlider::render() {
    color(1, 1, 1);
    rect(position.x - 1, position.y - 1, position.x + width + 1, position.y + height + 1);
    for (float y = 0; y < height; y++) {
        for (float x = 0; x < width; x++) {
            Float3 renderColor = texture[(int) y];
            color(renderColor.x, renderColor.y, renderColor.z);
            point(position.x + x, position.y + y);
        }
    }

    if (currentMousePosition.x != -1) {
        color(0.2, 0.2, 0.2);
        circleFill(position.x + width / 2, currentMousePosition.y, 6, 20);
    }
}

Float3 ColorSlider::sampleTexture(Float2 position) {
    return texture[(int) (position.y - this->position.y)];
}

void ColorSlider::generateTexture() {
    for (float y = 0; y < height; y++) {
        float stepY = y / height;
        Float3 interpColor = lerp(bottomColor, topColor, stepY);
        texture[(int) y] = interpColor;
    }
}

void ColorSlider::setColors(Float3 bottomColor, Float3 topColor) {
    this->bottomColor = bottomColor;
    this->topColor = topColor;
    generateTexture();
    Float3 sample = sampleTexture(currentMousePosition);
    if (sample.x != currentSample.x || sample.y != currentSample.y || sample.z != currentSample.z) {
        currentSample = sample;
        notifyOnValueChangedListeners();
    }
}

void ColorSlider::addOnValueChangedListener(std::function<void(Float3 color)> listener) {
    this->onValueChangedListeners.push_back(listener);
}

void ColorSlider::notifyOnValueChangedListeners() {
    for (int i = 0; i < onValueChangedListeners.size(); i++) {
        this->onValueChangedListeners[i](currentSample);
    }
}

ColorSlider::ColorSlider(Float3 position, int width, int height) {
    this->position = position;
    this->width = width;
    this->height = height;
    this->texture = new Float3[height];
    this->mouseDragging = false;
    this->currentMousePosition = {position.x, position.y + height - 1};
    this->currentSample = sampleTexture(currentMousePosition);
}

bool ColorSlider::pointIntersectsObject(Float3 point) {
    return isPointInsideBounds({point.x, point.y}, {position.x, position.y}, {width, height});
}

void ColorSlider::translate(Float3 translationAmount) {
    CanvasObject::translate(translationAmount);
    currentMousePosition = currentMousePosition + Float2(translationAmount.x, translationAmount.y);
}