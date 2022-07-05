#ifndef KMPATTACKWIDGET_H
#define KMPATTACKWIDGET_H

#include <QWidget>

namespace Ui {
    class KMPAttackWidget;
}

class KMPAttackWidget : public QWidget {
    Q_OBJECT

public:
    explicit KMPAttackWidget(QWidget *parent = nullptr);
    ~KMPAttackWidget();

private slots:
    void updateSummary();

private:
    Ui::KMPAttackWidget *ui;
};

#endif // KMPATTACKWIDGET_H
