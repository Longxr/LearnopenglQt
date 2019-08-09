#include "QtFunctionWidget.h"
#include <QDebug>
#include <QTimer>

QtFunctionWidget::QtFunctionWidget(QWidget *parent) : QOpenGLWidget (parent),
    vbo(QOpenGLBuffer::VertexBuffer)
{
    m_pTimer = new QTimer(this);
    connect(m_pTimer, &QTimer::timeout, this, [=]{
        m_nTimeValue += 1;
        update();
    });
    m_pTimer->start(40);
}

QtFunctionWidget::~QtFunctionWidget(){
    makeCurrent();

    vbo.destroy();
    vao.destroy();

    delete texture1;
    delete texture2;

    doneCurrent();
}

void QtFunctionWidget::initializeGL(){
    this->initializeOpenGLFunctions();

    bool success = shaderProgram.addShaderFromSourceFile(QOpenGLShader::Vertex, ":/textures.vert");
    if (!success) {
        qDebug() << "shaderProgram addShaderFromSourceFile failed!" << shaderProgram.log();
        return;
    }

    success = shaderProgram.addShaderFromSourceFile(QOpenGLShader::Fragment, ":/textures.frag");
    if (!success) {
        qDebug() << "shaderProgram addShaderFromSourceFile failed!" << shaderProgram.log();
        return;
    }

    success = shaderProgram.link();
    if(!success) {
        qDebug() << "shaderProgram link failed!" << shaderProgram.log();
    }

    // configure global opengl state
    // -----------------------------
    glEnable(GL_DEPTH_TEST);

    //VAO，VBO data
    float vertices[] = {
        -0.5f, -0.5f, -0.5f,  0.0f, 0.0f,
         0.5f, -0.5f, -0.5f,  1.0f, 0.0f,
         0.5f,  0.5f, -0.5f,  1.0f, 1.0f,
         0.5f,  0.5f, -0.5f,  1.0f, 1.0f,
        -0.5f,  0.5f, -0.5f,  0.0f, 1.0f,
        -0.5f, -0.5f, -0.5f,  0.0f, 0.0f,

        -0.5f, -0.5f,  0.5f,  0.0f, 0.0f,
         0.5f, -0.5f,  0.5f,  1.0f, 0.0f,
         0.5f,  0.5f,  0.5f,  1.0f, 1.0f,
         0.5f,  0.5f,  0.5f,  1.0f, 1.0f,
        -0.5f,  0.5f,  0.5f,  0.0f, 1.0f,
        -0.5f, -0.5f,  0.5f,  0.0f, 0.0f,

        -0.5f,  0.5f,  0.5f,  1.0f, 0.0f,
        -0.5f,  0.5f, -0.5f,  1.0f, 1.0f,
        -0.5f, -0.5f, -0.5f,  0.0f, 1.0f,
        -0.5f, -0.5f, -0.5f,  0.0f, 1.0f,
        -0.5f, -0.5f,  0.5f,  0.0f, 0.0f,
        -0.5f,  0.5f,  0.5f,  1.0f, 0.0f,

         0.5f,  0.5f,  0.5f,  1.0f, 0.0f,
         0.5f,  0.5f, -0.5f,  1.0f, 1.0f,
         0.5f, -0.5f, -0.5f,  0.0f, 1.0f,
         0.5f, -0.5f, -0.5f,  0.0f, 1.0f,
         0.5f, -0.5f,  0.5f,  0.0f, 0.0f,
         0.5f,  0.5f,  0.5f,  1.0f, 0.0f,

        -0.5f, -0.5f, -0.5f,  0.0f, 1.0f,
         0.5f, -0.5f, -0.5f,  1.0f, 1.0f,
         0.5f, -0.5f,  0.5f,  1.0f, 0.0f,
         0.5f, -0.5f,  0.5f,  1.0f, 0.0f,
        -0.5f, -0.5f,  0.5f,  0.0f, 0.0f,
        -0.5f, -0.5f, -0.5f,  0.0f, 1.0f,

        -0.5f,  0.5f, -0.5f,  0.0f, 1.0f,
         0.5f,  0.5f, -0.5f,  1.0f, 1.0f,
         0.5f,  0.5f,  0.5f,  1.0f, 0.0f,
         0.5f,  0.5f,  0.5f,  1.0f, 0.0f,
        -0.5f,  0.5f,  0.5f,  0.0f, 0.0f,
        -0.5f,  0.5f, -0.5f,  0.0f, 1.0f
    };

    QOpenGLVertexArrayObject::Binder vaoBind(&vao);

    vbo.create();
    vbo.bind();
    vbo.allocate(vertices, sizeof(vertices));

    // position attribute
    int attr = -1;
    attr = shaderProgram.attributeLocation("aPos");
    shaderProgram.setAttributeBuffer(attr, GL_FLOAT, 0, 3, sizeof(GLfloat) * 5);
    shaderProgram.enableAttributeArray(attr);
    // texture coord attribute
    attr = shaderProgram.attributeLocation("aTexCoord");
    shaderProgram.setAttributeBuffer(attr, GL_FLOAT, sizeof(GLfloat) * 3, 2, sizeof(GLfloat) * 5);
    shaderProgram.enableAttributeArray(attr);

    // texture 1
    // ---------
    texture1 = new QOpenGLTexture(QImage(":/container.jpg"), QOpenGLTexture::GenerateMipMaps);
    if(!texture1->isCreated()){
        qDebug() << "Failed to load texture";
    }
    // set the texture wrapping parameters
    texture1->setWrapMode(QOpenGLTexture::DirectionS, QOpenGLTexture::Repeat);
    texture1->setWrapMode(QOpenGLTexture::DirectionT, QOpenGLTexture::Repeat);
    // set texture filtering parameters
    texture1->setMinificationFilter(QOpenGLTexture::Linear);
    texture1->setMagnificationFilter(QOpenGLTexture::Linear);

    // texture 2
    // ---------
    texture2 = new QOpenGLTexture(QImage(":/awesomeface.png").mirrored(true, true), QOpenGLTexture::GenerateMipMaps);
    if(!texture2->isCreated()){
        qDebug() << "Failed to load texture";
    }
    // set the texture wrapping parameters
    texture2->setWrapMode(QOpenGLTexture::DirectionS, QOpenGLTexture::Repeat);
    texture2->setWrapMode(QOpenGLTexture::DirectionT, QOpenGLTexture::Repeat);
    // set texture filtering parameters
    texture2->setMinificationFilter(QOpenGLTexture::Linear);
    texture1->setMagnificationFilter(QOpenGLTexture::Linear);

    // tell opengl for each sampler to which texture unit it belongs to (only has to be done once)
    shaderProgram.bind();   // don't forget to activate/use the shader before setting uniforms!
    shaderProgram.setUniformValue("texture1", 0);
    shaderProgram.setUniformValue("texture2", 1);
    // note: currently we set the projection matrix each frame, but since the projection matrix rarely changes it's often best practice to set it outside the main loop only once.
    QMatrix4x4 view;
    view.translate(QVector3D(0.0f, 0.0f, -3.0f));
    shaderProgram.setUniformValue("view", view);
    QMatrix4x4 projection;
    projection.perspective(45.0f, 1.0f * width() / height(), 0.1f, 100.0f);
    shaderProgram.setUniformValue("projection", projection);
    shaderProgram.release();

    vbo.release();
}

void QtFunctionWidget::resizeGL(int w, int h){
    glViewport(0, 0, w, h);
}

static QVector3D cubePositions[] = {
  QVector3D( 0.0f,  0.0f,  0.0f),
  QVector3D( 2.0f,  5.0f, -15.0f),
  QVector3D(-1.5f, -2.2f, -2.5f),
  QVector3D(-3.8f, -2.0f, -12.3f),
  QVector3D( 2.4f, -0.4f, -3.5f),
  QVector3D(-1.7f,  3.0f, -7.5f),
  QVector3D( 1.3f, -2.0f, -2.5f),
  QVector3D( 1.5f,  2.0f, -2.5f),
  QVector3D( 1.5f,  0.2f, -1.5f),
  QVector3D(-1.3f,  1.0f, -1.5f)
};

void QtFunctionWidget::paintGL(){
    glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT); // also clear the depth buffer now!

    // bind textures on corresponding texture units
    glActiveTexture(GL_TEXTURE0);
    texture1->bind();
    glActiveTexture(GL_TEXTURE1);
    texture2->bind();

    shaderProgram.bind();

    {// render box
        QOpenGLVertexArrayObject::Binder vaoBind(&vao);

        for (unsigned int i = 0; i < 10; i++) {
           // calculate the model matrix for each object and pass it to shader before drawing
           QMatrix4x4 model;
           model.translate(cubePositions[i]);
           float angle = (i + 1.0f) * m_nTimeValue;
           model.rotate(angle, QVector3D(1.0f, 0.3f, 0.5f));
           shaderProgram.setUniformValue("model", model);
           glDrawArrays(GL_TRIANGLES, 0, 36);
        }
    }

    texture1->release();
    texture2->release();
    shaderProgram.release();
}
