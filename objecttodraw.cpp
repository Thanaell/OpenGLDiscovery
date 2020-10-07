#include "objecttodraw.h"

ObjectToDraw::ObjectToDraw(ObjectData data, ObjectType type, QObject *parent) : QObject(parent), m_type(type)
{
    m_data.resize(6*data.vertexesColors.size());
    m_vbo=new QOpenGLBuffer();
    m_count=0;
    m_transPS=data.trans;
    m_angle=data.angle;
    m_rotAxis=data.rotAxis;
    m_modelMatrix=QMatrix4x4();
    m_modelMatrix.setToIdentity();
    for (auto element : data.vertexesColors){
        add(element.first, element.second);
    }
}

void ObjectToDraw::setModelMatrix(QVector3D pos, float angle, QVector3D rotAxis){
    m_modelMatrix.translate(pos);
    m_modelMatrix.rotate(angle, rotAxis);
}

ObjectToDraw::ObjectToDraw(ObjectType type, float sideSize, QVector3D color, QObject *parent) :  QObject(parent), m_type(type)
{
    m_angle=0;
    m_transPS=QVector3D(0,0,0);
    m_rotAxis=QVector3D(0,0,0);

    std::vector<std::pair<QVector3D,QVector3D>> vertexesColors;
    if (type==ObjectType::TRIANGLE){
        vertexesColors.push_back(std::pair<QVector3D,QVector3D>(QVector3D(-sideSize,0,0),QVector3D(0,1,0)));
        vertexesColors.push_back(std::pair<QVector3D,QVector3D>(QVector3D(sideSize,0,0),QVector3D(0,0,1)));
        vertexesColors.push_back(std::pair<QVector3D,QVector3D>(QVector3D(0,sideSize,0),QVector3D(1,0,0)));
    }
    else if (type==ObjectType::CUBE){
        vertexesColors.push_back(std::pair<QVector3D,QVector3D>(QVector3D( -sideSize,-sideSize,-sideSize),color));
        vertexesColors.push_back(std::pair<QVector3D,QVector3D>(QVector3D(-sideSize,-sideSize, sideSize),color));
        vertexesColors.push_back(std::pair<QVector3D,QVector3D>(QVector3D(-sideSize, sideSize, sideSize),color));
        vertexesColors.push_back(std::pair<QVector3D,QVector3D>(QVector3D(sideSize, sideSize,-sideSize),color));
        vertexesColors.push_back(std::pair<QVector3D,QVector3D>(QVector3D(-sideSize,-sideSize,-sideSize),color));
        vertexesColors.push_back(std::pair<QVector3D,QVector3D>(QVector3D(-sideSize, sideSize,-sideSize),color));
        vertexesColors.push_back(std::pair<QVector3D,QVector3D>(QVector3D(sideSize,-sideSize, sideSize),color));
        vertexesColors.push_back(std::pair<QVector3D,QVector3D>(QVector3D(-sideSize,-sideSize,-sideSize),color));
        vertexesColors.push_back(std::pair<QVector3D,QVector3D>(QVector3D(sideSize,-sideSize,-sideSize),color));
        vertexesColors.push_back(std::pair<QVector3D,QVector3D>(QVector3D( sideSize, sideSize,-sideSize),color));
        vertexesColors.push_back(std::pair<QVector3D,QVector3D>(QVector3D(sideSize,-sideSize,-sideSize),color));
        vertexesColors.push_back(std::pair<QVector3D,QVector3D>(QVector3D( -sideSize,-sideSize,-sideSize),color));
        vertexesColors.push_back(std::pair<QVector3D,QVector3D>(QVector3D(-sideSize,-sideSize,-sideSize),color));
        vertexesColors.push_back(std::pair<QVector3D,QVector3D>(QVector3D(-sideSize, sideSize, sideSize),color));
        vertexesColors.push_back(std::pair<QVector3D,QVector3D>(QVector3D(-sideSize, sideSize,-sideSize),color));
        vertexesColors.push_back(std::pair<QVector3D,QVector3D>(QVector3D(sideSize,-sideSize, sideSize),color));
        vertexesColors.push_back(std::pair<QVector3D,QVector3D>(QVector3D(-sideSize,-sideSize, sideSize),color));
        vertexesColors.push_back(std::pair<QVector3D,QVector3D>(QVector3D(-sideSize,-sideSize,-sideSize),color));
        vertexesColors.push_back(std::pair<QVector3D,QVector3D>(QVector3D(-sideSize, sideSize, sideSize),color));
        vertexesColors.push_back(std::pair<QVector3D,QVector3D>(QVector3D( -sideSize,-sideSize, sideSize),color));
        vertexesColors.push_back(std::pair<QVector3D,QVector3D>(QVector3D( sideSize,-sideSize, sideSize),color));
        vertexesColors.push_back(std::pair<QVector3D,QVector3D>(QVector3D(sideSize, sideSize, sideSize),color));
        vertexesColors.push_back(std::pair<QVector3D,QVector3D>(QVector3D(sideSize,-sideSize,-sideSize),color));
        vertexesColors.push_back(std::pair<QVector3D,QVector3D>(QVector3D(sideSize, sideSize,-sideSize),color));
        vertexesColors.push_back(std::pair<QVector3D,QVector3D>(QVector3D(sideSize,-sideSize,-sideSize),color));
        vertexesColors.push_back(std::pair<QVector3D,QVector3D>(QVector3D(sideSize, sideSize, sideSize),color));
        vertexesColors.push_back(std::pair<QVector3D,QVector3D>(QVector3D(sideSize,-sideSize, sideSize),color));
        vertexesColors.push_back(std::pair<QVector3D,QVector3D>(QVector3D( sideSize, sideSize, sideSize),color));
        vertexesColors.push_back(std::pair<QVector3D,QVector3D>(QVector3D(sideSize, sideSize,-sideSize),color));
        vertexesColors.push_back(std::pair<QVector3D,QVector3D>(QVector3D(-sideSize, sideSize,-sideSize),color));
        vertexesColors.push_back(std::pair<QVector3D,QVector3D>(QVector3D(sideSize, sideSize, sideSize),color));
        vertexesColors.push_back(std::pair<QVector3D,QVector3D>(QVector3D(-sideSize, sideSize,-sideSize),color));
        vertexesColors.push_back(std::pair<QVector3D,QVector3D>(QVector3D(-sideSize, sideSize, sideSize),color));
        vertexesColors.push_back(std::pair<QVector3D,QVector3D>(QVector3D(sideSize, sideSize, sideSize),color));
        vertexesColors.push_back(std::pair<QVector3D,QVector3D>(QVector3D(-sideSize, sideSize, sideSize),color));
        vertexesColors.push_back(std::pair<QVector3D,QVector3D>(QVector3D(sideSize,-sideSize, sideSize),color));
    }
    m_data.resize(6*vertexesColors.size());
    m_vbo=new QOpenGLBuffer();
    m_count=0;
    for (auto element : vertexesColors){
        add(element.first, element.second);
    }
}

void ObjectToDraw::add(const QVector3D &pos, const QVector3D &color)
{
    GLfloat *p = m_data.data() + m_count;
    *p++ = pos.x();
    *p++ = pos.y();
    *p++ = pos.z();
    *p++ = color.x();
    *p++ = color.y();
    *p++ = color.z();
    m_count += 6;
}
