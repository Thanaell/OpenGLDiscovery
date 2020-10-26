#include "sqshape.h"

SQShape::SQShape():MovableShape(2)
{
    m_squares.push_back(QPoint(0,0));
    m_squares.push_back(QPoint(1,0));
    m_squares.push_back(QPoint(0,1));
    m_squares.push_back(QPoint(1,1));
}

ShapeType SQShape::getType() const{
    return ShapeType::SQ;
}
