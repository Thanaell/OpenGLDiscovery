#include "tshape.h"

//---------------------------------
TShape::TShape():MovableShape(3)
{
    m_squares.push_back(QPoint(0,1));
    m_squares.push_back(QPoint(1,1));
    m_squares.push_back(QPoint(1,2));
    m_squares.push_back(QPoint(2,1));
}

//---------------------------------
ShapeType TShape::getType() const{
    return ShapeType::T;
}
