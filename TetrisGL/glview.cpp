#include "glview.h"
#include <QMouseEvent>
#include <QOpenGLShaderProgram>
#include <QCoreApplication>
#include <math.h>
#include <QOpenGLTexture>



GLView::GLView(std::unique_ptr<Game> game,QWidget *parent)
    : QOpenGLWidget(parent), m_bg(nullptr)

{
    m_game=std::move(game);
    setFocusPolicy(Qt::FocusPolicy::StrongFocus);
    for (int x =0; x<m_game->getGridWidth(); x++){
        for (int y=0; y<m_game->getGridHeight(); y++){
            m_objects[{x,y}]=std::make_shared<GLSquare>();
            m_objects[{x,y}]->translateModelMatrix(QVector3D(x,y,0));
        }
    }

    for (int x =0; x<m_game->getUpcomingGridWidth(); x++){
        for (int y=0; y<m_game->getUpcomingGridHeight(); y++){
            m_upcomingObjects[{x,y}]=std::make_shared<GLSquare>();
            m_upcomingObjects[{x,y}]->translateModelMatrix(QVector3D(x+14,y+4,0));
        }
    }

    m_imagesPerShapeType[ShapeType::EMPTY]=new QImage(50,50,QImage::Format_ARGB32 );
    m_imagesPerShapeType[ShapeType::EMPTY]->fill(qRgba(0,0,0,0));
    m_imagesPerShapeType[ShapeType::L]=new QImage(QString(":images/red.png"));
    m_imagesPerShapeType[ShapeType::Z]=new QImage(QString(":images/green.png"));
    m_imagesPerShapeType[ShapeType::S]=new QImage(QString(":images/blue.png"));
    m_imagesPerShapeType[ShapeType::IL]=new QImage(QString(":images/purple.png"));
    m_imagesPerShapeType[ShapeType::T]=new QImage(QString(":images/yellow.png"));
    m_imagesPerShapeType[ShapeType::Bar]=new QImage(QString(":images/cyan.png"));
    m_imagesPerShapeType[ShapeType::SQ]=new QImage(QString(":images/darkgreen.png"));

    m_bg=std::make_shared<GLBackgroundRectangle>(static_cast<float>(m_game->getGridWidth())/static_cast<float>(m_game->getGridHeight()));
    m_bg->translateModelMatrix(QVector3D(0,0,-1));
    m_bg->scale(20);
    m_bgImage=new QImage(QString(":images/background.jpg"));

    m_bgUpcoming=std::make_shared<GLBackgroundRectangle>(0.5);
    m_bgUpcoming->translateModelMatrix(QVector3D(12,4,-1));
    m_bgUpcoming->scale(15);
    m_bgUpcomingImage=new QImage(QString(":images/background.jpg"));

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
    return QSize(600, 1000);
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
    "   vec4 test = texture2D(myTex,texC);\n"
    "   if(test.w==0){\n"
    "       discard;\n"
    "   } \n"
    "   FragColor =test;\n"

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

    m_bgVao.create();
    QOpenGLVertexArrayObject::Binder vaoBinder(&m_bgVao);
    m_bg->getVBO()->create();
    m_bg->getVBO()->bind();
    m_bg->getVBO()->allocate( m_bg->const_data(),m_bg->count()*sizeof (GLfloat));
    setupVertexAttribs(m_bg);

    m_bgUpcomingVao.create();
    QOpenGLVertexArrayObject::Binder vaoBinder2(&m_bgUpcomingVao);
    //vaoBinder(&m_bgUpcomingVao);
    m_bgUpcoming->getVBO()->create();
    m_bgUpcoming->getVBO()->bind();
    m_bgUpcoming->getVBO()->allocate( m_bgUpcoming->const_data(),m_bgUpcoming->count()*sizeof (GLfloat));
    setupVertexAttribs(m_bgUpcoming);

    for (auto object : m_objects){
        m_vaos[object.first].create();
        QOpenGLVertexArrayObject::Binder vaoBinder(&m_vaos[object.first]);
        object.second->getVBO()->create();
        object.second->getVBO()->bind();
        object.second->getVBO()->allocate(object.second->const_data(),object.second->count()*sizeof (GLfloat));
        setupVertexAttribs(object.second);
    }

    for (auto object : m_upcomingObjects){
        m_upcomingVaos[object.first].create();
        QOpenGLVertexArrayObject::Binder vaoBinder(&m_upcomingVaos[object.first]);
        object.second->getVBO()->create();
        object.second->getVBO()->bind();
        object.second->getVBO()->allocate(object.second->const_data(),object.second->count()*sizeof (GLfloat));
        setupVertexAttribs(object.second);
    }

    for (auto image : m_imagesPerShapeType){
        m_objectsTextures[image.first]=new QOpenGLTexture(*image.second);
    }
    m_bgTexture=new QOpenGLTexture(*m_bgImage);
    m_bgUpcomingTexture=new QOpenGLTexture(*m_bgUpcomingImage);

    m_program->release();
    int fps=100;
    m_timer.start(1000/fps);
    QObject::connect(&m_timer, &QTimer::timeout, this, &GLView::mTimeOut);
}

void GLView::setupVertexAttribs(std::shared_ptr<GLObject> object)
{
    object->getVBO()->bind();
    QOpenGLFunctions *f = QOpenGLContext::currentContext()->functions();
    f->glEnableVertexAttribArray(1);
    f->glVertexAttribPointer(1,3, GL_FLOAT, GL_FALSE, 5*sizeof (GLfloat), nullptr);

    f->glEnableVertexAttribArray(2);
    f->glVertexAttribPointer(2,2, GL_FLOAT, GL_FALSE, 5*sizeof (GLfloat), (void*)(3*sizeof(GLfloat)));

    object->getVBO()->release();
}

void GLView::paintGL()
{
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    glEnable(GL_DEPTH_TEST);
    glEnable(GL_CULL_FACE);

    QMatrix4x4 viewMatrix;

    QMatrix4x4 projMatrix=QMatrix4x4();
    projMatrix.setToIdentity();
    projMatrix.ortho(-1,m_game->getGridWidth()+12,-1,m_game->getGridHeight()+1,-10,100);
    //projMatrix.ortho(-100,100,-50,100,-100,100);

    for (auto &vao : m_vaos){
        QOpenGLVertexArrayObject::Binder vaoBinder(&vao.second);
        QOpenGLTexture *texture=m_objectsTextures[m_game->getGrid()[vao.first].first];
        QMatrix4x4 modelMatrix;
        modelMatrix=m_objects[vao.first]->getModelMatrix();
        modelMatrix.scale(1);
        m_program->bind();
        m_program->setUniformValue(m_modelMatLoc, modelMatrix);
        m_program->setUniformValue(m_viewMatLoc, viewMatrix);
        m_program->setUniformValue(m_projMatLoc, projMatrix);
        texture->bind();
        glDrawArrays(GL_TRIANGLES,0,6);
    }

    for (auto &vao : m_upcomingVaos){
        QOpenGLVertexArrayObject::Binder vaoBinder(&vao.second);
        QOpenGLTexture *texture=m_objectsTextures[m_game->getUpcomingGrid()[vao.first]];
        QMatrix4x4 modelMatrix;
        modelMatrix=m_upcomingObjects[vao.first]->getModelMatrix();
        modelMatrix.scale(1);
        m_program->bind();
        m_program->setUniformValue(m_modelMatLoc, modelMatrix);
        m_program->setUniformValue(m_viewMatLoc, viewMatrix);
        m_program->setUniformValue(m_projMatLoc, projMatrix);

        texture->bind();
        glDrawArrays(GL_TRIANGLES,0,6);
    }

    QOpenGLVertexArrayObject::Binder vaoBinder(&m_bgVao);
    QOpenGLTexture *texture=m_bgTexture;
    QMatrix4x4 modelMatrix;
    modelMatrix=m_bg->getModelMatrix();
    m_program->bind();
    m_program->setUniformValue(m_modelMatLoc, modelMatrix);
    m_program->setUniformValue(m_viewMatLoc, viewMatrix);
    m_program->setUniformValue(m_projMatLoc, projMatrix);
    texture->bind();
    glDrawArrays(GL_TRIANGLES,0,6);

    QOpenGLVertexArrayObject::Binder vaoBinder2(&m_bgUpcomingVao);
    QOpenGLTexture *texture2=m_bgUpcomingTexture;
    QMatrix4x4 modelMatrix2;
    modelMatrix2=m_bgUpcoming->getModelMatrix();
    m_program->bind();
    m_program->setUniformValue(m_modelMatLoc, modelMatrix2);
    m_program->setUniformValue(m_viewMatLoc, viewMatrix);
    m_program->setUniformValue(m_projMatLoc, projMatrix);
    texture2->bind();
    glDrawArrays(GL_TRIANGLES,0,6);

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
