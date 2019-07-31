#ifndef SHADERPROGRAMWIDGET_H
#define SHADERPROGRAMWIDGET_H

#include <QOpenGLWidget>


#include <QOpenGLWidget>
#include <QOpenGLShader>
#include <QOpenGLShaderProgram>
#include <QDebug>
#include <QOpenGLFunctions>

class ShaderProgramWidget : public QOpenGLWidget, protected QOpenGLFunctions
{
public:
    ShaderProgramWidget(QWidget *parent = nullptr);
    ~ShaderProgramWidget();
protected:
    virtual void initializeGL();
    virtual void resizeGL(int w, int h);
    virtual void paintGL();
private:
    QOpenGLShaderProgram shaderProgram;
    QVector<QVector3D> vertices;
};

#endif // SHADERPROGRAMWIDGET_H
