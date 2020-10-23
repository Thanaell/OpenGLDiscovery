#include "barshape.h"

BarShape::BarShape():MovableShape(4)
{
    m_squares.push_back(QPoint(0,0));
    m_squares.push_back(QPoint(1,0));
    m_squares.push_back(QPoint(2,0));
    m_squares.push_back(QPoint(3,0));
}

ShapeType BarShape::getType() const{
    return ShapeType::Bar;
}
