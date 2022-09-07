#ifndef DICTIONARYBRUTEPROCESS_H
#define DICTIONARYBRUTEPROCESS_H

#include <QObject>
#include <QProgressBar>

class DictionaryBruteProcess : public QObject {
    Q_OBJECT
public:
    DictionaryBruteProcess(uint32_t seedMang, uint32_t goalMang, bool useDem, uint32_t goalDem, QString prefix, QString suffix);

public slots:
    void startBruteforce();

signals:
    void updateProgress(int prog);
    void bruteforceEnded(QStringList res);

private:
    std::vector<std::string> wordList;
    QString prefix;
    QString suffix;
    uint32_t seedMang;
    uint32_t goalMang;
    bool useDem;
    uint32_t goalDem;
};

#endif // DICTIONARYBRUTEPROCESS_H
