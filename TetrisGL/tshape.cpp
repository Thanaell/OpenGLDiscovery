#include "tshape.h"

TShape::TShape():MovableShape(3)
{
    m_squares.push_back(QPoint(0,0));
    m_squares.push_back(QPoint(1,0));
    m_squares.push_back(QPoint(1,1));
    m_squares.push_back(QPoint(2,0));
}

ShapeType TShape::getType() const{
    return ShapeType::T;
}
