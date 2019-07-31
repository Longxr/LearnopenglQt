#include <QApplication>
#include "MainWindow.h"
#include "ShaderProgramWidget.h"
#include "ExtraFunctionsWidget.h"

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);

//    MainWindow w;
    ShaderProgramWidget w;
//    ExtraFunctionsWidget w;

    w.show();

    return a.exec();
}
