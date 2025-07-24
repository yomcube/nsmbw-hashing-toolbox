#include "hashwidget.h"
#include "ui_hashwidget.h"
#include "hashing.h"

HashWidget::HashWidget(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::HashWidget)
{
    ui->setupUi(this);
    QFont font("Courier New");
    font.setStyleHint(QFont::Monospace);
    ui->inpStrs->setFont(font);
    ui->outHashes->setFont(font);
    connect(ui->inpStrs, &QPlainTextEdit::textChanged, this, &HashWidget::updateResult);
}

HashWidget::~HashWidget()
{
    delete ui;
}

void HashWidget::updateResult() {
    QStringList strings = ui->inpStrs->toPlainText().split("\n");
    QString resStr = "";
    for (int i = 0; i < strings.size(); i++) {
        uint res = Hashing::hash(strings[i], 0x1505);
        resStr += "0x" + QString::number(res, 16) + "\n";
    }
    ui->outHashes->setPlainText(resStr);
}
