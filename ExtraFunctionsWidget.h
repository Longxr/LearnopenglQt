#ifndef EXTRAFUNCTIONSWIDGET_H
#define EXTRAFUNCTIONSWIDGET_H

#include <QOpenGLWidget>
#include <QOpenGLExtraFunctions>
#include <QOpenGLFunctions_3_3_Core>
#include <QOpenGLShader>
#include <QOpenGLShaderProgram>

class ExtraFunctionsWidget : public QOpenGLWidget
                           , protected /*QOpenGLExtraFunctions*/QOpenGLFunctions_3_3_Core
{
    Q_OBJECT
public:
    explicit ExtraFunctionsWidget(QWidget *parent = nullptr);
    ~ExtraFunctionsWidget();

protected:
    virtual void initializeGL();
    virtual void resizeGL(int w, int h);
    virtual void paintGL();

private:
    QOpenGLShader *CreateShader(const QString& fileName, QOpenGLShader::ShaderTypeBit type);

    QOpenGLShaderProgram shaderProgram;
};

#endif // EXTRAFUNCTIONSWIDGET_H
