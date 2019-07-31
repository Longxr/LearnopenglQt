#include "MainWindow.h"
#include "ui_MainWindow.h"

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::on_pushButton_clicked()
{
    static int rate = 1.0;

    QRect rect = ui->openGLWidget->rect().adjusted(rate,rate,-rate,-rate);

    ui->openGLWidget->drawRect(rect);
    rate += 4;
}
