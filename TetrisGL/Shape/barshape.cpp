#include "barshape.h"

BarShape::BarShape():MovableShape(4)
{
    m_squares.push_back(QPoint(0,2));
    m_squares.push_back(QPoint(1,2));
    m_squares.push_back(QPoint(2,2));
    m_squares.push_back(QPoint(3,2));

}

ShapeType BarShape::getType() const{
    return ShapeType::Bar;
}
