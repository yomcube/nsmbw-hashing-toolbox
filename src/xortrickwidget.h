#ifndef XORTRICKWIDGET_H
#define XORTRICKWIDGET_H

#include <QWidget>

namespace Ui {
    class XORTrickWidget;
}

class XORTrickWidget : public QWidget {
    Q_OBJECT

public:
    explicit XORTrickWidget(QWidget *parent = nullptr);
    ~XORTrickWidget();

private slots:
    void updateResult();

private:
    Ui::XORTrickWidget *ui;
};

#endif // XORTRICKWIDGET_H
