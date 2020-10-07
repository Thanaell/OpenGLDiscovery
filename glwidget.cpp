/****************************************************************************
**
** Copyright (C) 2016 The Qt Company Ltd.
** Contact: https://www.qt.io/licensing/
**
** This file is part of the examples of the Qt Toolkit.
**
** $QT_BEGIN_LICENSE:BSD$
** Commercial License Usage
** Licensees holding valid commercial Qt licenses may use this file in
** accordance with the commercial license agreement provided with the
** Software or, alternatively, in accordance with the terms contained in
** a written agreement between you and The Qt Company. For licensing terms
** and conditions see https://www.qt.io/terms-conditions. For further
** information use the contact form at https://www.qt.io/contact-us.
**
** BSD License Usage
** Alternatively, you may use this file under the terms of the BSD license
** as follows:
**
** "Redistribution and use in source and binary forms, with or without
** modification, are permitted provided that the following conditions are
** met:
**   * Redistributions of source code must retain the above copyright
**     notice, this list of conditions and the following disclaimer.
**   * Redistributions in binary form must reproduce the above copyright
**     notice, this list of conditions and the following disclaimer in
**     the documentation and/or other materials provided with the
**     distribution.
**   * Neither the name of The Qt Company Ltd nor the names of its
**     contributors may be used to endorse or promote products derived
**     from this software without specific prior written permission.
**
**
** THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS
** "AS IS" AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT
** LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR
** A PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT
** OWNER OR CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL,
** SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT
** LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE,
** DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY
** THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT
** (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE
** OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE."
**
** $QT_END_LICENSE$
**
****************************************************************************/

#include "glwidget.h"
#include <QMouseEvent>
#include <QOpenGLShaderProgram>
#include <QCoreApplication>
#include <math.h>

float sign(){
    int result=std::rand();
    float sign=static_cast<float>(result<RAND_MAX/2) ? 1 : -1;
    return sign;
}

float floatRand(){
    auto result=static_cast <float> (rand()) / static_cast <float> (RAND_MAX);
    return result;
}

bool GLWidget::m_transparent = false;

void GLWidget::addObjectFromData(ObjectData data, ObjectType type){
    m_objects.insert({m_nbObjects,std::make_shared<ObjectToDraw>(data,type)});
    m_nbObjects++;
}

void GLWidget::addObject(std::shared_ptr<ObjectToDraw> object){
    m_objects.insert({m_nbObjects,object});
    m_nbObjects++;
}

GLWidget::GLWidget(QWidget *parent)
    : QOpenGLWidget(parent), m_coefLoc(-1), m_modelMatLoc(-1), m_viewMatLoc(-1),m_projMatLoc(-1), m_program(nullptr)
    , m_rotLoc(-1), m_transLoc(-1)
{
    m_elapsed=0;
    m_nbObjects=0;
    m_core = QSurfaceFormat::defaultFormat().profile() == QSurfaceFormat::CoreProfile;
    // --transparent causes the clear color to be transparent. Therefore, on systems that
    // support it, the widget will become transparent apart from the logo.
    if (m_transparent) {
        QSurfaceFormat fmt = format();
        fmt.setAlphaBufferSize(8);
        setFormat(fmt);
    }
    srand (static_cast <unsigned> (time(0)));
    int i=0;
    const int nbCubes=50;
    while (i<nbCubes){

        float size=(floatRand())/5.0f;
        QVector3D color=QVector3D(floatRand(),floatRand(),floatRand());
        auto object=std::make_shared<ObjectToDraw>(ObjectType::CUBE,size, color);
        auto pos=QVector3D(sign()*floatRand(),sign()*floatRand(),sign()*floatRand());
        object->setModelMatrix(pos,rand(),QVector3D(sign()*floatRand(),sign()*floatRand(),sign()*floatRand()));
        object->setRotation(rand()*100/RAND_MAX,QVector3D(floatRand(),floatRand(),floatRand()));
        addObject(object);
        //qDebug()<<object->getModelMatrix();
        i++;
    }
}

GLWidget::~GLWidget()
{
    cleanup();
}

QSize GLWidget::minimumSizeHint() const
{
    return QSize(50, 50);
}

QSize GLWidget::sizeHint() const
{
    return QSize(1000, 1000);
}

void GLWidget::cleanup()
{
    if (m_program == nullptr)
        return;
    makeCurrent();
    delete m_program;
    m_program = nullptr;
    doneCurrent();
}

static const char *vertexShaderSource =
    "#version 150\n"
    "in vec3 vertex;\n"
    "in vec3 color;\n"
    "out vec3 colorF; \n"
    "uniform mat4 model;\n"
    "uniform mat4 view;\n"
    "uniform mat4 proj;\n"
    "uniform float coef;\n" 
    "void main()\n"
    "{\n"
    "   gl_Position = proj*view*model*vec4(vertex.xyz,1.0);\n"
    "   colorF=vec3(color);\n"
    "}\n";

static const char *fragmentShaderSource =
    "#version 150\n"
    "out vec4 FragColor;\n"
    "in vec3 colorF;\n"
    "void main() {\n"
    "   FragColor = vec4(colorF,1);\n"
    "}\n";

void GLWidget::initializeGL()
{
    connect(context(), &QOpenGLContext::aboutToBeDestroyed, this, &GLWidget::cleanup);

    initializeOpenGLFunctions();
    glClearColor(0, 0, 0, m_transparent ? 0 : 1);

    m_program = new QOpenGLShaderProgram;
    m_program->addShaderFromSourceCode(QOpenGLShader::Vertex,vertexShaderSource);
    m_program->addShaderFromSourceCode(QOpenGLShader::Fragment,fragmentShaderSource);
    m_program->bindAttributeLocation("color", 1);
    m_program->bindAttributeLocation("vertex", 2);

    m_program->link();
    m_modelMatLoc = m_program->uniformLocation("model");
    m_viewMatLoc = m_program->uniformLocation("view");
    m_projMatLoc = m_program->uniformLocation("proj");


    m_program->bind();

    for (auto object : m_objects){
        m_vaos[object.first].create();
        QOpenGLVertexArrayObject::Binder vaoBinder(&m_vaos[object.first]);
        object.second.get()->getVBO()->create();
        object.second.get()->getVBO()->bind();
        object.second.get()->getVBO()->allocate(object.second->const_data(),object.second->count()*sizeof(GLfloat)),
        setupVertexAttribs(object.second);
    }
    m_program->release();
    m_timer.start(50);
    QObject::connect(&m_timer, &QTimer::timeout, this, &GLWidget::mTimeOut);
}

void GLWidget::setupVertexAttribs(std::shared_ptr<ObjectToDraw> object)
{
    object.get()->getVBO()->bind();
    QOpenGLFunctions *f = QOpenGLContext::currentContext()->functions();
    f->glEnableVertexAttribArray(2);
    f->glVertexAttribPointer(2,3, GL_FLOAT, GL_FALSE, 6*sizeof (GLfloat), nullptr);
    f->glEnableVertexAttribArray(1);
    f->glVertexAttribPointer(1,3, GL_FLOAT, GL_FALSE, 6*sizeof (GLfloat), (void*)(3*sizeof(GLfloat)));
    object.get()->getVBO()->release();

}

void GLWidget::paintGL()
{
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    glEnable(GL_DEPTH_TEST);
    glEnable(GL_CULL_FACE);

    float coefTrans=0.01;
    float coefRot=0.01;
    for (auto &vao : m_vaos){

        QOpenGLVertexArrayObject::Binder vaoBinder(&vao.second);

        QMatrix4x4 modelMatrix=m_objects[vao.first]->getModelMatrix();
        modelMatrix.translate(coefTrans*m_elapsed*m_objects[vao.first]->getTranslation());
        modelMatrix.rotate(m_objects[vao.first]->getAngle()*coefRot*m_elapsed, m_objects[vao.first]->getRotAxis());

        QMatrix4x4 viewMatrix=QMatrix4x4();
        viewMatrix.setToIdentity();
        viewMatrix.translate(QVector3D(0,0,-4));

        QMatrix4x4 projMatrix=QMatrix4x4();
        projMatrix.setToIdentity();
        projMatrix.perspective(45,1,0.1,100);

        m_program->bind();
        m_program->setUniformValue(m_modelMatLoc, modelMatrix);
        m_program->setUniformValue(m_viewMatLoc, viewMatrix);
        m_program->setUniformValue(m_projMatLoc, projMatrix);
        glDrawArrays(GL_TRIANGLES,0,36);
    }
    m_program->release();
}

void GLWidget::resizeGL(int width, int height)
{

}

void GLWidget::mTimeOut(){
    m_elapsed+=1;
    update();
}
