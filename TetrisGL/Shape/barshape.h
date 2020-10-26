#ifndef BARSHAPE_H
#define BARSHAPE_H

#include "movableshape.h"
class BarShape : public MovableShape
{
public:
    BarShape();
    ShapeType getType() const override;
};
#endif // BARSHAPE_H
