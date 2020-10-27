#ifndef GLVIEW_H
#define GLVIEW_H

#include <memory>
#include <QOpenGLFunctions>
#include <QOpenGLWidget>
#include <QOpenGLVertexArrayObject>

#include "GLObject/globject.h"
#include "game.h"

class QOpenGLTexture;
class QKeyEvent;

QT_FORWARD_DECLARE_CLASS(QOpenGLShaderProgram)

class GLView : public QOpenGLWidget, protected QOpenGLFunctions
{
    Q_OBJECT

public:
    GLView(std::shared_ptr<Game> game,QWidget *parent = nullptr);
    ~GLView();

    static bool isTransparent() { return m_transparent; }
    static void setTransparent(bool t) { m_transparent = t; }

    QSize minimumSizeHint() const override;
    QSize sizeHint() const override;

    void keyPressEvent(QKeyEvent * event) override;

public slots:
    void cleanup();

private slots:
    void mUpdateScoreArea();
    void mTimeOut();

protected:
    void initializeGL() override;
    void paintGL() override;
    void resizeGL(int width, int height) override;

private:
    std::shared_ptr<Game> m_game;

    bool m_core;
    static bool m_transparent;

    void setupVertexAttribs(std::shared_ptr<GLObject>);

    //Current VAO
    QOpenGLVertexArrayObject m_vao;

    //All possible  objects
    std::shared_ptr<GLObject> m_bg;
    std::shared_ptr<GLObject> m_bgUpcoming;
    std::map<std::pair<int,int>,std::shared_ptr<GLObject>> m_objects;
    std::map<std::pair<int,int>,std::shared_ptr<GLObject>> m_upcomingObjects;
    std::shared_ptr<GLObject> m_scoreArea;

    //All possible VAOS
    QOpenGLVertexArrayObject m_bgVao;
    QOpenGLVertexArrayObject m_bgUpcomingVao;
    std::map<std::pair<int,int>, QOpenGLVertexArrayObject> m_vaos;
    std::map<std::pair<int,int>, QOpenGLVertexArrayObject> m_upcomingVaos;
    QOpenGLVertexArrayObject m_scoreAreaVao;

    //All possible images used for generating textures
    std::map<ShapeType,QImage*> m_imagesPerShapeType;
    QImage * m_bgImage;
    QImage * m_bgUpcomingImage;
    QImage * m_scoreAreaImage;

    //All possible textures
    std::map<int,QOpenGLTexture*> m_objectsTextures;
    QOpenGLTexture * m_bgTexture;
    QOpenGLTexture * m_bgUpcomingTexture;
    QOpenGLTexture * m_scoreAreaTexture;

    //Shader program
    QOpenGLShaderProgram * m_program = nullptr;

    //Locs for loading uniforms into GLSL
    int m_modelMatLoc;
    int m_viewMatLoc;
    int m_projMatLoc;
    int m_colorLoc;
    int m_texLoc;

    //Variables to handle update with time
    QTimer m_timer;
    int m_fps;

};

#endif
