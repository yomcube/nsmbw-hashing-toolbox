#ifndef SYMBOLLISTWIDGET_H
#define SYMBOLLISTWIDGET_H

#include <QWidget>
#include "symbollistmodel.h"
#include <QtNetwork/qnetworkaccessmanager.h>
#include <QtNetwork/qnetworkrequest.h>
#include <QtNetwork/qnetworkreply.h>

namespace Ui {
    class SymbolListWidget;
}

class SymbolListWidget : public QWidget {
    Q_OBJECT

public:
    explicit SymbolListWidget(QWidget *parent = nullptr);
    ~SymbolListWidget();

private slots:
    void symListLoaded(QNetworkReply *reply);

private:
    Ui::SymbolListWidget *ui;

    SymbolListModel *slModel;
};

#endif // SYMBOLLISTWIDGET_H
