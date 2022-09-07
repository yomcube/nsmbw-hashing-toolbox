#include "xortrickwidget.h"
#include "ui_xortrickwidget.h"
#include "hashing.h"

XORTrickWidget::XORTrickWidget(QWidget *parent) :
      QWidget(parent),
      ui(new Ui::XORTrickWidget)
{
    ui->setupUi(this);
    QFont font("Courier New");
    font.setStyleHint(QFont::Monospace);
    ui->hashInput->setFont(font);
    ui->charInput->setFont(font);
    ui->result->setFont(font);

    connect(ui->hashInput, &QPlainTextEdit::textChanged, this, &XORTrickWidget::updateResult);
    connect(ui->charInput, &QPlainTextEdit::textChanged, this, &XORTrickWidget::updateResult);
}

XORTrickWidget::~XORTrickWidget() {
    delete ui;
}

void XORTrickWidget::updateResult() {
    QStringList hashes = ui->hashInput->toPlainText().split("\n");
    QStringList xorChars = ui->charInput->toPlainText().split("\n");
    QString resStr = "";
    int i = 0;
    while (i < hashes.size() && i < xorChars.size()) {
        if (hashes[i].length() == 0) {
            resStr += "\n";
            i++;
            continue;
        }
        QStringList individualHashes = hashes[i].split(QRegExp("[_ ,]"));
        uint runningXOR = 0;
        for (int j = 0; j < individualHashes.size(); j++) {
            runningXOR ^= individualHashes[j].toUInt(nullptr, 16);
        }
        runningXOR ^= Hashing::xorString(xorChars[i]);

        // figure out possibilities
        QString currRes = "XOR = " + QString::number(runningXOR & 0b11111) + " -> ";

        QStringList results = QStringList();

        if (runningXOR % 32 == 0) {
            results.append("(none)");
        }

        for (char c0 = '1'; c0 <= '9'; c0++) {
            if ((runningXOR ^ c0) % 32 == 0) {
                results.append(QString(c0));
            }
        }
        for (char c0 = '1'; c0 <= '9'; c0++) {
            for (char c1 = '0'; c1 <= '9'; c1++) {
                if ((runningXOR ^ c0 ^ c1) % 32 == 0) {
                    results.append(QString(c0) + QString(c1));
                }
            }
        }

        if (results.size() == 0) {
            currRes += "(no solutions for 0 < len(className) < 100)";
        } else {
            currRes += results.join(", ");
        }

        resStr += currRes + "\n";
        i++;
    }
    ui->result->setPlainText(resStr);
}
