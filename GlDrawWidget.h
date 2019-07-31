#ifndef GLDRAWWIDGET_H
#define GLDRAWWIDGET_H

#include <QObject>

#include <QOpenGLWidget>
#include <QOpenGLFunctions>
#include <QOpenGLBuffer>
QT_FORWARD_DECLARE_CLASS(QOpenGLShaderProgram)
class GlDrawWidget : public QOpenGLWidget, protected QOpenGLFunctions
{
    Q_OBJECT
public:
    GlDrawWidget(QWidget *parent = 0);
    ~GlDrawWidget();
    virtual void drawRect(QRect &r); //相对于本窗体的Rect

protected:
    void initializeGL() override;
    void paintGL() override;

private:
    QOpenGLShaderProgram *shaderProgram = nullptr;
    QOpenGLBuffer vbo;
};

#endif // GLDRAWWIDGET_H
