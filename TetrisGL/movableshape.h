#ifndef MOVABLESHAPE_H
#define MOVABLESHAPE_H

#include "shape.h"
#include <memory>

enum ShapeType{
    EMPTY,S,Z,L,IL,Bar,SQ,T
};


class MovableShape : public Shape
{
public:
    static std::unique_ptr<MovableShape> createMovableShape();
    MovableShape(int size);
    void rotateClockwise();
    void rotateAntiClockwise();
    virtual ShapeType getType() const = 0;
    void updateEdgeSpaces();
    int getLeftSpace(){return m_leftSpace;};
    int getRightSpace(){return m_rightSpace;};
    int getBottomSpace(){return m_bottomSpace;};
    int getLowestY(int column);
    int getVerticalSize();
    std::vector<QPoint> getAbsoluteSquares(QPoint shapePosition);

protected:
    int m_leftSpace;
    int m_rightSpace;
    int m_bottomSpace;
};

#endif // MOVABLESHAPE_H
