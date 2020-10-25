#ifndef MOVABLESHAPE_H
#define MOVABLESHAPE_H

#include "shape.h"
#include <memory>
#include <map>

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
    int getLowestY(int column);
    int getVerticalSize();
    std::vector<QPoint> getAbsoluteSquares(QPoint shapePosition);
    static int getMaxShapeSize();
    static void updateRandomShapesVec();

protected:
    int m_leftSpace;
    int m_rightSpace;
    int m_bottomSpace;
private:
    static std::map<ShapeType,int> weightPerShape;
    static std::vector<ShapeType> randomShapesVec;

};

#endif // MOVABLESHAPE_H
