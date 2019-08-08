#include <QApplication>
#include "QtFunctionWidget.h"
#include "CoreFunctionWidget.h"

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);

    QtFunctionWidget w1;
    CoreFunctionWidget w2;

    w1.setWindowTitle(QObject::tr("QtFunction"));
    w2.setWindowTitle(QObject::tr("CoreFunction"));

    w1.show();
    w2.show();

    return a.exec();
}
