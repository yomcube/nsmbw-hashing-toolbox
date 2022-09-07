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
    connect(ui->radUseDem, &QRadioButton::clicked, this, &DictionaryBruteWidget::setDemInputEnabledDisabled);
}

DictionaryBruteWidget::~DictionaryBruteWidget() {
    if (bruteforceThread != nullptr && bruteforceThread->isRunning()) {
        bruteforceThread->terminate();
    }
    delete ui;
}

void DictionaryBruteWidget::beginBruteforce() {

    ui->prefixMang->setEnabled(false);
    ui->suffixMang->setEnabled(false);
    ui->radUseDem->setEnabled(false);
    ui->mangHash->setEnabled(false);
    ui->demHash->setEnabled(false);
    ui->btnGo->setEnabled(false);

    uint32_t seedMang = Hashing::hash(ui->prefixMang->text(), 0x1505);
    uint32_t goalMang = Hashing::inverse_hash(ui->mangHash->text().toUInt(nullptr, 16), ui->suffixMang->text());

    ui->results->setText("");

    DictionaryBruteProcess *brute = new DictionaryBruteProcess(
        seedMang, goalMang,
        ui->radUseDem->isChecked(), ui->demHash->text().toUInt(nullptr, 16),
        ui->prefixMang->text(), ui->suffixMang->text()
    );
    bruteforceThread = new QThread();
    brute->moveToThread(bruteforceThread);
    connect(bruteforceThread, &QThread::started, brute, &DictionaryBruteProcess::startBruteforce);
    connect(brute, &DictionaryBruteProcess::updateProgress, this, &DictionaryBruteWidget::updateProgress);
    connect(brute, &DictionaryBruteProcess::bruteforceEnded, this, &DictionaryBruteWidget::bruteforceEnded);
    bruteforceThread->start();
}

void DictionaryBruteWidget::setDemInputEnabledDisabled(bool checked) {
    ui->demHash->setEnabled(checked);
}

void DictionaryBruteWidget::updateProgress(int prog) {
    ui->progressBar->setValue(prog);
}

void DictionaryBruteWidget::bruteforceEnded(QStringList res) {
    if (res.size() > 0) {
        QString resText = "";
        for (QString& entry : res) {
            resText += entry + "\n";
        }
        ui->results->setText(resText);
    } else {
        ui->results->setText("No collisions found.");
    }

    ui->progressBar->setValue(100.0);

    ui->prefixMang->setEnabled(true);
    ui->suffixMang->setEnabled(true);
    ui->radUseDem->setEnabled(true);
    ui->mangHash->setEnabled(true);
    ui->demHash->setEnabled(ui->radUseDem->isEnabled());
    ui->btnGo->setEnabled(true);

    bruteforceThread->exit();
}
