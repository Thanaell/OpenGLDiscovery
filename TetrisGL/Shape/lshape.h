#ifndef LSHAPE_H
#define LSHAPE_H


#include "movableshape.h"
class LShape : public MovableShape
{
public:
    LShape();
    ShapeType getType() const override;
};

#endif // LSHAPE_H
