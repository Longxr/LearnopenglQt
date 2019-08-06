#ifndef SHADERPROGRAMWIDGET_H
#define SHADERPROGRAMWIDGET_H

#include <QOpenGLWidget>


#include <QOpenGLWidget>
#include <QOpenGLShader>
#include <QOpenGLShaderProgram>
#include <QDebug>
#include <QOpenGLFunctions>
#include <QOpenGLVertexArrayObject>
#include <QOpenGLBuffer>

class ShaderProgramWidget : public QOpenGLWidget, protected QOpenGLFunctions
{
public:
    ShaderProgramWidget(QWidget *parent = nullptr);
    ~ShaderProgramWidget() Q_DECL_OVERRIDE;

protected:
    virtual void initializeGL() Q_DECL_OVERRIDE;
    virtual void resizeGL(int w, int h) Q_DECL_OVERRIDE;
    virtual void paintGL() Q_DECL_OVERRIDE;

private:
    QOpenGLShader *CreateShader(const QString& fileName, QOpenGLShader::ShaderTypeBit type);

private:
    QOpenGLShaderProgram shaderProgram;
    QOpenGLVertexArrayObject vao;
    QOpenGLBuffer vbo;
};

#endif // SHADERPROGRAMWIDGET_H
