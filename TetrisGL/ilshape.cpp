#include "ilshape.h"
#include <QDebug>

ILShape::ILShape():MovableShape(3)
{
    m_squares.push_back(QPoint(0,0));
    m_squares.push_back(QPoint(0,1));
    m_squares.push_back(QPoint(0,2));
    m_squares.push_back(QPoint(1,2));
}

ShapeType ILShape::getType() const{
    return ShapeType::IL;
}
