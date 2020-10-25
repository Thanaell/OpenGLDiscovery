#include "movableshape.h"
#include <QDebug>
#include "tshape.h"
#include "barshape.h"
#include "lshape.h"
#include "ilshape.h"
#include "sqshape.h"
#include "zshape.h"
#include "sshape.h"
#include <set>

std::map<ShapeType,int> MovableShape::weightPerShape{ {ShapeType::Bar,2}
                                                     ,{ShapeType::SQ,2}
                                                     ,{ShapeType::Z,4}
                                                     ,{ShapeType::S,4}
                                                     ,{ShapeType::L,4}
                                                     ,{ShapeType::IL,4}
                                                     ,{ShapeType::T,2}
};

std::vector<ShapeType> MovableShape::randomShapesVec{};

int intRand(int max){
    return rand()%max;
}

int MovableShape::getMaxShapeSize(){
    return 4;
}

int MovableShape::getVerticalSize(){
    std::set<int> lines;
    for (auto square : m_squares){
        lines.insert(square.y());
    }
    return lines.size();
}

void MovableShape::updateRandomShapesVec(){
    if (randomShapesVec.empty()){
        for (auto shape : weightPerShape){
            for (int i =0; i<shape.second;i++){
                randomShapesVec.push_back(shape.first);
            }
        }
    }
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

std::unique_ptr<MovableShape> MovableShape::createMovableShape(){
    int result=intRand(randomShapesVec.size());
    ShapeType type=randomShapesVec[result];
    MovableShape * shape=nullptr;
    switch (type) {
    case ShapeType::T:
        shape=new TShape();
        break;
    case ShapeType::L:
        shape=new LShape();
        break;
    case ShapeType::IL:
        shape=new ILShape();
        break;
    case ShapeType::Z:
        shape=new ZShape();
        break;
    case ShapeType::S:
        shape=new SShape();
        break;
    case ShapeType::SQ:
        shape=new SQShape();
        break;
    case ShapeType::Bar:
        shape=new BarShape();
        break;
    default:
        shape=new TShape();
    };
    randomShapesVec.erase(randomShapesVec.begin()+result);
    updateRandomShapesVec();
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
