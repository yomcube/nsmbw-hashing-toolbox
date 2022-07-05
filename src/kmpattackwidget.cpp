#include "kmpattackwidget.h"
#include "ui_kmpattackwidget.h"

KMPAttackWidget::KMPAttackWidget(QWidget *parent) :
      QWidget(parent),
      ui(new Ui::KMPAttackWidget)
{
    ui->setupUi(this);

    connect(ui->prefix1Input, &QLineEdit::textChanged, this, &KMPAttackWidget::updateSummary);
    connect(ui->prefix2Input, &QLineEdit::textChanged, this, &KMPAttackWidget::updateSummary);
    connect(ui->suffix1Input, &QLineEdit::textChanged, this, &KMPAttackWidget::updateSummary);
    connect(ui->suffix2Input, &QLineEdit::textChanged, this, &KMPAttackWidget::updateSummary);
}

KMPAttackWidget::~KMPAttackWidget() {
    delete ui;
}

void KMPAttackWidget::updateSummary() {
    ui->lblPlainOut1->setText("(...)" + ui->prefix1Input->text() + "<unknown>" + ui->suffix1Input->text());
    ui->lblPlainOut2->setText("(...)" + ui->prefix2Input->text() + "<unknown>" + ui->suffix2Input->text());
}
