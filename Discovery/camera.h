#ifndef CAMERA_H
#define CAMERA_H

#include <QObject>
#include <QVector3D>
#include <QMatrix4x4>
#include <math.h>
class Camera : public QObject
{
    Q_OBJECT
public:
    Camera();
    QMatrix4x4 getMatrix();
    float getRadius() const {return m_radius;};
    void moveRight(float mov);
    void moveLeft(float mov);
    void moveUp(float mov);
    void moveDown(float mov);
    void updateCirclePos();
private:
    QVector3D m_target;
    QVector3D m_up;
    QVector3D m_pos;
    const float m_radius;
    const float m_speed;
    float m_theta;
    float m_phi;

};

#endif // CAMERA_H
