#ifndef MOVABLESHAPE_H
#define MOVABLESHAPE_H

#include "shape.h"

enum ShapeType{
    EMPTY,S,Z,L,IL,Bar,SQ,T
};


class MovableShape : public Shape
{
public:
    MovableShape(int size);
    void rotateClockwise();
    void rotateAntiClockwise();
    virtual ShapeType getType() const = 0;
    virtual int getSize() const {return m_shapeSize;};

protected:
    int m_shapeSize;
};

#endif // MOVABLESHAPE_H
