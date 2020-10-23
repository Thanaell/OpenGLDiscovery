#include "movableshape.h"
#include <QDebug>

MovableShape::MovableShape(int size):Shape(),m_shapeSize(size)
{

}

void MovableShape::rotateClockwise(){
    std::vector<QPoint> newSquares;
    for (auto point : m_squares){
        int newX=point.y();
        int newY=m_shapeSize-point.x();
        point.setX(newX);
        point.setY(newY);
        newSquares.push_back(point);
        qDebug()<<point;
    }
    m_squares=newSquares;
}

void MovableShape::rotateAntiClockwise(){
    std::vector<QPoint> newSquares;
    for (auto point : m_squares){
        int newX=m_shapeSize-point.y();
        int newY=point.x();
        point.setX(newX);
        point.setY(newY);
        newSquares.push_back(point);
        qDebug()<<point;
    }
     m_squares=newSquares;
}
