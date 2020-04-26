
#include <vector>
#include <iostream>
#include <fstream>
#include <algorithm>
#include "../Managers/GlobalManager.h"
#include "../Figure/Figure.h"
#include "FigureListManager.h"

void FigureListManager::deleteSelectedFigures() {
    if (selectedFigures.size() == 0)
        return;
    for (int i = 0; i < selectedFigures.size(); i++) {
        Figure *fig = selectedFigures[i];
        removeFigure(figures, fig);
        GlobalManager::getInstance()->deleteObject(fig);
    }

    selectedFigures.clear();
}

void FigureListManager::clearSelectedFigures() {
    for (int i = 0; i < selectedFigures.size(); i++) {
        selectedFigures[i]->isSelected = false;
    }
    selectedFigures.clear();
}

void FigureListManager::addFigure(Figure *figure) {
    addFigure(figures, figure);
}

Float3 FigureListManager::calculateSelectedFiguresCenter() {
    Float3 mean = {0, 0, 0};
    int size = selectedFigures.size();
    for (int i = 0; i < size; i++) {
        mean.x += selectedFigures[i]->getCenter().x;
        mean.y += selectedFigures[i]->getCenter().y;
        mean.z += selectedFigures[i]->getCenter().z;
    }

    return {mean.x / size, mean.y / size, mean.z / size};
}

void FigureListManager::setSelectedFiguresRender(FigureRender figureRender) {
    int size = selectedFigures.size();
    for (int i = 0; i < size; i++) {
        if (figureRender == FigureRender::FillOnly) {
            selectedFigures[i]->backgroundAlpha = 1;
            selectedFigures[i]->lineAlpha = 0;
        }
        if (figureRender == FigureRender::LineOnly) {
            selectedFigures[i]->backgroundAlpha = 0;
            selectedFigures[i]->lineAlpha = 1;
        }
        if (figureRender == FigureRender::FillLine) {
            selectedFigures[i]->backgroundAlpha = 1;
            selectedFigures[i]->lineAlpha = 1;
        }
    }

}

void FigureListManager::updateSelectedFiguresZIndex(int zIndexModifier) {
    for (int i = 0; i < selectedFigures.size(); i++) {
        selectedFigures[i]->setZIndex(selectedFigures[i]->getZIndex() + zIndexModifier);
        updateFigureZIndex(selectedFigures[i]);
    }
}

void FigureListManager::updateFigureZIndex(Figure *figure) {
    if (figure->isSelected) {
        removeFigure(selectedFigures, figure);
        addFigure(selectedFigures, figure);
    }

    removeFigure(figures, figure);
    addFigure(figures, figure);
}

void FigureListManager::selectFigure(Figure *figure) {
    figure->isSelected = true;
    addFigure(selectedFigures, figure);
}

void FigureListManager::deselectFigure(Figure *figure) {
    figure->isSelected = false;
    removeFigure(selectedFigures, figure);
}

Figure *FigureListManager::getFirstInteractedFigure(Float2 mousePosition) {
    int size = figures.size();
    for (int i = 0; i < size; i++) {
        Figure *fig = figures[i];
        if (isPointInsidePolygon(mousePosition, fig->vertices, fig->vertices.size() - 1)) {
            return fig;
        }
    }

    return nullptr;
}

void FigureListManager::setDrawBounds(bool drawBounds) {
    for (int i = 0; i < figures.size(); i++) {
        figures[i]->drawBounds = drawBounds;
    }
}

void FigureListManager::addFigure(std::vector<Figure *> &figures, Figure *figure) {
    int size = figures.size();
    if (size == 0) {
        figures.push_back(figure);
    } else {
        for (int i = 0; i < size; i++) {
            if (figures[i]->getZIndex() <= figure->getZIndex()) {
                figures.insert(figures.begin() + i, figure);
                break;
            }
        }
        if (size == figures.size())
            figures.push_back(figure);
    }
}

bool FigureListManager::removeFigure(std::vector<Figure *> &figures, Figure *figure) {
    auto iterator = std::find(figures.begin(), figures.end(), figure);
    if (iterator != figures.cend()) {
        figures.erase(iterator);
        return true;
    }
    return false;
}

void FigureListManager::translateFigures(Float3 translationAmount) {

    for (int i = 0; i < selectedFigures.size(); i++) {
        selectedFigures[i]->translate({translationAmount.x, translationAmount.y, translationAmount.z});
    }
}

void FigureListManager::rotateFigures(float angle, Float3 center) {
    for (int i = 0; i < selectedFigures.size(); i++) {
        selectedFigures[i]->rotate(angle, center);
    }
}

void FigureListManager::rescaleFigures(Float3 scale, Float3 center) {
    for (int i = 0; i < selectedFigures.size(); i++) {
        selectedFigures[i]->rescale({scale.x, scale.y, scale.z}, center);
    }
}

void FigureListManager::setSelectedFiguresColor(Float3 color, bool fillColor) {
    for (int i = 0; i < selectedFigures.size(); i++) {
        if (fillColor)
            selectedFigures[i]->backgroundColor = color;
        else
            selectedFigures[i]->lineColor = color;
    }
}

void FigureListManager::serializeFigures(std::string filename) {
    std::ofstream file(filename, std::ios::binary | std::ios::out);
    int figuresSize = figures.size();
    file.write(reinterpret_cast<char *>(&figuresSize), sizeof(int));
    for (int i = 0; i < figuresSize; ++i) {
        Figure *f = figures[i];
        f->drawBounds = false;
        f->isSelected = false;
        file.write(reinterpret_cast<char *>(&f->backgroundColor), sizeof(Float3));
        file.write(reinterpret_cast<char *>(&f->lineColor), sizeof(Float3));
        file.write(reinterpret_cast<char *>(&f->highlightColor), sizeof(Float4));
        file.write(reinterpret_cast<char *>(&f->bounds), sizeof(Bounds));
        file.write(reinterpret_cast<char *>(&f->drawBounds), sizeof(bool));
        file.write(reinterpret_cast<char *>(&f->isSelected), sizeof(bool));
        file.write(reinterpret_cast<char *>(&f->backgroundAlpha), sizeof(float));
        file.write(reinterpret_cast<char *>(&f->lineAlpha), sizeof(float));
        file.write(reinterpret_cast<char *>(&f->position), sizeof(Float3));
        int zIndex = f->getZIndex();
        file.write(reinterpret_cast<char *>(&zIndex), sizeof(int));
        int verticesSize = f->vertices.size();
        file.write(reinterpret_cast<char *>(&verticesSize), sizeof(int));
        Float3 *data = f->vertices.data();
        for (int j = 0; j < verticesSize; ++j) {
            file.write(reinterpret_cast<char *>(&data[j]), sizeof(Float3));
        }
    }
    file.close();
}

void FigureListManager::deserializeFigures(std::string filename) {
    std::ifstream file(filename, std::ios::binary | std::ios::in);
    if (!file.is_open() || file.bad()) return;
    selectedFigures.clear();
    for (int k = 0; k < figures.size(); ++k) {
        GlobalManager::getInstance()->deleteObject(figures[k]);
    }
    figures.clear();
    int figuresSize;
    file.read(reinterpret_cast<char *>(&figuresSize), sizeof(int));
    for (int j = 0; j < figuresSize; ++j) {
        Figure *f = new Figure();
        file.read(reinterpret_cast<char *>(&f->backgroundColor), sizeof(Float3));
        file.read(reinterpret_cast<char *>(&f->lineColor), sizeof(Float3));
        file.read(reinterpret_cast<char *>(&f->highlightColor), sizeof(Float4));
        Bounds b;
        file.read(reinterpret_cast<char *>(&b), sizeof(Bounds));
        f->bounds = b;
        file.read(reinterpret_cast<char *>(&f->drawBounds), sizeof(bool));
        file.read(reinterpret_cast<char *>(&f->isSelected), sizeof(bool));
        file.read(reinterpret_cast<char *>(&f->backgroundAlpha), sizeof(float));
        file.read(reinterpret_cast<char *>(&f->lineAlpha), sizeof(float));
        file.read(reinterpret_cast<char *>(&f->position), sizeof(Float3));
        int zIndex = 0;
        file.read(reinterpret_cast<char *>(&zIndex), sizeof(int));
        f->setZIndex(zIndex);
        int verticesSize = 0;
        file.read(reinterpret_cast<char *>(&verticesSize), sizeof(int));
        for (int i = 0; i < verticesSize; ++i) {
            Float3 tmp;
            file.read(reinterpret_cast<char *>(&tmp), sizeof(Float3));
            f->vertices.push_back(tmp);
        }
        f->computeCentroid();
        addFigure(figures, f);
    }

    file.close();
}

bool FigureListManager::isFiguresSelected() {
    return !selectedFigures.empty();
}

void FigureListManager::setRenderZIndex(bool drawZIndex) {
    for (int i = 0; i < figures.size(); i++) {
        figures[i]->drawZIndex = drawZIndex;
    }
}
