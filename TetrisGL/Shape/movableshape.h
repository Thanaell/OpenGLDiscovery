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
    MovableShape(int size);

    virtual ShapeType getType() const = 0;
    int getLowestYColumn(int column) const;
    int getLowestYShape() const ;
    int getVerticalSize() const;
    std::vector<QPoint> getAbsoluteSquares(QPoint shapePosition) const;

    void rotateClockwise();
    void rotateAntiClockwise();

    static std::unique_ptr<MovableShape> createMovableShape();
    static int getMaxShapeSize();
    static void updateRandomShapesVec();

private:
    static std::map<ShapeType,int> weightPerShape;
    static std::vector<ShapeType> randomShapesVec;

};

#endif // MOVABLESHAPE_H
