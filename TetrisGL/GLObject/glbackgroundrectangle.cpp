#include "glbackgroundrectangle.h"

GLBackgroundRectangle::GLBackgroundRectangle(float ratio)
{
    // Define a rectangle with width of 1 and length of ratio
    std::vector<std::pair<QVector3D,QVector2D>> vertexes;
    vertexes.push_back({QVector3D(0,0,0),QVector2D(0,0)});
    vertexes.push_back({QVector3D(ratio,1,0),QVector2D(1,1)});
    vertexes.push_back({QVector3D(0,1,0),QVector2D(0,1)});

    vertexes.push_back({QVector3D(0,0,0),QVector2D(0,0)});
    vertexes.push_back({QVector3D(ratio,0,0),QVector2D(1,0)});
    vertexes.push_back({QVector3D(ratio,1,0),QVector2D(1,1)});

    m_data.resize(5*vertexes.size());
    m_vbo=new QOpenGLBuffer();
    m_count=0;
    m_modelMatrix=QMatrix4x4();
    m_modelMatrix.setToIdentity();
    for (auto element : vertexes){
        add(element.first,element.second);
    }
}
