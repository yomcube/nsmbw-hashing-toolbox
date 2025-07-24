#include "invhashwidget.h"
#include "ui_invhashwidget.h"
#include "hashing.h"

InvHashWidget::InvHashWidget(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::InvHashWidget)
{
    ui->setupUi(this);
    QFont font("Courier New");
    font.setStyleHint(QFont::Monospace);
    ui->hashInput->setFont(font);
    ui->charInput->setFont(font);
    ui->result->setFont(font);

    connect(ui->hashInput, &QPlainTextEdit::textChanged, this, &InvHashWidget::updateResult);
    connect(ui->charInput, &QPlainTextEdit::textChanged, this, &InvHashWidget::updateResult);
}

InvHashWidget::~InvHashWidget() {
    delete ui;
}

void InvHashWidget::updateResult() {
    QStringList hashes = ui->hashInput->toPlainText().split("\n");
    QStringList charsToUndo = ui->charInput->toPlainText().split("\n");
    QString resStr = "";
    int i = 0;
    while (i < hashes.size() && i < charsToUndo.size()) {
        uint hex = hashes[i].toUInt(nullptr, 16);
        uint res = Hashing::inverse_hash(hex, charsToUndo[i]);
        resStr += "0x" + QString::number(res, 16).rightJustified(8, '0') + "\n";
        i++;
    }
    ui->result->setPlainText(resStr);
}
