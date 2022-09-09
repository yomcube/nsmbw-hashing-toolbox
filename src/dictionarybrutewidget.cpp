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
    connect(ui->radUseDem, &QRadioButton::clicked, this, &DictionaryBruteWidget::setDemInput);
    connect(ui->radUseCustom, &QRadioButton::clicked, this, &DictionaryBruteWidget::setCustomInput);
}

DictionaryBruteWidget::~DictionaryBruteWidget() {
    if (bruteforceThread != nullptr && bruteforceThread->isRunning()) {
        bruteforceThread->terminate();
    }
    delete ui;
}

void DictionaryBruteWidget::beginBruteforce() {

    ui->prefixSym1->setEnabled(false);
    ui->suffixSym1->setEnabled(false);
    ui->prefixSym2->setEnabled(false);
    ui->suffixSym2->setEnabled(false);
    ui->radUseDem->setEnabled(false);
    ui->radUseCustom->setEnabled(false);
    ui->btnGo->setEnabled(false);

    ui->results->setText("");

    DictionaryBruteProcess *brute = new DictionaryBruteProcess();

    brute->setGoalHashes(ui->hashSym1->text().toUInt(nullptr, 16), ui->hashSym2->text().toUInt(nullptr, 16));
    brute->setKnownPartsForSymbol1(ui->prefixSym1->text(), ui->suffixSym1->text());
    brute->setKnownPartsForSymbol2(ui->prefixSym2->text(), ui->suffixSym2->text());
    brute->setUseDemangler(ui->radUseDem->isChecked());

    bruteforceThread = new QThread();
    brute->moveToThread(bruteforceThread);
    connect(bruteforceThread, &QThread::started, brute, &DictionaryBruteProcess::startBruteforce);
    connect(brute, &DictionaryBruteProcess::updateProgress, this, &DictionaryBruteWidget::updateProgress);
    connect(brute, &DictionaryBruteProcess::bruteforceEnded, this, &DictionaryBruteWidget::bruteforceEnded);
    bruteforceThread->start();
}

void DictionaryBruteWidget::setDemInput() {
    ui->prefixSym2->setEnabled(false);
    ui->suffixSym2->setEnabled(false);
}

void DictionaryBruteWidget::setCustomInput() {
    ui->prefixSym2->setEnabled(true);
    ui->suffixSym2->setEnabled(true);
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

    ui->prefixSym1->setEnabled(true);
    ui->suffixSym1->setEnabled(true);
    ui->prefixSym2->setEnabled(ui->radUseCustom->isChecked());
    ui->suffixSym2->setEnabled(ui->radUseCustom->isChecked());
    ui->radUseDem->setEnabled(true);
    ui->radUseCustom->setEnabled(true);
    ui->btnGo->setEnabled(true);

    bruteforceThread->exit();
}
