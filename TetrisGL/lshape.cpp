#include "lshape.h"

LShape::LShape():MovableShape(3)
{
    m_squares.push_back(QPoint(0,0));
    m_squares.push_back(QPoint(0,1));
    m_squares.push_back(QPoint(0,2));
    m_squares.push_back(QPoint(1,0));
}

ShapeType LShape::getType() const{
    return ShapeType::L;
}
