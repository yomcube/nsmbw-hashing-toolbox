#include <QApplication>
#include <QPushButton>
#include "ui_mainwindow.h"
#include "mainwindow.h"

int main(int argc, char **argv) {
    QApplication app(argc, argv);

    MainWindow ui;

    ui.show();

    return app.exec();
}
