#include "symbollistwidget.h"
#include "ui_symbollistwidget.h"
#include <QMessageBox>

SymbolListWidget::SymbolListWidget(QWidget *parent) :
      QWidget(parent),
      ui(new Ui::SymbolListWidget)
{
    ui->setupUi(this);

    QNetworkAccessManager *mgr = new QNetworkAccessManager();
    connect(mgr, &QNetworkAccessManager::finished, this, &SymbolListWidget::symListLoaded);
    QNetworkRequest request(QUrl("https://rootcubed.dev/nsmbw-symbols/symbolList/symbols"));
    mgr->get(request);

    slModel = new SymbolListModel();
    ui->tableView->setModel(slModel);
    ui->tableView->horizontalHeader()->setSectionResizeMode(0, QHeaderView::Interactive);
    ui->tableView->horizontalHeader()->setSectionResizeMode(1, QHeaderView::Interactive);
    ui->tableView->horizontalHeader()->setSectionResizeMode(2, QHeaderView::Interactive);
    ui->tableView->horizontalHeader()->setSectionResizeMode(3, QHeaderView::Fixed);
    ui->tableView->setColumnWidth(3, 150);
    ui->tableView->horizontalHeader()->setSectionResizeMode(4, QHeaderView::Fixed);
    ui->tableView->setColumnWidth(4, 200);
    ui->tableView->horizontalHeader()->setSectionResizeMode(5, QHeaderView::Stretch);
}

SymbolListWidget::~SymbolListWidget() {
    delete ui;
}

void SymbolListWidget::symListLoaded(QNetworkReply *reply) {
    if (reply->error()) {
        QMessageBox::information(0, "Error",
            "Error loading symbol list:\n" + reply->errorString() + "\nSymbol list unavailable."
        );
        return;
    }

    slModel->loadData(reply->readAll());
}
