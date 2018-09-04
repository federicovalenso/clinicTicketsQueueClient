#include "mainwindow.h"
#include <QApplication>
#include <QString>
#include <QDebug>

int main(int argc, char *argv[])
{
    QApplication app(argc, argv);
    QApplication::setStyle("Fusion");
    app.setOrganizationName("MANO \'MDC\'");
    app.setApplicationName("QueueTerminal");
    MainWindow w;
    w.show();

    return app.exec();
}
