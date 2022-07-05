#include "dictionarybrutewidget.h"
#include "ui_dictionarybrutewidget.h"
#include <QFile>
#include <QTextStream>
#include <QMessageBox>
#include <unordered_map>
#include "hashing.h"
#include "dictionarybruteprocess.h"

DictionaryBruteWidget::DictionaryBruteWidget(QWidget *parent) :
      QWidget(parent),
      ui(new Ui::DictionaryBruteWidget)
{
    ui->setupUi(this);

    connect(ui->btnGo, &QPushButton::clicked, this, &DictionaryBruteWidget::beginBruteforce);
}

DictionaryBruteWidget::~DictionaryBruteWidget() {
    if (bruteforceThread != nullptr && bruteforceThread->isRunning()) {
        bruteforceThread->terminate();
    }
    delete ui;
}

void DictionaryBruteWidget::beginBruteforce() {
    uint32_t seedMang = Hashing::hash(ui->prefixMang->text(), 0x1505);
    uint32_t goalMang = Hashing::inverse_hash(ui->mangHash->text().toUInt(nullptr, 16), ui->suffixMang->text());
    uint32_t seedDem = Hashing::hash(ui->prefixDem->text(), 0x1505);
    uint32_t goalDem = Hashing::inverse_hash(ui->demHash->text().toUInt(nullptr, 16), ui->suffixDem->text());

    ui->results->setText("");

    DictionaryBruteProcess *brute = new DictionaryBruteProcess(seedMang, goalMang, seedDem, goalDem);
    bruteforceThread = new QThread();
    brute->moveToThread(bruteforceThread);
    connect(bruteforceThread, &QThread::started, brute, &DictionaryBruteProcess::startBruteforce);
    connect(brute, &DictionaryBruteProcess::updateProgress, this, &DictionaryBruteWidget::updateProgress);
    connect(brute, &DictionaryBruteProcess::bruteforceEnded, this, &DictionaryBruteWidget::bruteforceEnded);
    bruteforceThread->start();
}

void DictionaryBruteWidget::updateProgress(int prog) {
    ui->progressBar->setValue(prog);
}

void DictionaryBruteWidget::bruteforceEnded(QStringList res) {
    QString prefixMang = ui->prefixMang->text();
    QString suffixMang = ui->suffixMang->text();

    if (res.size() > 0) {
        QString resText = "";
        for (QString& entry : res) {
            resText += prefixMang + entry + suffixMang + "\n";
        }
        ui->results->setText(resText);
    } else {
        ui->results->setText("No collisions found.");
    }

    ui->progressBar->setValue(100.0);

    bruteforceThread->exit();
}
