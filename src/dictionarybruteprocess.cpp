#include "dictionarybruteprocess.h"
#include <QFile>
#include <QTextStream>
#include <QMessageBox>
#include <unordered_map>
#include <vector>
#include "hashing.h"
#include "nvidia_demangler.h"

DictionaryBruteProcess::DictionaryBruteProcess() {
    QFile wordFile(":/res/nsmbwWordList.txt");
    if(!wordFile.open(QIODevice::ReadOnly)) {
        QMessageBox::information(0, "Error",
                                 "Error reading word list:\n" + wordFile.errorString() + "\nDictionary attack unavailable."
                                 );
    }

    QTextStream in(&wordFile);
    while(!in.atEnd()) {
        wordList.emplace_back(in.readLine().toStdString());
    }
    wordFile.close();

    wordList.emplace_back("");
}

void DictionaryBruteProcess::setGoalHashes(uint32_t h1, uint32_t h2) {
    this->goalH1 = h1;
    this->goalH2 = h2;
}

void DictionaryBruteProcess::setKnownPartsForSymbol1(QString prefix, QString suffix) {
    this->prefix1 = prefix;
    this->suffix1 = suffix;
}

void DictionaryBruteProcess::setKnownPartsForSymbol2(QString prefix, QString suffix) {
    this->prefix2 = prefix;
    this->suffix2 = suffix;
}

void DictionaryBruteProcess::setUseDemangler(bool useDemangler) {
    this->useDemangler = useDemangler;
}

void DictionaryBruteProcess::startBruteforce() {

    uint32_t startSym1 = Hashing::hash(prefix1, 0x1505);
    uint32_t endSym1 = Hashing::inverse_hash(goalH1, suffix1);

    // key: hash
    std::map<uint32_t, std::vector<uint32_t>> memo;

    emit updateProgress(0);

    for (int idx1 = 0; idx1 < wordList.size(); idx1++) {
        for (int idx2 = 0; idx2 < wordList.size(); idx2++) {

            uint32_t hash = Hashing::hash(wordList[idx2], Hashing::hash(wordList[idx1], startSym1));

            memo[hash].emplace_back((idx1 << 16) | idx2);
        }
        emit updateProgress(50.0 * ((double) idx1 / wordList.size()));
    }
    std::vector<std::string> results;
    for (int idx3 = 0; idx3 < wordList.size(); idx3++) {
        for (int idx4 = 0; idx4 < wordList.size(); idx4++) {
            uint32_t hash = Hashing::inverse_hash(Hashing::inverse_hash(endSym1, wordList[idx4]), wordList[idx3]);
            if (memo.count(hash) == 0) continue;

            // collision!
            for (uint32_t val : memo[hash]) {
                int idx1 = val >> 16;
                int idx2 = val & 0xFFFF;
                results.emplace_back(wordList[idx1] + wordList[idx2] + wordList[idx3] + wordList[idx4]);
            }
        }
        emit updateProgress(50.0 + 50.0 * ((double) idx3 / wordList.size()));
    }

    // filter results
    QStringList filteredRes = QStringList();
    std::string pre1str = prefix1.toStdString();
    std::string suf1str = suffix1.toStdString();
    for (std::string &r : results) {
        QString otherSym;
        if (useDemangler) {
            std::string fullString = pre1str + r + suf1str;
            otherSym = QString::fromStdString(nvidia_demangler::nvidia_demangle(fullString));
        } else {
            otherSym = prefix2 + QString::fromStdString(r) + suffix2;
        }
        if (Hashing::hash(otherSym, 0x1505) == goalH2) {
            filteredRes.append(QString::fromStdString(pre1str + r + suf1str));
        }
    }
    filteredRes.removeDuplicates();


    emit bruteforceEnded(filteredRes);
}
