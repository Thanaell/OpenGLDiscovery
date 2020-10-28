#ifndef GLOBJECT_H
#define GLOBJECT_H

#include <QObject>
#include <QOpenGLBuffer>
#include <QVector3D>
#include <QVector4D>
#include <QMatrix4x4>
#include <QVector2D>

//Base class for GL Objects
class GLObject
{
public:
    GLObject();
    const GLfloat * const_data(){return m_data.constData();};
    void add(const QVector3D &v, const QVector2D &tex);
    int count() const { return m_count; }
    int vertexCount() const { return m_count / 6; }
    QOpenGLBuffer * getVBO() const {return m_vbo;};
    QMatrix4x4 getModelMatrix() const {return m_modelMatrix;};
    void translateModelMatrix(QVector3D);
    void scale(int scaleFactor);
;
protected :
    QOpenGLBuffer * m_vbo;
    QVector<GLfloat> m_data;
    int m_count;
    QMatrix4x4 m_modelMatrix;

};


#endif // GLOBJECT_H
