#include "camera.h"

Camera::Camera(): QObject(), m_radius(5), m_speed(0.07)
{
    m_target=QVector3D(0,0,0);
    m_up=QVector3D(0,1,0);
    m_theta=90;
    m_phi=0;
    updateCirclePos();
}
void Camera::updateCirclePos(){
    float camX = sin(m_theta)*cos(m_phi)*m_radius;
    float camY = sin(m_theta)*sin(m_phi)*m_radius;
    float camZ = cos(m_theta)*m_radius;
    m_pos=QVector3D(camX,camY,camZ);
    m_up=(QVector3D(m_radius*cos(m_theta)*cos(m_phi),m_radius*cos(m_theta)*sin(m_phi),-m_radius*sin(m_theta)));
}

void Camera::moveRight(float mov){
    m_phi-=mov*m_speed;
    updateCirclePos();
}

void Camera::moveLeft(float mov){
    m_phi+=mov*m_speed;
    updateCirclePos();
}

void Camera::moveUp(float mov){
    m_theta+=mov*m_speed;
    updateCirclePos();
}

void Camera::moveDown(float mov){
    m_theta-=mov*m_speed;
    updateCirclePos();
}

QMatrix4x4 Camera::getMatrix(){
    QMatrix4x4 view=QMatrix4x4();
    view.setToIdentity();
    view.lookAt(m_pos,m_target,m_up);
    return view;
}
