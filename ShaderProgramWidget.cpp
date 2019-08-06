#include "ShaderProgramWidget.h"

#include <QFile>

ShaderProgramWidget::ShaderProgramWidget(QWidget *parent) : QOpenGLWidget (parent),
    vbo(QOpenGLBuffer::VertexBuffer)
{

}

ShaderProgramWidget::~ShaderProgramWidget(){
    vbo.destroy();
}

void ShaderProgramWidget::initializeGL(){
    this->initializeOpenGLFunctions();
    //着色器部分
    QOpenGLShader* pVertShader = CreateShader("triangle.vert", QOpenGLShader::Vertex);
    QOpenGLShader* pFragShader = CreateShader("triangle.frag", QOpenGLShader::Fragment);

    if (pVertShader) {
        shaderProgram.addShader(pVertShader);
    } else {
        return;
    }

    if (pFragShader) {
        shaderProgram.addShader(pFragShader);
    } else {
        return;
    }

    bool success = shaderProgram.link();
    if(!success) {
        qDebug() << "ERROR::SHADER::PROGRAM::LINKING_FAILED";
    }

    //VAO，VBO数据部分
    GLfloat vertices[] = {
        0.5f, -0.5f, 0.0f,  1.0f, 0.0f, 0.0f,  // Bottom Right
        -0.5f, -0.5f, 0.0f,  0.0f, 1.0f, 0.0f,  // Bottom Left
        0.0f,  0.5f, 0.0f,  0.0f, 0.0f, 1.0f   // Top
    };

    vbo.create(); //创建buffer
    vbo.bind();
    vbo.allocate(vertices, sizeof(vertices));
//    vbo.allocate(sizeof(vertices));
//    vbo.write(0, vertices, sizeof(vertices));

    int attr = -1;
    attr = shaderProgram.attributeLocation("aPos");
    shaderProgram.setAttributeBuffer(attr, GL_FLOAT, 0, 3, sizeof(GLfloat) * 6);
    shaderProgram.enableAttributeArray(attr);

    attr = shaderProgram.attributeLocation("aColor");
    shaderProgram.setAttributeBuffer(attr, GL_FLOAT, sizeof(GLfloat) * 3, 3, sizeof(GLfloat) * 6);
    shaderProgram.enableAttributeArray(attr);

    vbo.release();
}

void ShaderProgramWidget::resizeGL(int w, int h){
    glViewport(0, 0, w, h);
}

void ShaderProgramWidget::paintGL(){
    glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
    glClear(GL_COLOR_BUFFER_BIT);

    shaderProgram.bind();
    vbo.bind();
    glDrawArrays(GL_TRIANGLES, 0, 3);
    vbo.release();
    shaderProgram.release();
}

QOpenGLShader *ShaderProgramWidget::CreateShader(const QString &fileName, QOpenGLShader::ShaderTypeBit type)
{
    QFile file(QStringLiteral(":/") + fileName);
    if (!file.open(QIODevice::ReadOnly)) {
        qWarning("Can not load shader %s: %s", file.fileName().toUtf8().constData(), file.errorString().toUtf8().constData());
        return nullptr;
    }
    QByteArray src = file.readAll();
    file.close();

    QOpenGLShader *pShader = new QOpenGLShader(type, this);

    bool success = pShader->compileSourceCode(src.constData());
    if(!success){
        qDebug() << "CreateShader compileSourceCode failed!";
        qDebug() << pShader->log();

        delete pShader;
        pShader = nullptr;
    }

    return pShader;
}
