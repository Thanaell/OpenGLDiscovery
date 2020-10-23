#include "ilshape.h"
#include <QDebug>

ILShape::ILShape():MovableShape(3)
{
    m_squares.push_back(QPoint(0,0));
    m_squares.push_back(QPoint(1,0));
    m_squares.push_back(QPoint(2,0));
    m_squares.push_back(QPoint(2,1));
}

ShapeType ILShape::getType() const{
    return ShapeType::IL;
}
