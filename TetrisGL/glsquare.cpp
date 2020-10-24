#include "glsquare.h"

int GLSquare::scaleFactor=3;

GLSquare::GLSquare()
{
    std::vector<QVector3D> vertexes;
    vertexes.push_back(QVector3D(-1,-1,0));
    vertexes.push_back(QVector3D(0,0,0));
    vertexes.push_back(QVector3D(-1,0,0));

    vertexes.push_back(QVector3D(-1,-1,0));
    vertexes.push_back(QVector3D(0,-1,0));
    vertexes.push_back(QVector3D(0,0,0));

    m_data.resize(3*vertexes.size());
    m_vbo=new QOpenGLBuffer();
    m_count=0;
    m_modelMatrix=QMatrix4x4();
    m_modelMatrix.setToIdentity();
    for (auto element : vertexes){
        add(element);
    }
}

void GLSquare::translateModelMatrix(QVector3D pos){
    m_modelMatrix.translate(pos);
}

void GLSquare::add(const QVector3D &pos)
{
    GLfloat *p = m_data.data() + m_count;
    *p++ = pos.x();
    *p++ = pos.y();
    *p++ = pos.z();
    m_count += 3;
}
