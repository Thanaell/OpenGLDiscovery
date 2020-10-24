#include "globject.h"


GLObject::GLObject()
{

}

void GLObject::scale(int scaleFactor){
    m_modelMatrix.scale(scaleFactor);
}

void GLObject::translateModelMatrix(QVector3D pos){
    m_modelMatrix.translate(pos);
}

void GLObject::add(const QVector3D &pos, const QVector2D &tex)
{
    GLfloat *p = m_data.data() + m_count;
    *p++ = pos.x();
    *p++ = pos.y();
    *p++ = pos.z();
    *p++ = tex.x();
    *p++ = tex.y();

    m_count += 5;
}
