#include "ShaderProgramWidget.h"

static const char *vertexShaderSource =
        "#version 330 core\n"
        "layout(location = 0) in vec3 aPos;\n"
        "void main(){\n"
        "  gl_Position = vec4(aPos.x, aPos.y, aPos.z, 1.0f);\n"
        "}\n\0";
static const char *fragmentShaderSource =
        "#version 330 core\n"
        "out vec4 FragColor;\n"
        "void main(){\n"
        "   FragColor = vec4(1.0f, 0.5f, 0.2f, 1.0f);\n"
        "}\n\0";

ShaderProgramWidget::ShaderProgramWidget(QWidget *parent) : QOpenGLWidget (parent),
    vbo(QOpenGLBuffer::VertexBuffer)
{

}

ShaderProgramWidget::~ShaderProgramWidget(){
    vbo.destroy();
}

void ShaderProgramWidget::initializeGL(){
    this->initializeOpenGLFunctions();//这个init()函数至关重要，如果继承QOpenGlFunctions,必须使用这个初始化函数
    //着色器部分
    QOpenGLShader vertexShader(QOpenGLShader::Vertex);
    bool success = vertexShader.compileSourceCode(vertexShaderSource);
    if(!success){
        qDebug() << "ERROR::SHADER::VERTEX::COMPILATION_FAILED" << endl;
        return;
    }

    QOpenGLShader fragmentShader(QOpenGLShader::Fragment);
    success = fragmentShader.compileSourceCode(fragmentShaderSource);
    if(!success){
        qDebug() << "ERROR::SHADER::FRAGMENT::COMPILATION_FAILED" << endl;
        return;
    }

    shaderProgram.addShader(&vertexShader);
    shaderProgram.addShader(&fragmentShader);
    success = shaderProgram.link();
    if(!success) {
        qDebug() << "ERROR::SHADER::PROGRAM::LINKING_FAILED\n" << endl;
    }

    //VAO，VBO数据部分
    GLfloat vertices[] = {
        -0.5f, -0.5f, 0.0f, // left
         0.5f, -0.5f, 0.0f, // right
         0.0f,  0.5f, 0.0f  // top
    };

    vbo.create(); //创建buffer
    vbo.bind();
    vbo.allocate(vertices, sizeof(vertices));
//    vbo.allocate(sizeof(vertices));
//    vbo.write(0, vertices, sizeof(vertices));

    int attr = -1;
    attr = shaderProgram.attributeLocation("aPos");
    shaderProgram.setAttributeBuffer(attr, GL_FLOAT, 0, 3, sizeof(GLfloat) * 3);
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
