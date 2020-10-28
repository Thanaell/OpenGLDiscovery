#include "sshape.h"

//---------------------------------
SShape::SShape():MovableShape(3)
{
    m_squares.push_back(QPoint(0,0));
    m_squares.push_back(QPoint(1,0));
    m_squares.push_back(QPoint(1,1));
    m_squares.push_back(QPoint(2,1));
}

//---------------------------------
ShapeType SShape::getType() const{
    return ShapeType::S;
}
