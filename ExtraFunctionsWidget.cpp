#include "ExtraFunctionsWidget.h"
#include <QDebug>
#include <QFile>

static GLuint VBO, VAO, EBO;

ExtraFunctionsWidget::ExtraFunctionsWidget(QWidget *parent) : QOpenGLWidget(parent)
{

}

ExtraFunctionsWidget::~ExtraFunctionsWidget()
{
    glDeleteVertexArrays(1, &VAO);
    glDeleteBuffers(1, &VBO);
//    glDeleteBuffers(1, &EBO);
}

void ExtraFunctionsWidget::initializeGL(){
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
    delete pVertShader;
    delete pFragShader;

    //VAO，VBO数据部分
    float vertices[] = {
        0.5f, -0.5f, 0.0f,  1.0f, 0.0f, 0.0f,  // Bottom Right
        -0.5f, -0.5f, 0.0f,  0.0f, 1.0f, 0.0f,  // Bottom Left
        0.0f,  0.5f, 0.0f,  0.0f, 0.0f, 1.0f   // Top
    };

    glGenVertexArrays(1, &VAO);
    glGenBuffers(1, &VBO);

    // bind the Vertex Array Object first, then bind and set vertex buffer(s), and then configure vertex attributes(s).
    glBindVertexArray(VAO);

    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);  //顶点数据复制到缓冲



    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(GLfloat), (void*)0);
    glEnableVertexAttribArray(0);
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(GLfloat), (GLvoid*)(3*sizeof(GLfloat)));
    glEnableVertexAttribArray(1);
    glBindBuffer(GL_ARRAY_BUFFER, 0);
    glBindVertexArray(0);

//    You can unbind the VAO afterwards so other VAO calls won't accidentally modify this VAO, but this rarely happens. Modifying other
//    VAOs requires a call to glBindVertexArray anyways so we generally don't unbind VAOs (nor VBOs) when it's not directly necessary.
    glBindVertexArray(0);   //取消VAO绑定

    //线框模式，QOpenGLExtraFunctions没这函数, 3_3_Core有
//    glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
}

void ExtraFunctionsWidget::resizeGL(int w, int h){
    glViewport(0, 0, w, h);
}

void ExtraFunctionsWidget::paintGL(){
    glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
    glClear(GL_COLOR_BUFFER_BIT);

    shaderProgram.bind();
    glBindVertexArray(VAO); // seeing as we only have a single VAO there's no need to bind it every time, but we'll do so to keep things a bit more organized
    glDrawArrays(GL_TRIANGLES, 0, 3);
//    glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);

    shaderProgram.release();
}

QOpenGLShader *ExtraFunctionsWidget::CreateShader(const QString &fileName, QOpenGLShader::ShaderTypeBit type)
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
