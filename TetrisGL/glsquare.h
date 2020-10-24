#ifndef GLSQUARE_H
#define GLSQUARE_H
#include <QObject>
#include <QOpenGLBuffer>
#include <QVector3D>
#include <QVector4D>
#include <QMatrix4x4>
#include <QVector2D>

class GLSquare
{
public:
    GLSquare();
    QOpenGLBuffer * getVBO() const {return m_vbo;};
    const GLfloat * const_data(){return m_data.constData();};
    void add(const QVector3D &v, const QVector2D &tex);
    int count() const { return m_count; }
    int vertexCount() const { return m_count / 6; }
    QMatrix4x4 getModelMatrix() const {return m_modelMatrix;};
    void translateModelMatrix(QVector3D);
;
private :
    QOpenGLBuffer * m_vbo;
    QVector<GLfloat> m_data;
    int m_count;
    QMatrix4x4 m_modelMatrix;
    static int scaleFactor;

signals:

};
#endif // GLSQUARE_H
