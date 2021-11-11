#include "mainwindow.h"

#include <QApplication>
#include <QObject>

Q_DECLARE_METATYPE(ResourceIntensiveTask*);
Q_DECLARE_METATYPE(ResourceIntensiveTask);
Q_DECLARE_METATYPE(Matrix*);
Q_DECLARE_METATYPE(Matrix);

int main(int argc, char *argv[])
{

    qRegisterMetaType<ResourceIntensiveTask*>();
    qRegisterMetaType<ResourceIntensiveTask>();
    qRegisterMetaType<Matrix*>();
    qRegisterMetaType<Matrix>();

    QApplication a(argc, argv);
    MainWindow w;
    w.show();
    return a.exec();
}
