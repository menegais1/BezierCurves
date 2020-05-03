//Floating window offering a callback to any one desiring to use the color picker widget
//can be moved, activated and deactivated as it is a panel


#ifndef COLORPICKERPANEL_H
#define COLORPICKERPANEL_H

#include "../Base/CanvasObject.h"
#include "../Panel/Panel.h"
#include "../Vectors/Float3.h"
#include "ColorPicker.h"
#include "ColorSlider.h"
#include "../Button/Button.h"
#include <vector>
#include <functional>

class ColorPickerPanel : public Panel {
public:
    void mouse(int button, int state, int wheel, int direction, int x, int y) override;

    void render() override;

    bool pointIntersectsObject(Float3 point) override;

    ColorPickerPanel(Float3 position, Float3 scale, Float3 backgroundColor);

    void addOnValueChangedListener(std::function<void(Float3 color)> listener);

    void notifyOnValueChangedListeners();

private:
    bool mouseDragging;
    bool mouseInside;
    Float2 currentMousePosition;
    Float2 lastMousePosition;
    ColorPicker *colorPicker;
    ColorSlider *colorSlider;
    Button *closeButton;
    Float3 colorPreview;
    std::vector<std::function<void(Float3 color)>> onValueChangedListeners;
    std::vector<std::function<void(bool isActive)>> onActivateListeners;
};

#endif