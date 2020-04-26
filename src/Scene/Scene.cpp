#include "Scene.h"
#include "../Utilities.h"
#include "../Figure/Figure.h"
#include "../Canvas/gl_canvas2d.h"
#include "../Managers/GlobalManager.h"
#include <iostream>
#include <algorithm>
#include <cmath>
#include <sstream>
#include "../Vectors/Float2.h"
#include "../Vectors/Float3.h"
#include "../Vectors/Float4.h"

void Scene::mouse(int button, int state, int wheel, int direction, int x, int y) {
    currentMousePosition = {x, y};
    switch (mode) {
        case SceneMode::Insert:
            handleInsertMode(button, state);
            break;
        case SceneMode::Translate:
            handleTranslateMode();
            break;
        case SceneMode::Rotate:
            handleRotateMode();
            break;
        case SceneMode::Scale:
            handleScaleMode();
            break;
        case SceneMode::Default:
            handleDefaultMode(button, state);
            break;
    }
    lastMousePosition = currentMousePosition;
}

void Scene::singleSelection(int x, int y) {
    if (isMouseInsideObject()) {
        figureListManager.clearSelectedFigures();
    }
    Figure *fig = figureListManager.getFirstInteractedFigure({x, y});
    if (fig != nullptr) {
        if (!fig->isMouseInsideObject()) return;
        figureListManager.clearSelectedFigures();
        figureListManager.selectFigure(fig);
    }
}

void Scene::multipleSelection(int x, int y) {
    Figure *fig = figureListManager.getFirstInteractedFigure({x, y});
    if (fig != nullptr) {
        if (!fig->isMouseInsideObject()) return;
        figureListManager.selectFigure(fig);
    } else if (isMouseInsideObject()) {
        figureListManager.clearSelectedFigures();
    }
}

void Scene::insertNewFigure() {
    if (tmpVertices.size() < 2)
        return;
    Float3 backgroundColor = {0.2, 0.2, 0.2};
    Float3 lineColor = {0, 1, 0};
    tmpVertices.push_back(tmpVertices[0]);
    Figure *fig = new Figure(backgroundColor, lineColor, highlightColor, tmpVertices);
    fig->setZIndex(0);
    fig->drawBounds = drawBounds;
    figureListManager.addFigure(fig);
}

void Scene::keyboard(int key) {
    if (mode == SceneMode::Default && key == 214) {
        multipleSelect = true;
    }
}

void Scene::keyboardUp(int key) {
    ignoreUpperCaseChar(key);
    handleSceneMode(static_cast<SceneMode>(key));
    handleSceneOperator(static_cast<Operator>(key));
}

void Scene::handleSceneMode(SceneMode sceneMode) {
    switch (sceneMode) {
        case SceneMode::Insert:
            setInsertMode();
            break;
        case SceneMode::Translate:
            if (figureListManager.isFiguresSelected())
                setTranslateMode();
            break;
        case SceneMode::Rotate:
            if (figureListManager.isFiguresSelected())
                setRotateMode();
            break;
        case SceneMode::Scale:
            if (figureListManager.isFiguresSelected())
                setScaleMode();
            break;
    }
}

void Scene::handleSceneOperator(Operator op) {
    switch (op) {
        case Operator::MultipleSelect:
            multipleSelect = false;
            break;
        case Operator::InsertPolygon:
            if (mode == SceneMode::Insert) {
                insertNewFigure();
                mode = SceneMode::Default;
            }
            break;
        case Operator::FixY:
            fixatedAxis = {0, 1};
            break;
        case Operator::FixX:
            fixatedAxis = {1, 0};
            break;
        case Operator::RenderBounds:
            drawBounds = !drawBounds;
            drawPolygonBounds();
            break;
        case Operator::DeleteSelected:
            figureListManager.deleteSelectedFigures();
            break;
        default:
            break;
    }
}

void Scene::drawPolygonBounds() {
    figureListManager.setDrawBounds(drawBounds);
}

void Scene::render() {
    color(1, 1, 1, 1);
    if (tmpVertices.size() > 0 && mode == SceneMode::Insert)
        renderPolygonInsertion();

    renderCurrentMode();

    if (mode == SceneMode::Rotate || mode == SceneMode::Scale) {
        line(selectionCenter.x, selectionCenter.y, currentMousePosition.x, currentMousePosition.y);
    }
}

void Scene::renderPolygonInsertion() {
    int size = tmpVertices.size();
    for (int i = 0; i < size; i++) {
        color(1, 1, 1);
        circle(tmpVertices[i].x, tmpVertices[i].y, 3, 10);
    }
    for (int i = 0; i < size - 1; i++) {
        color(1, 0, 0);
        line(tmpVertices[i].x, tmpVertices[i].y, tmpVertices[i + 1].x, tmpVertices[i + 1].y);
    }
    line(tmpVertices[size - 1].x, tmpVertices[size - 1].y, tmpVertices[0].x, tmpVertices[0].y);
}

void Scene::renderCurrentMode() {

    std::stringstream stream;
    int screenHeight = *GlobalManager::getInstance()->screenHeight;
    switch (mode) {
        case SceneMode::Default:
            color(1, 1, 1);
            text(20, screenHeight - 10, "Mode: Default");
            break;
        case SceneMode::Insert:
            color(1, 1, 1);
            stream << "Mode: Insert";
            text(20, screenHeight - 10, stream.str().c_str());
            text(20, screenHeight - 23, "Left mouse: Insert point");
            text(20, screenHeight - 35, "I key: clear points");
            text(20, screenHeight - 47, "Enter key: finish insertion");
            text(20, screenHeight - 59,
                 "Keys from 0-9: Change figure type, 0 is free polygon, 1 is circle, 3 is triangle... 9 is nonagon");
            break;
        case SceneMode::Translate:
            color(1, 1, 1);
            text(20, screenHeight - 10, "Mode: Translation");
            text(20, screenHeight - 23, "Move Mouse: Translate selected figures");
            text(20, screenHeight - 35, "T Key: finish translation");
            text(20, screenHeight - 47, "X Key: Fixate x axis");
            text(20, screenHeight - 59, "Y Key: Fixate y axis");
            break;
        case SceneMode::Rotate:
            color(1, 1, 1);
            text(20, screenHeight - 10, "Mode: Rotation");
            text(20, screenHeight - 23, "Move Mouse: Rotate selected figures");
            text(20, screenHeight - 35, "R Key: finish rotation");
            break;
        case SceneMode::Scale:
            color(1, 1, 1);
            text(20, screenHeight - 10, "Mode: Scaling");
            text(20, screenHeight - 23, "Move Mouse: Scale selected figures");
            text(20, screenHeight - 35, "S Key: finish scaling");
            text(20, screenHeight - 47, "X Key: Fixate x axis");
            text(20, screenHeight - 59, "Y Key: Fixate y axis");
            break;
        default:
            break;
    }
}

void Scene::ignoreUpperCaseChar(int &key) {
    if (key >= 'A' && key <= 'Z') {
        key = std::tolower(key);
    }
}

Scene::Scene() {
    mode = SceneMode::Default;
    highlightColor = {245 / 255.0, 195 / 255.0, 120 / 255.0, 0.8};
    drawBounds = false;
    this->scale = Float3(*GlobalManager::getInstance()->screenWidth, *GlobalManager::getInstance()->screenHeight, 0);
    this->setZIndex(-10000);
}

void Scene::setInsertMode() {
    mode = SceneMode::Insert;
    tmpVertices.clear();
}

void Scene::setTranslateMode() {
    if (mode == SceneMode::Translate) {
        mode = SceneMode::Default;
    } else {
        mode = SceneMode::Translate;
        lastMousePosition = GlobalManager::getInstance()->mousePosition;
        fixatedAxis = {1, 1};
    }
}

void Scene::setScaleMode() {
    if (mode == SceneMode::Scale) {
        mode = SceneMode::Default;
    } else {
        mode = SceneMode::Scale;
        lastMousePosition = GlobalManager::getInstance()->mousePosition;
        selectionCenter = figureListManager.calculateSelectedFiguresCenter();
        fixatedAxis = {1, 1};
    }
}

void Scene::setRotateMode() {
    if (mode == SceneMode::Rotate) {
        mode = SceneMode::Default;
    } else {
        mode = SceneMode::Rotate;
        selectionCenter = figureListManager.calculateSelectedFiguresCenter();
    }
}

void Scene::setDefaultMode() {
    mode = SceneMode::Default;
}

void Scene::handleInsertMode(int button, int state) {
    if (leftMouseDown(button, state)) {
        tmpVertices.push_back({currentMousePosition.x, currentMousePosition.y, 0});
    }
}

void Scene::handleTranslateMode() {
    Float3 translation = {currentMousePosition.x - lastMousePosition.x, currentMousePosition.y - lastMousePosition.y,
                          0};
    figureListManager.translateFigures({translation.x * fixatedAxis.x, translation.y * fixatedAxis.y, 0});
}

void Scene::handleScaleMode() {
    Float3 scale1 = {currentMousePosition.x - selectionCenter.x, currentMousePosition.y - selectionCenter.y, 0};
    Float3 scale2 = {lastMousePosition.x - selectionCenter.x, lastMousePosition.y - selectionCenter.y, 0};

    float scaleAmount = scale1.length() / scale2.length();
    Float3 scale = {fixatedAxis.x != 0 ? scaleAmount : 1, fixatedAxis.y != 0 ? scaleAmount : 1, 0};
    figureListManager.rescaleFigures(scale, selectionCenter);
}

void Scene::handleRotateMode() {
    Float2 vector1 = Float2(currentMousePosition.x - selectionCenter.x,
                            currentMousePosition.y - selectionCenter.y).unit();
    Float2 vector2 = Float2(lastMousePosition.x - selectionCenter.x, lastMousePosition.y - selectionCenter.y).unit();
    float direction = vector1.x * vector2.y - vector1.y * vector2.x;
    direction = direction > 0 ? -1 : 1;
    float dot = vector1.x * vector2.x + vector1.y * vector2.y;
    dot = dot > 1 ? 1 : dot;
    float angle = std::acos(dot);
    figureListManager.rotateFigures(angle * direction, selectionCenter);
}

void Scene::handleDefaultMode(int button, int state) {
    bool isLeftMouseClicked = leftMouseDown(button, state);
    if (!isLeftMouseClicked)
        return;
    if (multipleSelect) {
        multipleSelection(currentMousePosition.x, currentMousePosition.y);
    } else {
        singleSelection(currentMousePosition.x, currentMousePosition.y);
    }
}

bool Scene::pointIntersectsObject(Float3 point) {
    return true;
}
