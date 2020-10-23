#include "shape.h"

Shape::Shape()
{

}

void Shape::addSquare(QPoint p){
    m_squares.push_back(p);
}

void Shape::removeSquare(QPoint p){
    for (auto it = m_squares.begin(); it!=m_squares.end(); it++){
        if (*it==p){
            m_squares.erase(it);
            break;
        }
    }
}
