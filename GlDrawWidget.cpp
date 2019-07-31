#include "GlDrawWidget.h"


#include <QOpenGLShaderProgram>
#include <QDebug>

GlDrawWidget::GlDrawWidget(QWidget *parent)
    : QOpenGLWidget(parent)
{
}

GlDrawWidget::~GlDrawWidget()
{
    makeCurrent();vbo.destroy();
}

void GlDrawWidget::initializeGL()
{
    initializeOpenGLFunctions();
    GLfloat cols[12]{  //我预先指定了边框的颜色,这里我使用的是QOpenglBuffer，且是一个私有变量，在对象存在期间不会销毁,所以不用加static关键字，加上也可以。如果是直接setAttributeArray()一个局部的数据就要加上static,因为画一个顶点都会访问一次该buffer中的颜色,如果该内存不在了，结果会不对，貌似opengl内部没有把这个数据存到它自己那里(个人根据结果猜测)。但是如果是在paintGL()函数中临时加载数据就可以是栈区的，因为paintGL()函数中会执行画的操作,没画完之前,栈区的数据是不会释放的。

        0.0f,1.0f,1.0f, 0.0f,1.0f,1.0f,
        0.0f,1.0f,1.0f, 0.0f,1.0f,1.0f,
    };
    vbo.create(); //创建buffer
    vbo.bind();   //绑定到当前上下文
    vbo.allocate(32 * sizeof(GLfloat)); //分配32个GLfloat的数据空间,其中前24个点对应四个边线顶点的颜色
    vbo.write(0,cols,sizeof(cols)); //在起始位置写入颜色

    shaderProgram = new QOpenGLShaderProgram(this);

    static const char vsrc[]{ //很简单就是传入顶点,和颜色
        "attribute highp vec4 vertex;\n" \
        "attribute mediump vec4 col;\n" \
        "varying mediump vec4 color;\n" \
        "void main()\n"
        "{\n"
        "   gl_Position=vertex;\n"
        "   color=col;\n"
        "}"
    };
    QOpenGLShader *vshader = new QOpenGLShader(QOpenGLShader::Vertex,this);
    vshader->compileSourceCode(vsrc);

    static const char fsrc[]{
        "varying mediump vec4 color;\n"
        "void main()\n"
        "{\n"
        "   gl_FragColor=color;\n"
        "}"
    };
    QOpenGLShader *fshader = new QOpenGLShader(QOpenGLShader::Fragment,this);
    fshader->compileSourceCode(fsrc);

    shaderProgram->addShader(vshader);
    shaderProgram->addShader(fshader);
    shaderProgram->bind();
    shaderProgram->link(); //必须在link、和bind之后，才能设置attribute变量的值，opegnl决定的
    shaderProgram->enableAttributeArray("vertex");
    shaderProgram->enableAttributeArray("col");
    shaderProgram->setAttributeBuffer("vertex",GL_FLOAT,24 * sizeof(GLfloat),2,2*sizeof(GLfloat));
    shaderProgram->setAttributeBuffer("col",GL_FLOAT,0,3,3*sizeof(GLfloat));
    glEnable(GL_DEPTH_TEST); //启动深度测试
//    glEnable(GL_LINE_SMOOTH);
    glClearColor(0.0f,0.0f,0.0f,0.0f); //背景透明,还需设置FramelessWindowHint和WA_TranslucentBackground
}

void GlDrawWidget::paintGL()
{
    glDrawArrays(GL_LINE_LOOP,0,4); //根据点顺序画封闭线
}

void GlDrawWidget::drawRect(QRect &r)
{
    makeCurrent();  //必须使用该函数;使其处理渲染当前opengl上下文的状态，并且会绑定内存空间到那个opengl上下文;后面对opengl中的操作才会有效。在initialIzeGL()和paintGL()中系统会在调用它们之前调用这个接口，所以不用再调。也可以把操作直接写到paintGL()中。
    GLfloat w = 2 * GLfloat(r.width()) / width();
    GLfloat h = 2 * GLfloat(r.height()) / height();
    GLfloat x = -1.0 + 2 * GLfloat(r.x()) / width();
    GLfloat y = -1.0 + 2 * GLfloat(r.y()) / height();
    GLfloat vertexs[]{
        x,y,
        x + w, y,
        x + w, y + h,
        x, y + h
    }; //变换顶点到opengl坐标
    vbo.write(24 * sizeof(GLfloat),vertexs,sizeof(vertexs)); //写入数据
    update(); //重新绘画
}
