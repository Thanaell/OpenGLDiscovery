#ifndef ZSHAPE_H
#define ZSHAPE_H


#include "movableshape.h"
class ZShape : public MovableShape
{
public:
    ZShape();
    ShapeType getType() const override;
};
#endif // ZSHAPE_H
