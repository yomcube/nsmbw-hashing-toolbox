#ifndef DEMANGLEWIDGET_H
#define DEMANGLEWIDGET_H

#include <QWidget>

namespace Ui {
class DemangleWidget;
}

class DemangleWidget : public QWidget
{
    Q_OBJECT

public:
    explicit DemangleWidget(QWidget *parent = nullptr);
    ~DemangleWidget();

private slots:
    void updateResult();

private:
    Ui::DemangleWidget *ui;
};

#endif // DEMANGLEWIDGET_H
