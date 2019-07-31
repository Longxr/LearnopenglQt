#ifndef EXTRAFUNCTIONSWIDGET_H
#define EXTRAFUNCTIONSWIDGET_H

#include <QOpenGLWidget>
#include <QOpenGLExtraFunctions>
#include <QOpenGLFunctions_3_3_Core>

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
    GLuint shaderProgram;
};

#endif // EXTRAFUNCTIONSWIDGET_H
