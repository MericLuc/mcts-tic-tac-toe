#include <QApplication>

#include "ui/mainwindow.h"

#include <iostream>
#include <memory>

int
main(int argc, char* argv[])
{
    QApplication app(argc, argv);
    MainWindow   window;

    window.show();

    return app.exec();
}
