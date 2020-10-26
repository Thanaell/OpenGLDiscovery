#include "lshape.h"

LShape::LShape():MovableShape(3)
{
    m_squares.push_back(QPoint(1,0));
    m_squares.push_back(QPoint(1,1));
    m_squares.push_back(QPoint(1,2));
    m_squares.push_back(QPoint(2,0));
}

ShapeType LShape::getType() const{
    return ShapeType::L;
}
