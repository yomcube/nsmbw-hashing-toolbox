#ifndef INVHASHWIDGET_H
#define INVHASHWIDGET_H

#include <QWidget>

namespace Ui {
    class InvHashWidget;
}

class InvHashWidget : public QWidget {
    Q_OBJECT

public:
    explicit InvHashWidget(QWidget *parent = nullptr);
    ~InvHashWidget();

private slots:
    void updateResult();

private:
    Ui::InvHashWidget *ui;
};

#endif // INVHASHWIDGET_H
