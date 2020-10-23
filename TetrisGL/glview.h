#ifndef GLVIEW_H
#define GLVIEW_H

#include <QOpenGLWidget>
#include <QOpenGLFunctions>
#include <QOpenGLVertexArrayObject>
#include <QOpenGLBuffer>
#include <QMatrix4x4>
#include <QTimer>
#include <memory>
#include "game.h"
#include "glsquare.h"

QT_FORWARD_DECLARE_CLASS(QOpenGLShaderProgram)

class GLView : public QOpenGLWidget, protected QOpenGLFunctions
{
    Q_OBJECT

public:
    GLView(std::unique_ptr<Game> game,QWidget *parent = nullptr);
    ~GLView();

    static bool isTransparent() { return m_transparent; }
    static void setTransparent(bool t) { m_transparent = t; }

    QSize minimumSizeHint() const override;
    QSize sizeHint() const override;

    void keyPressEvent(QKeyEvent * event) override;

public slots:
    void cleanup();

private slots:
    void mTimeOut();


signals:

protected:
    void initializeGL() override;
    void paintGL() override;
    void resizeGL(int width, int height) override;

private:
    std::unique_ptr<Game> m_game;
    void setupVertexAttribs(GLSquare);
    bool m_core;
    QOpenGLVertexArrayObject m_vao;
    std::map<std::pair<int,int>, QOpenGLVertexArrayObject> m_vaos;
    QOpenGLShaderProgram * m_program = nullptr;
    static bool m_transparent;
    int m_modelMatLoc;
    int m_viewMatLoc;
    int m_projMatLoc;
    int m_colorLoc;
    QTimer m_timer;
    int m_elapsed;
    std::map<std::pair<int,int>,GLSquare> m_objects;
};

#endif
