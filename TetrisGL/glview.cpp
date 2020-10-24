#include "glview.h"
#include <QMouseEvent>
#include <QOpenGLShaderProgram>
#include <QCoreApplication>
#include <math.h>
#include <QOpenGLTexture>



GLView::GLView(std::unique_ptr<Game> game,QWidget *parent)
    : QOpenGLWidget(parent)

{
    m_game=std::move(game);
    setFocusPolicy(Qt::FocusPolicy::StrongFocus);
    for (int x =0; x<m_game->getGridWidth(); x++){
        for (int y=0; y<m_game->getGridHeight(); y++){
            m_objects[{x,y}]=GLSquare();
            m_objects[{x,y}].translateModelMatrix(QVector3D(x,y,0));
        }
    }
    imagesPerInt[ShapeType::EMPTY]=new QImage(50,50,QImage::Format_RGB16);
    imagesPerInt[ShapeType::EMPTY]->fill(Qt::gray);
    imagesPerInt[ShapeType::L]=new QImage(QString(":images/red.png"));
    imagesPerInt[ShapeType::Z]=new QImage(QString(":images/green.png"));
    imagesPerInt[ShapeType::S]=new QImage(QString(":images/blue.png"));
    imagesPerInt[ShapeType::IL]=new QImage(QString(":images/purple.png"));
    imagesPerInt[ShapeType::T]=new QImage(QString(":images/yellow.png"));
    imagesPerInt[ShapeType::Bar]=new QImage(QString(":images/cyan.png"));
    imagesPerInt[ShapeType::SQ]=new QImage(QString(":images/orange.png"));

}

GLView::~GLView()
{
    cleanup();
}

QSize GLView::minimumSizeHint() const
{
    return QSize(50, 50);
}

QSize GLView::sizeHint() const
{
    return QSize(300, 1000);
}

void GLView::cleanup()
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
    "in vec2 texCoords;\n"
    "out vec2 texC;\n"
    "in vec3 vertex;\n"
    "uniform mat4 model;\n"
    "uniform mat4 view;\n"
    "uniform mat4 proj;\n"
    "void main()\n"
    "{\n"
    "   gl_Position = proj*view*model*vec4(vertex.xyz,1.0);\n"
    "   texC=texCoords;\n"
    "}\n";

static const char *fragmentShaderSource =
    "#version 150\n"
    "uniform sampler2D myTex;\n"
    "out vec4 FragColor;\n"
    "in vec2 texC;\n"
    "void main() {\n"
    "   FragColor = texture2D(myTex,texC);\n"
    "}\n";

void GLView::initializeGL()
{
    connect(context(), &QOpenGLContext::aboutToBeDestroyed, this, &GLView::cleanup);

    initializeOpenGLFunctions();
    glClearColor(0, 0, 0, 1);

    m_program = new QOpenGLShaderProgram;
    m_program->addShaderFromSourceCode(QOpenGLShader::Vertex,vertexShaderSource);
    m_program->addShaderFromSourceCode(QOpenGLShader::Fragment,fragmentShaderSource);
    m_program->bindAttributeLocation("vertex", 1);
    m_program->bindAttributeLocation("texCoords",2);

    m_program->link();
    m_modelMatLoc = m_program->uniformLocation("model");
    m_viewMatLoc = m_program->uniformLocation("view");
    m_projMatLoc = m_program->uniformLocation("proj");
    m_texLoc=m_program->uniformLocation("myTex");

    m_program->bind();
    m_program->setUniformValue(m_texLoc,0);

    for (auto object : m_objects){
        m_vaos[object.first].create();
        QOpenGLVertexArrayObject::Binder vaoBinder(&m_vaos[object.first]);
        object.second.getVBO()->create();
        object.second.getVBO()->bind();
        object.second.getVBO()->allocate(object.second.const_data(),object.second.count()*sizeof (GLfloat));
        setupVertexAttribs(object.second);
    }
    m_program->release();
    int fps=100;
    m_timer.start(1000/fps);
    QObject::connect(&m_timer, &QTimer::timeout, this, &GLView::mTimeOut);
}

void GLView::setupVertexAttribs(GLSquare square)
{
    square.getVBO()->bind();
    QOpenGLFunctions *f = QOpenGLContext::currentContext()->functions();
    f->glEnableVertexAttribArray(1);
    f->glVertexAttribPointer(1,3, GL_FLOAT, GL_FALSE, 5*sizeof (GLfloat), nullptr);

    f->glEnableVertexAttribArray(2);
    f->glVertexAttribPointer(2,2, GL_FLOAT, GL_FALSE, 5*sizeof (GLfloat), (void*)(3*sizeof(GLfloat)));

    square.getVBO()->release();
}

void GLView::paintGL()
{
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    glEnable(GL_DEPTH_TEST);
    glEnable(GL_CULL_FACE);

    for (auto &vao : m_vaos){

        QOpenGLVertexArrayObject::Binder vaoBinder(&vao.second);

        QImage *image=imagesPerInt[m_game->getGrid()[vao.first].first];
        QOpenGLTexture *texture=new QOpenGLTexture(*image);

        QMatrix4x4 modelMatrix;
        modelMatrix=m_objects[vao.first].getModelMatrix();
        modelMatrix.scale(1);

        QMatrix4x4 viewMatrix;

        QMatrix4x4 projMatrix=QMatrix4x4();
        projMatrix.setToIdentity();
        projMatrix.ortho(-2,m_game->getGridWidth(),-2,m_game->getGridHeight(),-10,10);

        m_program->bind();
        m_program->setUniformValue(m_modelMatLoc, modelMatrix);
        m_program->setUniformValue(m_viewMatLoc, viewMatrix);
        m_program->setUniformValue(m_projMatLoc, projMatrix);

        texture->bind();
        glDrawArrays(GL_TRIANGLES,0,6);
    }
    m_program->release();
}

void GLView::resizeGL(int width, int height)
{

}

void GLView::mTimeOut(){
    m_elapsed+=1;
    update();
}

void GLView::keyPressEvent(QKeyEvent *event){
    m_game->reactToKey(event->key());
}
