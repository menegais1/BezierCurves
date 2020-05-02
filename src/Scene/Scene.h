//Class responsible for listening to all mouse and keyboard input,
//dealing with current program state and redirecting functions using the curveListManager
//Basically it's the main file of the program

#ifndef SCENE_H
#define SCENE_H

#include <vector>
#include "Curve/BezierCurve.h"
#include "../Vectors/Float3.h"
#include "../Vectors/Float4.h"
#include "../Curve/BezierCurveManager.h"

enum Operator {
    InsertPolygon = 13,
    SendToBack = 200,
    SendToFront = 202,
    MultipleSelect = 214,
    FixX = 'x',
    FixY = 'y',
    RenderBounds = 'b',
    DeleteSelected = 127,
    SelectFillColor = 'f',
    SelectLineColor = 'l',
    LoadFile = 'o',
    SaveFile = 'p',
    BackgroundVisible = 'g',
    LineVisible = 'h',
    BothVisible = 'j',
    RenderZIndex = 'z'
};

enum SceneMode {
    Insert = 'i',
    Translate = 't',
    Scale = 's',
    Rotate = 'r',
    Default = 0
};

class Scene : public CanvasObject {
public:
    void mouse(int button, int state, int wheel, int direction, int x, int y) override;

    void keyboardUp(int key) override;

    void keyboard(int key) override;

    void render() override;

    bool pointIntersectsObject(Float3 point) override;

    Scene();

private:
    BezierCurveManager curveListManager;
    std::vector<Float3> tmpVertices;
    int mode;
    bool multipleSelect;
    bool drawBounds;
    Float4 highlightColor;
    Float2 lastMousePosition;
    Float3 selectionCenter;
    Float2 currentMousePosition;
    Float2 fixatedAxis;

    void renderPolygonInsertion();

    void renderCurrentMode();

    void insertNewcurve();

    void singleSelection(int x, int y);

    void multipleSelection(int x, int y);

    void drawPolygonBounds();

    void handleSceneMode(SceneMode sceneMode);

    void handleSceneOperator(Operator op);

    //Scene Mode handle
    void setInsertMode();

    void setTranslateMode();

    void setScaleMode();

    void setRotateMode();

    void setDefaultMode();

    void handleInsertMode(int button, int state);

    void handleTranslateMode();

    void handleScaleMode();

    void handleRotateMode();

    void handleDefaultMode(int button, int state);

    //Components
    void ignoreUpperCaseChar(int &key);

};

#endif
