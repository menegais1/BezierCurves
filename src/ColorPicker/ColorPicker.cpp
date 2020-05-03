
#include "ColorPicker.h"
#include "../Vectors/Float3.h"
#include "../Canvas/gl_canvas2d.h"
#include "../Utilities.h"
#include <iostream>

void ColorPicker::mouse(int button, int state, int wheel, int direction, int x, int y) {
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
            //SLOW CODE, CAN BE OPTIMIZED
            notifyOnValueChangedListeners();
        }
    }
}

void ColorPicker::render() {
    for (float y = 0; y < height; y++) {
        for (float x = 0; x < width; x++) {
            int idx = y * width + x;
            Float3 renderColor = texture[idx];
            color(renderColor.x, renderColor.y, renderColor.z);
            point(position.x + x, position.y + y);
        }
    }

    color(0.2, 0.2, 0.2);
    circleFill(currentMousePosition.x, currentMousePosition.y, 6, 20);
}

Float3 ColorPicker::sampleTexture(Float2 position) {
    position = position - Float2(this->position.x, this->position.y);
    int idx = (int) position.y * width + (int) position.x;
    return texture[idx];
}

void ColorPicker::generateTexture() {
    int size = topVertices.size();
    float blockNumbers = size - 1;
    float blockSize = width / blockNumbers;

    for (float y = 0; y < height; y++) {
        for (float x = 0; x < width; x++) {
            int i = x / blockSize;
            float stepX = (x - (i * blockSize)) / blockSize;
            float stepY = y / height;
            Float3 interpX = lerp(topVertices[i], topVertices[i + 1], stepX);
            Float3 interpY = lerp(bottomVertices[i], bottomVertices[i + 1], stepX);
            Float3 interpColor = lerp(interpX, interpY, stepY);
            int idx = y * width + x;
            texture[idx] = interpColor;
        }
    }
}

void ColorPicker::addOnValueChangedListener(std::function<void(Float3 color)> listener) {
    this->onValueChangedListeners.push_back(listener);
}

void ColorPicker::notifyOnValueChangedListeners() {
    for (int i = 0; i < onValueChangedListeners.size(); i++) {
        this->onValueChangedListeners[i](currentSample);
    }
}

ColorPicker::ColorPicker(Float3 position, int width, int height) {
    topVertices.push_back({1, 0, 0});
    topVertices.push_back({1, 1, 0});
    topVertices.push_back({0, 1, 0});
    topVertices.push_back({0, 1, 1});
    topVertices.push_back({0, 0, 1});
    topVertices.push_back({1, 0, 1});
    topVertices.push_back({1, 0, 0});
    bottomVertices.push_back({1, 1, 1});
    bottomVertices.push_back({1, 1, 1});
    bottomVertices.push_back({1, 1, 1});
    bottomVertices.push_back({1, 1, 1});
    bottomVertices.push_back({1, 1, 1});
    bottomVertices.push_back({1, 1, 1});
    bottomVertices.push_back({1, 1, 1});
    this->position = position;
    this->width = width;
    this->height = height;
    this->texture = new Float3[width * height];
    mouseDragging = false;
    this->currentMousePosition = {position.x + width / 2, position.y + height / 2};
    generateTexture();
    this->currentSample = sampleTexture(currentMousePosition);
}

bool ColorPicker::pointIntersectsObject(Float3 point) {
    return isPointInsideBounds({point.x, point.y}, {position.x, position.y}, {width, height});
}

void ColorPicker::translate(Float3 translationAmount) {
    CanvasObject::translate(translationAmount);
    currentMousePosition = currentMousePosition + Float2(translationAmount.x, translationAmount.y);
}
