#include "z3attackwidget.h"
#include "ui_z3attackwidget.h"
#include <QMessageBox>
#include <QThread>
#include <QTimer>
#include "z3attackprocess.h"
#include "hashing.h"

Z3AttackWidget::Z3AttackWidget(QWidget *parent) :
      QWidget(parent),
      ui(new Ui::Z3AttackWidget)
{
    ui->setupUi(this);

    z3Timer = new QTimer(this);
    z3Timer->setSingleShot(true);
    connect(z3Timer, &QTimer::timeout, this, &Z3AttackWidget::z3Terminate);
    connect(ui->btnStartSearch, &QPushButton::clicked, this, &Z3AttackWidget::beginZ3);
}

Z3AttackWidget::~Z3AttackWidget() {
    delete ui;
}

void Z3AttackWidget::beginZ3() {
    h1seed = Hashing::hash(ui->hash1Prefix->text(), 0x1505);
    h1goal = Hashing::inverse_hash(ui->hash1Value->text().toUInt(nullptr, 16), ui->hash1Suffix->text());
    h2seed = Hashing::hash(ui->hash2Prefix->text(), 0x1505);
    h2goal = Hashing::inverse_hash(ui->hash2Value->text().toUInt(nullptr, 16), ui->hash2Suffix->text());

    minLen = ui->minLen->text().toInt();
    maxLen = ui->maxLen->text().toInt();
    if (minLen > maxLen) {
        QMessageBox::information(this, "Error", "Minimum length must be >= maximum length!");
        return;
    }

    currLen = minLen;

    ui->results->setText("");
    z3Iter();
}

void Z3AttackWidget::z3Iter() {
    Z3AttackProcess *z3Proc = new Z3AttackProcess(h1seed, h1goal, h2seed, h2goal, currLen);
    z3Thread = new QThread(this);
    z3Proc->moveToThread(z3Thread);
    connect(z3Thread, &QThread::started, z3Proc, &Z3AttackProcess::z3Start);
    connect(z3Proc, &Z3AttackProcess::z3Finished, this, &Z3AttackWidget::z3IterDone);
    z3Thread->start();

    alreadyDone = false;
    z3Timer->start(ui->z3Timeout->text().toUInt() * 1000);
}

void Z3AttackWidget::z3Terminate() {
    if (!alreadyDone) {
        z3Thread->terminate();
        z3IterDone(QString("Timeout."));
    }
}

void Z3AttackWidget::z3IterDone(QString res) {
    alreadyDone = true;
    z3Timer->stop();
    ui->results->setText(ui->results->toPlainText() + "Length " + QString::number(currLen) + ":\n" + res + "\n");
    z3Thread->quit();
    currLen++;
    if (currLen <= maxLen) {
        z3Iter();
    }
}
