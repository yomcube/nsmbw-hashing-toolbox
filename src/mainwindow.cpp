#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <QMessageBox>
#include <QDebug>

MainWindow::MainWindow(QWidget *parent) :
      QMainWindow(parent),
      ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    connect(ui->submitSymbolBtn, &QPushButton::clicked, this, &MainWindow::submitSymbol);
}

MainWindow::~MainWindow() {
    delete ui;
}

void MainWindow::submitSymbol() {
    QNetworkAccessManager *mgr = new QNetworkAccessManager();
    connect(mgr, &QNetworkAccessManager::finished, this, &MainWindow::symSubmitted);
    QNetworkRequest request(QUrl("https://rootcubed.dev/nsmbw-symbols/symbolList/submit_symbol?sym=" + ui->submitSymName->text()));
    mgr->get(request);
}

void MainWindow::symSubmitted(QNetworkReply *reply) {
    if (reply->error()) {
        QMessageBox::information(0, "Error", "Error submitting symbol:\n" + reply->errorString());
        return;
    }
    QString replyStr = reply->readAll();
    if (replyStr == "ok") {
        QMessageBox::information(0, "Success", "Symbol added to database!");
    } else {
        QMessageBox::information(0, "Error", "Symbol not added to database.\nReason:\n" + replyStr);
    }
}
