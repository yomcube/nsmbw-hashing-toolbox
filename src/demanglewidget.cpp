#include "demanglewidget.h"
#include "ui_demanglewidget.h"
#include "nvidia_demangler.h"

DemangleWidget::DemangleWidget(QWidget *parent) :
      QWidget(parent),
      ui(new Ui::DemangleWidget)
{
    ui->setupUi(this);
    QFont font("Courier New");
    font.setStyleHint(QFont::Monospace);
    ui->inpMangled->setFont(font);
    ui->outDemangled->setFont(font);
    connect(ui->inpMangled, &QPlainTextEdit::textChanged, this, &DemangleWidget::updateResult);
}

DemangleWidget::~DemangleWidget()
{
    delete ui;
}

void DemangleWidget::updateResult() {
    QStringList syms = ui->inpMangled->toPlainText().split("\n");
    QString resStr = "";
    for (int i = 0; i < syms.size(); i++) {
        resStr += QString::fromStdString(nvidia_demangler::nvidia_demangle(syms[i].toStdString())) + "\n";
    }
    ui->outDemangled->setPlainText(resStr);
}
