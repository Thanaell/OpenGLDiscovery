#include "glview.h"
#include <QMouseEvent>
#include <QOpenGLShaderProgram>
#include <QCoreApplication>
#include <math.h>



GLView::GLView(std::unique_ptr<Game> game,QWidget *parent)
    : QOpenGLWidget(parent)

{
    m_game=std::move(game);
    setFocusPolicy(Qt::FocusPolicy::StrongFocus);
    for (int i =0; i<m_game->getGridHeight(); i++){
        for (int j=0; j<m_game->getGridWidth(); j++){
            m_objects[{i,j}]=GLSquare();
            m_objects[{i,j}].translateModelMatrix(QVector3D(j,i,0));
        }
    }
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
    "in vec3 vertex;\n"
    "uniform mat4 model;\n"
    "uniform mat4 view;\n"
    "uniform mat4 proj;\n"
    "void main()\n"
    "{\n"
    "   gl_Position = proj*view*model*vec4(vertex.xyz,1.0);\n"
    "}\n";

static const char *fragmentShaderSource =
    "#version 150\n"
    "out vec4 FragColor;\n"
    "uniform vec3 color;\n"
    "void main() {\n"
    "   FragColor = vec4(color,1);\n"
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

    m_program->link();
    m_modelMatLoc = m_program->uniformLocation("model");
    m_viewMatLoc = m_program->uniformLocation("view");
    m_projMatLoc = m_program->uniformLocation("proj");
    m_colorLoc=m_program->uniformLocation("color");

    m_program->bind();

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
    f->glVertexAttribPointer(1,3, GL_FLOAT, GL_FALSE, 3*sizeof (GLfloat), nullptr);
    square.getVBO()->release();
}

void GLView::paintGL()
{
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    glEnable(GL_DEPTH_TEST);
    glEnable(GL_CULL_FACE);

    for (auto &vao : m_vaos){

        QOpenGLVertexArrayObject::Binder vaoBinder(&vao.second);

        QVector3D color;
        switch (m_game->getGrid()[vao.first].first){
        case EMPTY:
            color=QVector3D(0.2,0.2,0.2);
            break;
        case T:
            color=QVector3D(1,0,1);
            break;
        case S:
            color=QVector3D(1,1,0);
            break;
        case L:
            color=QVector3D(0,1,1);
            break;
        case Z:
            color=QVector3D(1,0,0);
            break;
        case SQ:
            color=QVector3D(0,1,0);
            break;
        case IL:
            color=QVector3D(0,0,1);
            break;
        case Bar:
            color=QVector3D(1,1,1);
            break;
        }

        QMatrix4x4 modelMatrix;
        modelMatrix=m_objects[vao.first].getModelMatrix();
        modelMatrix.scale(1);

        QMatrix4x4 viewMatrix;

        QMatrix4x4 projMatrix=QMatrix4x4();
        projMatrix.setToIdentity();
        projMatrix.ortho(-5,15,-3,51,-10,20);

        m_program->bind();
        m_program->setUniformValue(m_modelMatLoc, modelMatrix);
        m_program->setUniformValue(m_viewMatLoc, viewMatrix);
        m_program->setUniformValue(m_projMatLoc, projMatrix);
        m_program->setUniformValue(m_colorLoc,color);
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
