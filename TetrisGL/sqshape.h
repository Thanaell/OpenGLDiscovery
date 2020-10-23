#ifndef SQSHAPE_H
#define SQSHAPE_H


#include "movableshape.h"
class SQShape : public MovableShape
{
public:
    SQShape();
    ShapeType getType() const override;
};
#endif // SQSHAPE_H
