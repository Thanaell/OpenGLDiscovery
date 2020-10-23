#include "zshape.h"

ZShape::ZShape():MovableShape(3)
{
    m_squares.push_back(QPoint(0,1));
    m_squares.push_back(QPoint(1,1));
    m_squares.push_back(QPoint(1,0));
    m_squares.push_back(QPoint(2,0));
}

ShapeType ZShape::getType() const{
    return ShapeType::Z;
}
