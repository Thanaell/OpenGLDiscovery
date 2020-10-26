#ifndef ILSHAPE_H
#define ILSHAPE_H


#include "movableshape.h"
class ILShape : public MovableShape
{
public:
    ILShape();
    ShapeType getType() const override;
};

#endif // ILSHAPE_H
