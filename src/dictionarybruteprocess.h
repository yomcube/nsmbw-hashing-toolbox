#ifndef DICTIONARYBRUTEPROCESS_H
#define DICTIONARYBRUTEPROCESS_H

#include <QObject>
#include <QProgressBar>

class DictionaryBruteProcess : public QObject {
    Q_OBJECT
public:
    DictionaryBruteProcess(uint32_t seedMang, uint32_t goalMang, uint32_t seedDem, uint32_t goalDem);

public slots:
    void startBruteforce();

signals:
    void updateProgress(int prog);
    void bruteforceEnded(QStringList res);

private:
    QStringList wordList;
    uint32_t seedMang;
    uint32_t goalMang;
    uint32_t seedDem;
    uint32_t goalDem;
};

#endif // DICTIONARYBRUTEPROCESS_H
