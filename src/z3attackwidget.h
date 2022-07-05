#ifndef Z3ATTACKWIDGET_H
#define Z3ATTACKWIDGET_H

#include <QWidget>

namespace Ui {
    class Z3AttackWidget;
}

class Z3AttackWidget : public QWidget {
    Q_OBJECT

public:
    explicit Z3AttackWidget(QWidget *parent = nullptr);
    ~Z3AttackWidget();

public slots:
    void beginZ3();
    void z3Terminate();

signals:

private:
    void z3Iter();
    void z3IterDone(QString res);
    Ui::Z3AttackWidget *ui;
    int minLen, maxLen;
    int currLen;
    bool alreadyDone;
    uint32_t h1seed, h1goal, h2seed, h2goal;
    QThread *z3Thread;
    QTimer *z3Timer;
};

#endif // Z3ATTACKWIDGET_H
