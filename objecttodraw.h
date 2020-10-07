#ifndef OBJECTTODRAW_H
#define OBJECTTODRAW_H

#include <QObject>
#include <QOpenGLBuffer>
#include <QVector3D>
#include <QVector4D>
#include <QMatrix4x4>

enum ObjectType{
    TRIANGLE, CUBE, OTHER
};

struct ObjectData{
    std::vector<std::pair<QVector3D, QVector3D>> vertexesColors;
    QVector3D colorR;
    QVector3D trans;
    float angle;
    QVector3D rotAxis;
};

class ObjectToDraw : public QObject
{
    Q_OBJECT
public:
    ObjectToDraw(ObjectData data, ObjectType type=ObjectType::OTHER, QObject *parent = nullptr);
    ObjectToDraw(ObjectType type, float sideSize, QVector3D color, QObject *parent = nullptr);
    QOpenGLBuffer * getVBO() const {return m_vbo;};
    const GLfloat * const_data(){return m_data.constData();};
    void add(const QVector3D &v, const QVector3D &c);
    int count() const { return m_count; }
    int vertexCount() const { return m_count / 6; }
    QVector4D getColor() const { return m_color;};
    QVector3D getTranslation() const { return m_transPS;};
    float getAngle() const { return m_angle;};
    QVector3D getRotAxis()const {return m_rotAxis;};
    void setColor(QVector4D color){m_color=color;};
    void setTranslation(QVector3D trans){ m_transPS=trans;};
    void setRotation(float angle, QVector3D axis){m_angle=angle; m_rotAxis=axis;};
    void setModelMatrix(QVector3D pos, float angle, QVector3D rotAxis);
    QMatrix4x4 getModelMatrix() const {return m_modelMatrix;};
;
private :
    QOpenGLBuffer * m_vbo;
    ObjectType m_type;
    QVector<GLfloat> m_data;
    int m_count;
    QVector4D m_color;
    QVector3D m_transPS;
    float m_angle;
    QVector3D m_rotAxis;
    QMatrix4x4 m_modelMatrix;

signals:

};

#endif // OBJECTTODRAW_H
