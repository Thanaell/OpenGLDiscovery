#ifndef TSHAPE_H
#define TSHAPE_H

#include "movableshape.h"

class TShape : public MovableShape
{
public:
    TShape();
    ShapeType getType() const override;
};

#endif // TSHAPE_H
