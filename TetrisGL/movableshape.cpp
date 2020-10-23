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

std::unique_ptr<MovableShape> MovableShape::createMovableShape(){
    int result=intRand(7);
    switch (result) {
    case 0:
        return std::unique_ptr<TShape>(new TShape());
    case 1:
        return std::unique_ptr<LShape>(new LShape());
    case 2:
        return std::unique_ptr<ILShape>(new ILShape());
    case 3:
        return std::unique_ptr<ZShape>(new ZShape());
    case 4:
        return std::unique_ptr<SShape>(new SShape());
    case 5:
        return std::unique_ptr<SQShape>(new SQShape());
    case 6:
        return std::unique_ptr<BarShape>(new BarShape());
    }
}

MovableShape::MovableShape(int size):Shape(),m_shapeSize(size)
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
}
