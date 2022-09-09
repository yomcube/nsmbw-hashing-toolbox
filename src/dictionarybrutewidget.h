#ifndef DICTIONARYBRUTEWIDGET_H
#define DICTIONARYBRUTEWIDGET_H

#include <QWidget>
#include <QThread>

namespace Ui {
    class DictionaryBruteWidget;
}

class DictionaryBruteWidget : public QWidget {
    Q_OBJECT

public:
    explicit DictionaryBruteWidget(QWidget *parent = nullptr);
    ~DictionaryBruteWidget();

private slots:
    void beginBruteforce();
    void updateProgress(int prog);
    void bruteforceEnded(QStringList res);
    void setDemInput();
    void setCustomInput();

private:
    Ui::DictionaryBruteWidget *ui;
    QThread *bruteforceThread = nullptr;
};

#endif // DICTIONARYBRUTEWIDGET_H
