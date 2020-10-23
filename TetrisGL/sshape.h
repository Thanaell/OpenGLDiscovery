#ifndef SSHAPE_H
#define SSHAPE_H


#include "movableshape.h"
class SShape : public MovableShape
{
public:
    SShape();
    ShapeType getType() const override;
};

#endif // SSHAPE_H
