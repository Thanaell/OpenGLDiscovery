#ifndef BOTTOMSHAPE_H
#define BOTTOMSHAPE_H

#include "shape.h"
#include <map>

class BottomShape : public Shape
{
public:
    BottomShape(int size);
    void updateHeightMap();
    int getHeight(int i){ return m_heightPerColumn[i];};
private:
    std::map<int,int> m_heightPerColumn;
};

#endif // BOTTOMSHAPE_H
