#include "bottomshape.h"

BottomShape::BottomShape(int size):Shape(size)
{
    for (int i=0; i<m_shapeSize;i++){
         m_heightPerColumn[i]=0;
    }
}

void BottomShape::updateHeightMap(){
    for (int i=0; i<m_shapeSize;i++){
        for (auto square : m_squares){
            if (square.x()==i && square.y()>m_heightPerColumn[i]){
                m_heightPerColumn[i]=square.y();
            }
        }
    }
}
