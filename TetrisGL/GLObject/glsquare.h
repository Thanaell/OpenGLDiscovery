#ifndef GLSQUARE_H
#define GLSQUARE_H
#include <QObject>
#include <QOpenGLBuffer>
#include <QVector3D>
#include <QVector4D>
#include <QMatrix4x4>
#include <QVector2D>
#include "globject.h"

//GL Square, for each square of Tetris shapes
class GLSquare : public GLObject
{
public:
    GLSquare();
};
#endif // GLSQUARE_H
