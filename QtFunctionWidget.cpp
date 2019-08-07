#include "QtFunctionWidget.h"
#include <QDebug>
#include <QTimer>

QtFunctionWidget::QtFunctionWidget(QWidget *parent) : QOpenGLWidget (parent),
    vbo(QOpenGLBuffer::VertexBuffer),
    ebo(QOpenGLBuffer::IndexBuffer)
{
//    m_pTimer = new QTimer(this);
//    m_pTimer->setInterval(200);

//    connect(m_pTimer, &QTimer::timeout, this, [=]{
//        m_uniformValue += 0.1f;

//        if (m_uniformValue > 1.5f) {
//            m_uniformValue = -1.5f;
//        }

//        update();
//    });

//    m_pTimer->start();
}

QtFunctionWidget::~QtFunctionWidget(){
    makeCurrent();

    vbo.destroy();
    ebo.destroy();
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

    //VAO，VBO data
    float vertices[] = {
        // positions          // colors           // texture coords
         0.5f,  0.5f, 0.0f,   1.0f, 0.0f, 0.0f,   1.0f, 1.0f, // top right
         0.5f, -0.5f, 0.0f,   0.0f, 1.0f, 0.0f,   1.0f, 0.0f, // bottom right
        -0.5f, -0.5f, 0.0f,   0.0f, 0.0f, 1.0f,   0.0f, 0.0f, // bottom left
        -0.5f,  0.5f, 0.0f,   1.0f, 1.0f, 0.0f,   0.0f, 1.0f  // top left
    };
    unsigned int indices[] = {  // note that we start from 0!
        0, 1, 3,  // first Triangle
        1, 2, 3   // second Triangle
    };

    QOpenGLVertexArrayObject::Binder vaoBind(&vao);

    vbo.create();
    vbo.bind();
    vbo.allocate(vertices, sizeof(vertices));

    ebo.create();
    ebo.bind();
    ebo.allocate(indices, sizeof(indices));

    // position attribute
    int attr = -1;
    attr = shaderProgram.attributeLocation("aPos");
    shaderProgram.setAttributeBuffer(attr, GL_FLOAT, 0, 3, sizeof(GLfloat) * 8);
    shaderProgram.enableAttributeArray(attr);
    // color attribute
    attr = shaderProgram.attributeLocation("aColor");
    shaderProgram.setAttributeBuffer(attr, GL_FLOAT, sizeof(GLfloat) * 3, 3, sizeof(GLfloat) * 8);
    shaderProgram.enableAttributeArray(attr);
    // texture coord attribute
    attr = shaderProgram.attributeLocation("aTexCoord");
    shaderProgram.setAttributeBuffer(attr, GL_FLOAT, sizeof(GLfloat) * 6, 2, sizeof(GLfloat) * 8);
    shaderProgram.enableAttributeArray(attr);

    // texture 1
    // ---------
    texture1 = new QOpenGLTexture(QImage(":/container.jpg"), QOpenGLTexture::GenerateMipMaps); //直接生成绑定一个2d纹理, 并生成多级纹理MipMaps
    if(!texture1->isCreated()){
        qDebug() << "Failed to load texture";
    }
    // set the texture wrapping parameters
    texture1->setWrapMode(QOpenGLTexture::DirectionS, QOpenGLTexture::Repeat);// 等于glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
    texture1->setWrapMode(QOpenGLTexture::DirectionT, QOpenGLTexture::Repeat);//    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
    // set texture filtering parameters
    texture1->setMinificationFilter(QOpenGLTexture::Linear);   //等价于glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    texture1->setMagnificationFilter(QOpenGLTexture::Linear);

    // texture 2
    // ---------
    texture2 = new QOpenGLTexture(QImage(":/awesomeface.png").mirrored(true, true), QOpenGLTexture::GenerateMipMaps); //直接生成绑定一个2d纹理, 并生成多级纹理MipMaps
    if(!texture2->isCreated()){
        qDebug() << "Failed to load texture";
    }
    // set the texture wrapping parameters
    texture2->setWrapMode(QOpenGLTexture::DirectionS, QOpenGLTexture::Repeat);// 等于glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
    texture2->setWrapMode(QOpenGLTexture::DirectionT, QOpenGLTexture::Repeat);//    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
    // set texture filtering parameters
    texture2->setMinificationFilter(QOpenGLTexture::Linear);   //等价于glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    texture1->setMagnificationFilter(QOpenGLTexture::Linear);

    // tell opengl for each sampler to which texture unit it belongs to (only has to be done once)
    shaderProgram.bind();   // don't forget to activate/use the shader before setting uniforms!
    shaderProgram.setUniformValue("texture1", 0);
    shaderProgram.setUniformValue("texture2", 1);
    shaderProgram.release();

    vbo.release();
//    remember: do NOT unbind the EBO while a VAO is active as the bound element buffer object IS stored in the VAO; keep the EBO bound.
//    ebo.release();

}

void QtFunctionWidget::resizeGL(int w, int h){
    glViewport(0, 0, w, h);
}

void QtFunctionWidget::paintGL(){
    glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
    glClear(GL_COLOR_BUFFER_BIT);

    shaderProgram.bind();
    {
        glActiveTexture(GL_TEXTURE0);
        texture1->bind();
//        glBindTexture(GL_TEXTURE_2D, texture1->textureId());
        glActiveTexture(GL_TEXTURE1);
        texture2->bind();
//        glBindTexture(GL_TEXTURE_2D, texture2->textureId());

        // render container
        QOpenGLVertexArrayObject::Binder vaoBind(&vao);
        glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, nullptr);

        texture1->release();
        texture2->release();
    }
    shaderProgram.release();
}
