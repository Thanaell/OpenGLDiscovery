#include "movableshape.h"
#include <QDebug>
#include "tshape.h"
#include "barshape.h"
#include "lshape.h"
#include "ilshape.h"
#include "sqshape.h"
#include "zshape.h"
#include "sshape.h"

int intRand(int max){
    return rand()%max;
}

std::vector<QPoint> MovableShape::getAbsoluteSquares(QPoint shapePosition){
    std::vector<QPoint> result;
    for (auto square : m_squares){
        result.push_back(square+shapePosition);
    }
    return result;
}

int MovableShape::getLowestY(int column){
    QPoint lowestPoint(0,100);
    for (auto square : m_squares){
        if (square.x()==column && square.y()<lowestPoint.y()){
            lowestPoint=square;
        }
    }
    return lowestPoint.y();
}

void MovableShape::updateEdgeSpaces(){
    QPoint leftestPoint(100,0);
    QPoint rightestPoint(0,0);
    QPoint lowestPoint(0,100);
    for (auto point : m_squares){
        if (point.x()>rightestPoint.x()){
            rightestPoint=point;
        }
        if (point.x()<leftestPoint.x()){
            leftestPoint=point;
        }
        if (point.y()<lowestPoint.y()){
            lowestPoint=point;
        }
    }
    m_rightSpace=m_shapeSize-rightestPoint.x();
    m_leftSpace=leftestPoint.x();
    m_bottomSpace=lowestPoint.y();;
}

std::unique_ptr<MovableShape> MovableShape::createMovableShape(){
    int result=intRand(7);
    MovableShape * shape=nullptr;
    switch (result) {
    case 0:
        shape=new TShape();
        break;
    case 1:
        shape=new LShape();
        break;
    case 2:
        shape=new ILShape();
        break;
    case 3:
        shape=new ZShape();
        break;
    case 4:
        shape=new SShape();
        break;
    case 5:
        shape=new SQShape();
        break;
    case 6:
        shape=new BarShape();
        break;
    default:
        shape=new TShape();
    };
    shape->updateEdgeSpaces();
    return std::unique_ptr<MovableShape>(shape);
}

MovableShape::MovableShape(int size):Shape(size)
{

}

void MovableShape::rotateClockwise(){
    std::vector<QPoint> newSquares;
    for (auto point : m_squares){
        int newX=point.y();
        int newY=m_shapeSize-point.x()-1;
        point.setX(newX);
        point.setY(newY);
        newSquares.push_back(point);
    }
    m_squares=newSquares;
    updateEdgeSpaces();
}

void MovableShape::rotateAntiClockwise(){
    std::vector<QPoint> newSquares;
    for (auto point : m_squares){
        int newX=m_shapeSize-point.y()-1;
        int newY=point.x();
        point.setX(newX);
        point.setY(newY);
        newSquares.push_back(point);
    }
     m_squares=newSquares;
     updateEdgeSpaces();
}
