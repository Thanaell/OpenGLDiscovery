#include "ilshape.h"
#include <QDebug>

ILShape::ILShape():MovableShape(3)
{
    m_squares.push_back(QPoint(1,0));
    m_squares.push_back(QPoint(1,1));
    m_squares.push_back(QPoint(1,2));
    m_squares.push_back(QPoint(2,2));
}

ShapeType ILShape::getType() const{
    return ShapeType::IL;
}
