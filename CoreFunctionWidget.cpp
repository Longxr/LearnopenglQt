#include "CoreFunctionWidget.h"
#include <QDebug>
#include <QTimer>
#include <QKeyEvent>
#include <QDateTime>

// lighting
static QVector3D lightPos(1.2f, 1.0f, 2.0f);

CoreFunctionWidget::CoreFunctionWidget(QWidget *parent) : QOpenGLWidget(parent)
{
    camera = std::make_unique<Camera>(QVector3D(0.0f, 0.0f, 3.0f));
    m_bLeftPressed = false;

    m_pTimer = new QTimer(this);
    connect(m_pTimer, &QTimer::timeout, this, [=]{
        m_nTimeValue += 1;
        update();
    });
    m_pTimer->start(40);//25 fps

}

CoreFunctionWidget::~CoreFunctionWidget()
{
    glDeleteVertexArrays(1, &lightVAO);
    glDeleteVertexArrays(1, &cubeVAO);
    glDeleteBuffers(1, &VBO);
}

void CoreFunctionWidget::initializeGL(){
    this->initializeOpenGLFunctions();

    createShader();

    // set up vertex data (and buffer(s)) and configure vertex attributes
    // ------------------------------------------------------------------
    float vertices[] = {
        // positions          // normals           // texture coords
        -0.5f, -0.5f, -0.5f,  0.0f,  0.0f, -1.0f,  0.0f,  0.0f,
         0.5f, -0.5f, -0.5f,  0.0f,  0.0f, -1.0f,  1.0f,  0.0f,
         0.5f,  0.5f, -0.5f,  0.0f,  0.0f, -1.0f,  1.0f,  1.0f,
         0.5f,  0.5f, -0.5f,  0.0f,  0.0f, -1.0f,  1.0f,  1.0f,
        -0.5f,  0.5f, -0.5f,  0.0f,  0.0f, -1.0f,  0.0f,  1.0f,
        -0.5f, -0.5f, -0.5f,  0.0f,  0.0f, -1.0f,  0.0f,  0.0f,

        -0.5f, -0.5f,  0.5f,  0.0f,  0.0f,  1.0f,  0.0f,  0.0f,
         0.5f, -0.5f,  0.5f,  0.0f,  0.0f,  1.0f,  1.0f,  0.0f,
         0.5f,  0.5f,  0.5f,  0.0f,  0.0f,  1.0f,  1.0f,  1.0f,
         0.5f,  0.5f,  0.5f,  0.0f,  0.0f,  1.0f,  1.0f,  1.0f,
        -0.5f,  0.5f,  0.5f,  0.0f,  0.0f,  1.0f,  0.0f,  1.0f,
        -0.5f, -0.5f,  0.5f,  0.0f,  0.0f,  1.0f,  0.0f,  0.0f,

        -0.5f,  0.5f,  0.5f, -1.0f,  0.0f,  0.0f,  1.0f,  0.0f,
        -0.5f,  0.5f, -0.5f, -1.0f,  0.0f,  0.0f,  1.0f,  1.0f,
        -0.5f, -0.5f, -0.5f, -1.0f,  0.0f,  0.0f,  0.0f,  1.0f,
        -0.5f, -0.5f, -0.5f, -1.0f,  0.0f,  0.0f,  0.0f,  1.0f,
        -0.5f, -0.5f,  0.5f, -1.0f,  0.0f,  0.0f,  0.0f,  0.0f,
        -0.5f,  0.5f,  0.5f, -1.0f,  0.0f,  0.0f,  1.0f,  0.0f,

         0.5f,  0.5f,  0.5f,  1.0f,  0.0f,  0.0f,  1.0f,  0.0f,
         0.5f,  0.5f, -0.5f,  1.0f,  0.0f,  0.0f,  1.0f,  1.0f,
         0.5f, -0.5f, -0.5f,  1.0f,  0.0f,  0.0f,  0.0f,  1.0f,
         0.5f, -0.5f, -0.5f,  1.0f,  0.0f,  0.0f,  0.0f,  1.0f,
         0.5f, -0.5f,  0.5f,  1.0f,  0.0f,  0.0f,  0.0f,  0.0f,
         0.5f,  0.5f,  0.5f,  1.0f,  0.0f,  0.0f,  1.0f,  0.0f,

        -0.5f, -0.5f, -0.5f,  0.0f, -1.0f,  0.0f,  0.0f,  1.0f,
         0.5f, -0.5f, -0.5f,  0.0f, -1.0f,  0.0f,  1.0f,  1.0f,
         0.5f, -0.5f,  0.5f,  0.0f, -1.0f,  0.0f,  1.0f,  0.0f,
         0.5f, -0.5f,  0.5f,  0.0f, -1.0f,  0.0f,  1.0f,  0.0f,
        -0.5f, -0.5f,  0.5f,  0.0f, -1.0f,  0.0f,  0.0f,  0.0f,
        -0.5f, -0.5f, -0.5f,  0.0f, -1.0f,  0.0f,  0.0f,  1.0f,

        -0.5f,  0.5f, -0.5f,  0.0f,  1.0f,  0.0f,  0.0f,  1.0f,
         0.5f,  0.5f, -0.5f,  0.0f,  1.0f,  0.0f,  1.0f,  1.0f,
         0.5f,  0.5f,  0.5f,  0.0f,  1.0f,  0.0f,  1.0f,  0.0f,
         0.5f,  0.5f,  0.5f,  0.0f,  1.0f,  0.0f,  1.0f,  0.0f,
        -0.5f,  0.5f,  0.5f,  0.0f,  1.0f,  0.0f,  0.0f,  0.0f,
        -0.5f,  0.5f, -0.5f,  0.0f,  1.0f,  0.0f,  0.0f,  1.0f
    };

    // first, configure the cube's VAO (and VBO)
    glGenVertexArrays(1, &cubeVAO);
    glGenBuffers(1, &VBO);

    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

    glBindVertexArray(cubeVAO);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(0);
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)(3 * sizeof(float)));
    glEnableVertexAttribArray(1);
    glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)(6 * sizeof(float)));
    glEnableVertexAttribArray(2);

    // second, configure the light's VAO (VBO stays the same; the vertices are the same for the light object which is also a 3D cube)
    glGenVertexArrays(1, &lightVAO);
    glBindVertexArray(lightVAO);

    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    // note that we update the lamp's position attribute's stride to reflect the updated buffer data
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(0);

    // load textures (we now use a utility function to keep the code more organized)
    // -----------------------------------------------------------------------------
    diffuseMap = loadTexture(":/container2.png");
    specularMap = loadTexture(":/container2_specular.png");

    // shader configuration
    // --------------------
    lightingShader.bind();
    lightingShader.setUniformValue("material.diffuse", 0);
    lightingShader.setUniformValue("material.specular", 1);
    lightingShader.release();

    // configure global opengl state
    // -----------------------------
    glEnable(GL_DEPTH_TEST);
}

void CoreFunctionWidget::resizeGL(int w, int h){
    glViewport(0, 0, w, h);
}

void CoreFunctionWidget::paintGL(){
    // input
    // -----
    camera->processInput(0.5f);//speed

    // render
    // ------
    glClearColor(0.1f, 0.1f, 0.1f, 1.0f);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    // be sure to activate shader when setting uniforms/drawing objects
    lightingShader.bind();
    lightingShader.setUniformValue("light.position", lightPos);
    lightingShader.setUniformValue("viewPos", camera->position);

    // light properties
    lightingShader.setUniformValue("light.ambient", QVector3D(0.2f, 0.2f, 0.2f));
    lightingShader.setUniformValue("light.diffuse", QVector3D(0.5f, 0.5f, 0.5f));
    lightingShader.setUniformValue("light.specular", QVector3D(1.0f, 1.0f, 1.0f));

    // material properties
    lightingShader.setUniformValue("material.shininess", 64.0f);

    // view/projection transformations
    QMatrix4x4 projection;
    projection.perspective(camera->zoom, 1.0f * width() / height(), 0.1f, 100.0f);
    QMatrix4x4 view = camera->getViewMatrix();
    lightingShader.setUniformValue("projection", projection);
    lightingShader.setUniformValue("view", view);

    // world transformation
    QMatrix4x4 model;
    lightingShader.setUniformValue("model", model);

    // bind diffuse map
    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_2D, diffuseMap);
    // bind specular map
    glActiveTexture(GL_TEXTURE1);
    glBindTexture(GL_TEXTURE_2D, specularMap);

    // render the cube
    glBindVertexArray(cubeVAO);
    glDrawArrays(GL_TRIANGLES, 0, 36);
    lightingShader.release();

    // also draw the lamp object
    lampShader.bind();
    lampShader.setUniformValue("projection", projection);
    lampShader.setUniformValue("view", view);
    model = QMatrix4x4();
    model.translate(lightPos);
    model.scale(0.2f); // a smaller cube
    lampShader.setUniformValue("model", model);

    glBindVertexArray(lightVAO);
    glDrawArrays(GL_TRIANGLES, 0, 36);
    lampShader.release();
}

void CoreFunctionWidget::keyPressEvent(QKeyEvent *event)
{
    int key = event->key();
    if (key >= 0 && key < 1024)
        camera->keys[key] = true;
}

void CoreFunctionWidget::keyReleaseEvent(QKeyEvent *event)
{
    int key = event->key();
    if (key >= 0 && key < 1024)
        camera->keys[key] = false;
}

void CoreFunctionWidget::mousePressEvent(QMouseEvent *event)
{
    if(event->button() == Qt::LeftButton){
        m_bLeftPressed = true;
        m_lastPos = event->pos();
    }
}

void CoreFunctionWidget::mouseReleaseEvent(QMouseEvent *event)
{
    Q_UNUSED(event);

    m_bLeftPressed = false;
}

void CoreFunctionWidget::mouseMoveEvent(QMouseEvent *event)
{
    int xpos = event->pos().x();
    int ypos = event->pos().y();

    int xoffset = xpos - m_lastPos.x();
    int yoffset = m_lastPos.y() - ypos;
    m_lastPos = event->pos();

    camera->processMouseMovement(xoffset, yoffset);
}

void CoreFunctionWidget::wheelEvent(QWheelEvent *event)
{
    QPoint offset = event->angleDelta();
    camera->processMouseScroll(offset.y()/20.0f);
}

bool CoreFunctionWidget::createShader()
{
    bool success = lightingShader.addShaderFromSourceFile(QOpenGLShader::Vertex, ":/lighting_maps.vert");
    if (!success) {
        qDebug() << "shaderProgram addShaderFromSourceFile failed!" << lightingShader.log();
        return success;
    }

    success = lightingShader.addShaderFromSourceFile(QOpenGLShader::Fragment, ":/lighting_maps.frag");
    if (!success) {
        qDebug() << "shaderProgram addShaderFromSourceFile failed!" << lightingShader.log();
        return success;
    }

    success = lightingShader.link();
    if(!success) {
        qDebug() << "shaderProgram link failed!" << lightingShader.log();
    }

    success = lampShader.addShaderFromSourceFile(QOpenGLShader::Vertex, ":/lamp.vert");
    if (!success) {
        qDebug() << "shaderProgram addShaderFromSourceFile failed!" << lampShader.log();
        return success;
    }

    success = lampShader.addShaderFromSourceFile(QOpenGLShader::Fragment, ":/lamp.frag");
    if (!success) {
        qDebug() << "shaderProgram addShaderFromSourceFile failed!" << lampShader.log();
        return success;
    }

    success = lampShader.link();
    if(!success) {
        qDebug() << "shaderProgram link failed!" << lampShader.log();
    }

    return success;
}

uint CoreFunctionWidget::loadTexture(const QString& path)
{
    uint textureID;
    glGenTextures(1, &textureID);

    QImage image = QImage(path).convertToFormat(QImage::Format_RGBA8888).mirrored(true, true);
    if (!image.isNull()) {
        glBindTexture(GL_TEXTURE_2D, textureID);
        glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, image.width(), image.height(), 0, GL_RGBA, GL_UNSIGNED_BYTE, image.bits());
        glGenerateMipmap(GL_TEXTURE_2D);

        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    }

    return textureID;
}
