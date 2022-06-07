#include "app_version.h"
#include "mainwindow.h"

#include <QApplication>

int main(int argc, char *argv[])
{
#if QT_VERSION < QT_VERSION_CHECK(6,0,0)
    QApplication::setAttribute(Qt::AA_EnableHighDpiScaling);
#endif


    QApplication app(argc, argv);
    MainWindow window;

    window.setWindowTitle("Barium");
    window.setWindowIcon(QIcon(":/images/resources/Logo1.png"));
    app.setOrganizationName("The Barium Team");
    app.setApplicationName("Barium");
    app.setApplicationVersion(VERSION_STRING);

    window.show();
    return app.exec();
}
