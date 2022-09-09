#ifndef DICTIONARYBRUTEPROCESS_H
#define DICTIONARYBRUTEPROCESS_H

#include <QObject>
#include <QProgressBar>

class DictionaryBruteProcess : public QObject {
    Q_OBJECT
public:
    DictionaryBruteProcess();

    void setGoalHashes(uint32_t h1, uint32_t h2);
    void setKnownPartsForSymbol1(QString prefix, QString suffix);
    void setKnownPartsForSymbol2(QString prefix, QString suffix);
    void setUseDemangler(bool useDemangler);

public slots:
    void startBruteforce();

signals:
    void updateProgress(int prog);
    void bruteforceEnded(QStringList res);

private:
    std::vector<std::string> wordList;

    uint32_t goalH1;
    uint32_t goalH2;

    QString prefix1;
    QString suffix1;
    QString prefix2;
    QString suffix2;

    bool useDemangler = true;
};

#endif // DICTIONARYBRUTEPROCESS_H
