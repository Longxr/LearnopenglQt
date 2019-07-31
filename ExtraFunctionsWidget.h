#ifndef EXTRAFUNCTIONSWIDGET_H
#define EXTRAFUNCTIONSWIDGET_H

#include <QOpenGLWidget>
#include <QOpenGLExtraFunctions>

class ExtraFunctionsWidget : public QOpenGLWidget, protected QOpenGLExtraFunctions
{
    Q_OBJECT
public:
    explicit ExtraFunctionsWidget(QWidget *parent = nullptr);

protected:
    virtual void initializeGL();
    virtual void resizeGL(int w, int h);
    virtual void paintGL();
private:
    GLuint shaderProgram;
};

#endif // EXTRAFUNCTIONSWIDGET_H
