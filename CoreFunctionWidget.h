#ifndef COREFUNCTIONWIDGET_H
#define COREFUNCTIONWIDGET_H

#include <QOpenGLWidget>
#include <QOpenGLExtraFunctions>
#include <QOpenGLFunctions_3_3_Core>
#include <QOpenGLShaderProgram>

#include "Camera.h"

class CoreFunctionWidget : public QOpenGLWidget
                           , protected /*QOpenGLExtraFunctions*/QOpenGLFunctions_3_3_Core
{
    Q_OBJECT
public:
    explicit CoreFunctionWidget(QWidget *parent = nullptr);
    ~CoreFunctionWidget() Q_DECL_OVERRIDE;

protected:
    void initializeGL()  Q_DECL_OVERRIDE;
    void resizeGL(int w, int h) Q_DECL_OVERRIDE;
    void paintGL() Q_DECL_OVERRIDE;

    void keyPressEvent(QKeyEvent *event) Q_DECL_OVERRIDE;
    void keyReleaseEvent(QKeyEvent *event) Q_DECL_OVERRIDE;
    void mousePressEvent(QMouseEvent *event) Q_DECL_OVERRIDE;
    void mouseReleaseEvent(QMouseEvent *event) Q_DECL_OVERRIDE;
    void mouseMoveEvent(QMouseEvent *event) Q_DECL_OVERRIDE;
    void wheelEvent(QWheelEvent *event) Q_DECL_OVERRIDE;

private:
    bool createShader();
    uint loadTexture(const QString& path);

private:
    QOpenGLShaderProgram lightingShader, lampShader;

    QTimer* m_pTimer = nullptr;
    int     m_nTimeValue = 0;

    uint VBO, cubeVAO, lightVAO;
    uint diffuseMap, specularMap;

    // camera
    std::unique_ptr<Camera> camera;
    bool m_bLeftPressed;
    QPoint m_lastPos;
};

#endif // COREFUNCTIONWIDGET_H
