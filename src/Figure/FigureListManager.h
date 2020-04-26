//Class responsible for handling the created figures
//Serializing and deserializing them, dealing with selected figures, translation
//rotation and scaling, changing color, adding and deleting figures, and
//dealing with the zIndex ordered figure list

#ifndef FIGURELISTMANAGER_H
#define FIGURELISTMANAGER_H

#include <vector>
#include <string>
#include <fstream>
#include "../Figure/Figure.h"

class FigureListManager {
public:
    void deleteSelectedFigures();

    Float3 calculateSelectedFiguresCenter();

    void addFigure(Figure *figure);

    void updateSelectedFiguresZIndex(int zIndexModifier);

    void selectFigure(Figure *figure);

    void deselectFigure(Figure *figure);

    void clearSelectedFigures();

    void setDrawBounds(bool drawBounds);

    void translateFigures(Float3 translationAmount);

    void rotateFigures(float angle, Float3 center);

    void rescaleFigures(Float3 scale, Float3 center);

    void setSelectedFiguresColor(Float3 color, bool fillColor);

    void setSelectedFiguresRender(FigureRender figureRender);

    Figure *getFirstInteractedFigure(Float2 mousePosition);

    bool isFiguresSelected();

    void setRenderZIndex(bool b);

private:
    std::vector<Figure *> figures;
    std::vector<Figure *> selectedFigures;

    void updateFigureZIndex(Figure *figure);

    void addFigure(std::vector<Figure *> &figures, Figure *figure);

    bool removeFigure(std::vector<Figure *> &figures, Figure *figure);

};

#endif
